//
// Created by X-ray on 31/05/2025.
//

#include "menu_renderer.hpp"

#include "../options/base_option.hpp"
#include "../render/animate.hpp"
#include "../render/renderer.hpp"
#include "components/label_component.hpp"
#include "layout/home.hpp"

namespace base::menu::ui {
  MenuRenderer::MenuRenderer() {
    fallback_option_ = std::make_shared<components::LabelComponent>("label/invalid_submenu");
    fallback_submenu_->AddComponent(components::LabelComponent("label/invalid_submenu"));

    kMENU_RENDERER = this;

    layout::InitHomeLayout();

    // Register as mouse input listener with Renderer
    if (render::kRENDERER) {
      render::kRENDERER->RegisterMouseInputListener(this);
    }

    // Initialize fade animation - start visible
    fade_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(1.0f, 1.0f, 0);
    current_alpha_ = 1.0f;

    render::kRENDER_THREAD->AddRenderCallback(0, [](render::DrawQueueBuffer* draw_queue_buffer) {
      if (!kMENU_RENDERER)
        return;

      if (kMENU_RENDERER->menu_ui_key_state_.WasKeyPressed(VK_F4)) {
        if (kMENU_RENDERER->is_menu_opened_) {
          kMENU_RENDERER->CloseMenu();
        } else {
          kMENU_RENDERER->OpenMenu();
        }
      }
      if (kMENU_RENDERER->menu_ui_key_state_.WasKeyPressed(VK_BACK)) {
        if (kMENU_RENDERER->IsOnHomeSubmenu()) {
          kMENU_RENDERER->CloseMenu();
        } else {
          kMENU_RENDERER->PopSubmenu();
        }
      }

      kMENU_RENDERER->UpdateFadeAnimation();
      kMENU_RENDERER->UpdateHeightAnimation();

      // Render menu if visible or fading
      if (kMENU_RENDERER->current_alpha_ > 0.0f) {
        draw_queue_buffer->AddCommand(render::PushFont(kMENU_RENDERER->ui_props_.theme->text_props.font_bold));
        kMENU_RENDERER->RenderMenu(draw_queue_buffer);
        draw_queue_buffer->AddCommand(render::PopFont());
      }
    });
  }

  MenuRenderer::~MenuRenderer() {
    // Unregister as mouse input listener
    if (render::kRENDERER) {
      render::kRENDERER->UnregisterMouseInputListener(this);
    }
    kMENU_RENDERER = nullptr;
  }

  void MenuRenderer::RenderMenu(render::DrawQueueBuffer* draw_queue) {
    const auto submenu = GetCurrentSubmenu();
    if (!draw_queue || !submenu) {
      LOG_ERROR("Draw queue or submenu is null, cannot render menu.");
      return;
    }

    submenu->UpdateComponents();

    // Handle mouse input before rendering
    HandleMouseInput(submenu.get());

    if (menu_ui_navigation.WasKeyPressed(VK_UP)) {
      submenu->Scroll(Submenu::ScrollDirection::kUP);
    } else if (menu_ui_navigation.WasKeyPressed(VK_DOWN)) {
      submenu->Scroll(Submenu::ScrollDirection::kDOWN);
    }

    if (menu_ui_navigation.WasKeyPressed(VK_RETURN)) {
      submenu->GetCurrentComponent()->HandleButtonPress(components::BaseComponent::PressedButton::kSUBMIT);
    } else if (menu_ui_navigation.WasKeyPressed(VK_LEFT)) {
      submenu->GetCurrentComponent()->HandleButtonPress(components::BaseComponent::PressedButton::kLEFT);
    } else if (menu_ui_navigation.WasKeyPressed(VK_RIGHT)) {
      submenu->GetCurrentComponent()->HandleButtonPress(components::BaseComponent::PressedButton::kRIGHT);
    }

    if (option_interaction.WasKeyPressed(VK_F11)) {
      submenu->GetCurrentComponent()->Save();
    } else if (option_interaction.WasKeyPressed(VK_F12)) {
      //submenu->GetCurrentComponent()->HandleButtonPress(components::BaseComponent::PressedButton::kEXTRA_2);
    }

    const Submenu::component_list_t& components = submenu->GetComponents();

    std::float_t y_offset = ui_props_.theme->y_position;
    y_offset = DrawTopBar(draw_queue, submenu->GetName(), submenu->GetCurrentOptionIndexForDisplay(), submenu->GetOptionCountForDisplay(), y_offset);
    const std::float_t top_bar_y_offset = y_offset;
    if (components.empty()) {
      draw_queue->AddCommand(render::Rect({ui_props_.theme->x_position, y_offset}, {ui_props_.menu_width, ui_props_.menu_item_height}, ApplyAlphaToColor(ui_props_.theme->background_color)));
      DrawComponent(draw_queue, fallback_option_.get(), y_offset, false);
      y_offset += ui_props_.menu_item_height;
    }

    y_offset = DrawComponents(draw_queue, top_bar_y_offset, submenu.get(), components, y_offset);
    y_offset = DrawBottomBar(draw_queue, submenu->GetCurrentOptionIndexForDisplay(), submenu->GetOptionCountForDisplay(), y_offset);

    constexpr std::float_t info_box_spacing = 0.01f;
    y_offset += info_box_spacing;

    DrawInfoBox(draw_queue, submenu.get(), y_offset);
  }

  std::float_t MenuRenderer::DrawTopBar(render::DrawQueueBuffer* draw_queue, const std::string_view sub_name, const std::size_t cur_item_idx, const std::size_t item_count, std::float_t y_offset) {
    const auto sub_name_x = ui_props_.theme->x_position + ui_props_.theme->text_props.x_margin;
    const auto option_count_x = ui_props_.theme->x_position + ui_props_.menu_width - ui_props_.theme->text_props.x_margin;
    const auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;

    const std::string opt_idx = fmt::format("{}/{}", cur_item_idx, item_count);

    const float max_name_width = ui_props_.menu_width - (2 * ui_props_.theme->text_props.x_margin) - render::draw_helpers::CalcTextSize(nullptr, ui_props_.theme->text_props.font_size, ui_props_.theme->text_props.font_bold).y;

    auto display_name = std::string(sub_name);
    render::draw_helpers::WordWrap(ui_props_.theme->text_props.font_size, display_name, max_name_width, 1);

    if (!display_name.empty() && display_name.back() == '\n') {
      display_name.pop_back();
    }

    draw_queue->AddCommand(render::RectBorder(
      {ui_props_.theme->x_position - render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x, y_offset},
      {ui_props_.menu_width + render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x * 2, ui_props_.menu_item_height},
      ApplyAlphaToColor(ui_props_.theme->background_color),
      ApplyAlphaToColor(ui_props_.theme->seperator_color),
      true, false, true, true,
      ui_props_.seperator_height));

    draw_queue->AddCommand(render::Text({sub_name_x, text_y_pos}, ApplyAlphaToColor(ui_props_.theme->text_props.text_color), display_name, ui_props_.theme->text_props.font_size, false, false, true));
    draw_queue->AddCommand(render::Text({option_count_x, text_y_pos}, ApplyAlphaToColor(ui_props_.theme->text_props.text_color), opt_idx, ui_props_.theme->text_props.font_size, true, false, true));

    y_offset += ui_props_.menu_item_height;

    return y_offset;
  }

  std::float_t MenuRenderer::DrawComponents(render::DrawQueueBuffer* draw_queue, const std::float_t top_bar_y_offset, Submenu* submenu, const Submenu::component_list_t& components, std::float_t y_offset) {
    if (components.empty()) {
      return y_offset; // No components to draw
    }

    if (!submenu || submenu->GetComponents().empty()) {
      return y_offset; // No components to select
    }

    const std::size_t total_components = components.size();
    const std::uint32_t max_visible_options = ui_props_.max_options_drawn;
    const std::uint32_t options_to_draw = std::min(static_cast<std::uint32_t>(total_components), max_visible_options);

    // Calculate the starting index for the visible window
    std::size_t start_index = 0;
    if (total_components > max_visible_options) {
        submenu->UpdateScrollOffset(max_visible_options);
        start_index = submenu->GetScrollOffset();
    }

    std::float_t target_y_size = ui_props_.menu_item_height * static_cast<std::float_t>(options_to_draw);

    // Set target height and animate if it changes
    SetTargetMenuHeight(target_y_size);

    // Use the animated height for rendering
    std::float_t animated_y_size = current_menu_height_;

    draw_queue->AddCommand(render::RectBorder(
      {ui_props_.theme->x_position - render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x, y_offset},
      {ui_props_.menu_width + render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x * 2, animated_y_size},
      ApplyAlphaToColor(ui_props_.theme->background_color),
      ApplyAlphaToColor(ui_props_.theme->seperator_color),
      false, false, true, true,
      ui_props_.seperator_height));

    const std::float_t y_pos = DrawItemSelector(draw_queue, top_bar_y_offset, submenu);

    // Draw the visible window of components
    std::float_t current_y_offset = y_offset;
    for (std::uint32_t i = 0; i < options_to_draw; ++i) {
      if (const std::size_t component_index = start_index + i; component_index < total_components) {
        // Check if the selector's center is within the current item's bounds
        const float selector_center = y_pos + (ui_props_.menu_item_height / 2.0f);
        const bool is_selected = selector_center >= current_y_offset &&
                                 selector_center < current_y_offset + ui_props_.menu_item_height;

        DrawComponent(draw_queue, components[component_index].component.get(), current_y_offset, is_selected);
      }
      current_y_offset += ui_props_.menu_item_height;
    }

    return y_offset + animated_y_size;
  }

  void MenuRenderer::DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, const std::float_t y_offset, const bool inverse_text) const {
    const RgbColor text_color = inverse_text ? ui_props_.theme->text_props.inverse_text_color : ui_props_.theme->text_props.text_color;
    auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;

    const std::float_t left_text_x_pos = ui_props_.theme->x_position + ui_props_.theme->text_props.x_margin;
    const std::float_t right_text_x_pos = ui_props_.theme->x_position + ui_props_.menu_width - ui_props_.theme->text_props.x_margin;
    const std::float_t center_x_pos = GetMenuCenterX();

    if (component->HasCenterText()) {
      draw_queue->AddCommand(render::Text({center_x_pos, text_y_pos}, ApplyAlphaToColor(ui_props_.theme->text_props.sec_text_color), component->GetCenterText(), ui_props_.theme->text_props.font_size, false, true, true));
      return;
    }

    if (component->HasLeftText()) {
      draw_queue->AddCommand(render::Text({left_text_x_pos, text_y_pos}, ApplyAlphaToColor(text_color), component->GetLeftText(), ui_props_.theme->text_props.font_size, false, false, true));
    }

    if (component->HasRightText()) {
      draw_queue->AddCommand(render::Text({right_text_x_pos, text_y_pos}, ApplyAlphaToColor(text_color), component->GetRightText(), ui_props_.theme->text_props.font_size, true, false, true));
    }
  }

  std::float_t MenuRenderer::DrawBottomBar(render::DrawQueueBuffer* draw_queue, const std::size_t cur_item_idx, const std::size_t item_count, std::float_t y_offset) const {
    auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
    const std::float_t center_x_pos = GetMenuCenterX();

    std::string arrow_icon = ICON_FA_ARROW_DOWN_ARROW_UP;
    if (cur_item_idx == 1) {
      arrow_icon = ICON_FA_ARROW_DOWN;
    } else if (cur_item_idx == item_count) {
      arrow_icon = ICON_FA_ARROW_UP;
    }

    draw_queue->AddCommand(render::RectBorder(
      {ui_props_.theme->x_position - render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x, y_offset},
      {ui_props_.menu_width + render::draw_helpers::ScaleSquare(ui_props_.seperator_height).x * 2, ui_props_.menu_item_height},
      ApplyAlphaToColor(ui_props_.theme->background_color),
      ApplyAlphaToColor(ui_props_.theme->seperator_color),
      false, true, true, true,
      ui_props_.seperator_height));
    draw_queue->AddCommand(render::Text({center_x_pos, text_y_pos}, ApplyAlphaToColor(ui_props_.theme->text_props.text_color), arrow_icon, ui_props_.theme->text_props.font_size, false, true, true));

    return y_offset + ui_props_.menu_item_height;
  }

  std::float_t MenuRenderer::DrawItemSelector(render::DrawQueueBuffer* draw_queue, const std::float_t y_top, const Submenu* submenu) {
    if (!submenu || submenu->GetComponents().empty()) {
      return 0; // No components to select
    }

    const auto& components = submenu->GetComponents();
    const std::size_t current_option_index = submenu->GetCurrentOptionIndex();
    const std::size_t total_components = components.size();
    const std::uint32_t max_visible_options = ui_props_.max_options_drawn;

    if (current_option_index >= total_components) {
      return 0; // Invalid selection index
    }

    // Calculate the target Y position for the selector based on the current selection
    std::size_t visible_index = current_option_index;

    // If we have scrolling, adjust the visible index
    if (total_components > max_visible_options) {
      const std::size_t scroll_offset = submenu->GetScrollOffset();

      // Check if the selected item is currently visible
      if (current_option_index < scroll_offset ||
          current_option_index >= scroll_offset + max_visible_options) {
        return 0; // Selected item is not currently visible, don't draw selector
      }

      visible_index = current_option_index - scroll_offset;
    }

    // Calculate target position
    const std::float_t new_target_y = y_top + (visible_index * ui_props_.menu_item_height);

    // Update animation timing
    const auto current_time = std::chrono::steady_clock::now();
    const auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_update_time_).count();
    last_update_time_ = current_time;

    // Check if the target position has changed (user navigated) or we need to initialize animation
    if (constexpr std::float_t position_tolerance = 0.001f; !selector_animation_ || std::abs(new_target_y - selector_animation_->GetEnd()) > position_tolerance) {
      // Create new animation from current position to new target
      const std::float_t start_y = selector_animation_ ? current_selector_y_ : new_target_y;

      // If this is a big jump (like menu switch), use shorter animation
      const std::size_t animation_duration = std::abs(start_y - new_target_y) > ui_props_.menu_item_height * 2 ? 50 : 80;

      selector_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(start_y, new_target_y, animation_duration);
    }

    // Animate towards target position using the Lerp class
    if (selector_animation_) {
      current_selector_y_ = selector_animation_->Animate(delta_time);
    }

    // Draw the selector background
    draw_queue->AddCommand(render::RectBorder(
      {ui_props_.theme->x_position, current_selector_y_},
      {ui_props_.menu_width, ui_props_.menu_item_height},
      ApplyAlphaToColor(ui_props_.selector_color),
      ApplyAlphaToColor(ui_props_.theme->seperator_color),
      true, true, false, false,
      ui_props_.seperator_height));

    return current_selector_y_;
  }

  std::float_t MenuRenderer::GetMenuCenterX() const {
    const std::float_t center_x = ui_props_.theme->x_position + ui_props_.menu_width / 2;
    return center_x;
  }

  void MenuRenderer::UpdateFadeAnimation() {
    if (!fade_animation_) {
      return;
    }

    const auto current_time = std::chrono::steady_clock::now();
    const auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - fade_animation_update_time_).count();
    fade_animation_update_time_ = current_time;

    current_alpha_ = fade_animation_->Animate(delta_time);
  }

  void MenuRenderer::UpdateHeightAnimation() {
    if (!height_animation_) {
      return;
    }

    const auto current_time = std::chrono::steady_clock::now();
    const auto delta_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - height_animation_update_time_).count();
    height_animation_update_time_ = current_time;

    current_menu_height_ = height_animation_->Animate(delta_time);
  }

  void MenuRenderer::SetTargetMenuHeight(std::float_t target_height) {
    // Only animate if the height actually changes and it's not the first initialization
    if (current_menu_height_ > 0.0f && std::abs(target_height - target_menu_height_) > 0.0001f) {
      // Create animation from current height to target height - quick animation (50ms)
      height_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(current_menu_height_, target_height, 50);
    } else if (current_menu_height_ == 0.0f) {
      // Initialize without animation
      current_menu_height_ = target_height;
    }
    target_menu_height_ = target_height;
  }

  RgbColor MenuRenderer::ApplyAlphaToColor(const RgbColor& color) const {
    const std::uint8_t faded_alpha = static_cast<std::uint8_t>(color.a * current_alpha_);
    return RgbColor(color.r, color.g, color.b, faded_alpha);
  }

  void MenuRenderer::OpenMenu() {
    is_menu_opened_ = true;
    render::kRENDERER->RequestShowCursor();
    const std::float_t start_alpha = current_alpha_;
    constexpr std::float_t end_alpha = 1.0f;
    fade_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(start_alpha, end_alpha, 100);
  }

  void MenuRenderer::CloseMenu() {
    is_menu_opened_ = false;
    render::kRENDERER->RequestHideCursor();
    const std::float_t start_alpha = current_alpha_;
    constexpr std::float_t end_alpha = 0.0f;
    fade_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(start_alpha, end_alpha, 100);
  }

  void MenuRenderer::HandleMouseInput(Submenu* submenu) {
    if (!submenu || !is_menu_opened_) {
      mouse_in_menu_bounds_ = false;
      return;
    }

    // Get current mouse state directly from Windows, not from ImGui
    POINT mouse_pos;
    if (!GetCursorPos(&mouse_pos)) {
      mouse_in_menu_bounds_ = false;
      return;  // Failed to get mouse position
    }

    // Get window handle
    auto game_hwnd = win32::GetGameHwnd();
    if (!game_hwnd) {
      mouse_in_menu_bounds_ = false;
      return;
    }

    // Convert screen coordinates to client coordinates
    if (!ScreenToClient(game_hwnd.value(), &mouse_pos)) {
      mouse_in_menu_bounds_ = false;
      return;
    }

    // Get window client area size
    RECT client_rect;
    if (!GetClientRect(game_hwnd.value(), &client_rect)) {
      mouse_in_menu_bounds_ = false;
      return;
    }

    const float display_width = static_cast<float>(client_rect.right - client_rect.left);
    const float display_height = static_cast<float>(client_rect.bottom - client_rect.top);

    // Convert to normalized coordinates [0, 1]
    const float norm_mouse_x = static_cast<float>(mouse_pos.x) / display_width;
    const float norm_mouse_y = static_cast<float>(mouse_pos.y) / display_height;

    // Check if mouse is within menu bounds
    const float menu_left = ui_props_.theme->x_position;
    const float menu_right = ui_props_.theme->x_position + ui_props_.menu_width;
    const float menu_top = ui_props_.theme->y_position;

    const auto& components = submenu->GetComponents();
    const std::size_t total_components = components.size();
    const std::uint32_t max_visible_options = ui_props_.max_options_drawn;
    const std::uint32_t visible_items = std::min(static_cast<std::uint32_t>(total_components), max_visible_options);

    // Calculate the Y offset of the components area (after top bar)
    float components_y_offset = menu_top + ui_props_.menu_item_height; // After top bar

    mouse_in_menu_bounds_ = false;

    if (norm_mouse_x >= menu_left && norm_mouse_x <= menu_right && norm_mouse_y >= components_y_offset) {
      const float mouse_y_relative = norm_mouse_y - components_y_offset;
      const float item_height = ui_props_.menu_item_height;

      // Determine which item is under the mouse
      const std::int32_t item_index = static_cast<std::int32_t>(mouse_y_relative / item_height);

      if (item_index >= 0 && item_index < static_cast<std::int32_t>(visible_items)) {
        // Select the item under the mouse
        std::size_t start_index = 0;
        if (total_components > max_visible_options) {
          start_index = submenu->GetScrollOffset();
        }

        const std::size_t actual_index = start_index + item_index;
        const std::size_t current_index = submenu->GetCurrentOptionIndex();

        if (actual_index < total_components && actual_index != current_index) {
          submenu->SetCurrentOptionIndex(actual_index);
        }

        mouse_in_menu_bounds_ = true;
      }
    }
  }

  // MouseInputListener interface implementations
  void MenuRenderer::OnMouseLeftClick() {
    if (!is_menu_opened_ || !mouse_in_menu_bounds_) {
      return;
    }

    const auto submenu = GetCurrentSubmenu();
    if (!submenu) {
      return;
    }

    const auto current_component = submenu->GetCurrentComponent();
    if (current_component) {
      current_component->HandleButtonPress(components::BaseComponent::PressedButton::kSUBMIT);
    }
  }

  void MenuRenderer::OnMouseRightClick() {
    if (!is_menu_opened_ || !mouse_in_menu_bounds_) {
      return;
    }

    PopSubmenu();
  }

  void MenuRenderer::OnMouseWheel(float delta) {
    if (!is_menu_opened_) {
      return;
    }

    const auto submenu = GetCurrentSubmenu();
    if (!submenu) {
      return;
    }

    const auto& components = submenu->GetComponents();
    if (components.size() <= ui_props_.max_options_drawn) {
      return;
    }

    if (delta > 0) {
      submenu->Scroll(Submenu::ScrollDirection::kUP);
    } else {
      submenu->Scroll(Submenu::ScrollDirection::kDOWN);
    }
  }

  std::float_t MenuRenderer::DrawInfoBox(render::DrawQueueBuffer* draw_queue, const Submenu* submenu, std::float_t y_offset) const {
    if (!draw_queue || !submenu || submenu->GetComponents().empty()) {
      return y_offset;
    }

    const auto current_component = submenu->GetCurrentComponent();
    if (!current_component) {
      return y_offset;
    }

    const std::float_t text_max_x = ui_props_.menu_width - ui_props_.theme->text_props.x_margin * 2;

    std::string description = current_component->GetDescription();
    render::draw_helpers::WordWrap(ui_props_.theme->text_props.font_size, description, text_max_x, 2);

    if (current_component->IsSavable()) {
      std::string savable_text = localization::kMANAGER->Localize("info/save_able");
      render::draw_helpers::WordWrap(ui_props_.theme->text_props.font_size, savable_text, text_max_x, 1);
      if (description.empty()) {
        description = savable_text;
      } else {
        description += "\n" + savable_text;
      }
    }
    if (current_component->IsHotkeyAble()) {
      std::string hotkey_text = localization::kMANAGER->Localize("info/hotkey_able");
      render::draw_helpers::WordWrap(ui_props_.theme->text_props.font_size, hotkey_text, text_max_x, 1);
      if (description.empty()) {
        description = hotkey_text;
      } else {
        description += "\n" + hotkey_text;
      }
    }

    if (description.empty()) {
      return y_offset;
    }

    const ImVec2 text_size = render::draw_helpers::CalcTextSize(nullptr, ui_props_.theme->text_props.font_size, description, text_max_x);

    // Height = text height + top margin + bottom margin
    const std::float_t info_box_height = text_size.y + ui_props_.theme->text_props.y_margin * 2;

    // Draw the background box with border
    draw_queue->AddCommand(render::Rect({ui_props_.theme->x_position, y_offset}, {ui_props_.menu_width, info_box_height}, ApplyAlphaToColor(ui_props_.theme->background_color)));
    draw_queue->AddCommand(render::RectBorder({ui_props_.theme->x_position, y_offset}, {ui_props_.menu_width, info_box_height}, ApplyAlphaToColor(ui_props_.theme->background_color), ApplyAlphaToColor(ui_props_.theme->seperator_color), true, true, true, true, ui_props_.seperator_height));

    // Draw the info text as a single call
    const std::float_t text_x_pos = ui_props_.theme->x_position + ui_props_.theme->text_props.x_margin;
    const std::float_t text_y_pos = y_offset + ui_props_.theme->text_props.y_margin;
    draw_queue->AddCommand(render::Text({text_x_pos, text_y_pos}, ApplyAlphaToColor(ui_props_.theme->text_props.text_color), description, ui_props_.theme->text_props.font_size, false, false, false));

    return y_offset + info_box_height;
  }

}























