//
// Created by X-ray on 31/05/2025.
//

#include "menu_renderer.hpp"

#include "../render/renderer.hpp"
#include "../scripts/script_manager.hpp"
#include "components/label_component.hpp"
#include "../render/animate.hpp"

namespace base::menu::ui {
  MenuRenderer::MenuRenderer() {
    fallback_option_ = std::make_shared<components::LabelComponent>("label/invalid_submenu");
    fallback_submenu_->AddComponent(components::LabelComponent("label/invalid_submenu"));

    script_id_ = scripts::kSCRIPTMANAGER->AddScript(this);

    kMENU_RENDERER = this;
  }

  MenuRenderer::~MenuRenderer() {
    kMENU_RENDERER = nullptr;

    scripts::kSCRIPTMANAGER->RemoveScript(script_id_, GetScriptType());
  }

  void MenuRenderer::ScriptTick() {
    if (menu_ui_key_state_.WasKeyPressed(VK_F4))
      is_menu_opened_ = !is_menu_opened_;
    if (menu_ui_key_state_.WasKeyPressed(VK_BACK))
      PopSubmenu();

    if (is_menu_opened_)
      RenderMenu();
  }

  void MenuRenderer::RenderMenu() {
    render::DrawQueueBuffer* draw_queue = render::kRENDERER->GetDrawQueueBuffer();
    const auto submenu = GetCurrentSubmenu();
    if (!draw_queue || !submenu) {
      LOG_ERROR("Draw queue or submenu is null, cannot render menu.");
      return;
    }

    submenu->UpdateComponents();
    if (menu_ui_key_state_.WasKeyPressed(VK_UP)) {
      submenu->Scroll(Submenu::ScrollDirection::kUP);
    } else if (menu_ui_key_state_.WasKeyPressed(VK_DOWN)) {
      submenu->Scroll(Submenu::ScrollDirection::kDOWN);
    }
    const Submenu::component_list_t& components = submenu->GetComponents();

    std::float_t y_offset = ui_props_.theme.y_position;
    y_offset = DrawTopBar(draw_queue, submenu->GetName(), y_offset);
    std::float_t top_bar_y_offset = y_offset;
    if (components.empty()) {
      draw_queue->AddCommand(render::Rect({ui_props_.theme.x_position, y_offset}, {ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.background_color));
      DrawComponent(draw_queue, fallback_option_.get(), y_offset, false);
      y_offset += ui_props_.menu_item_height;
    }

    y_offset = DrawComponents(draw_queue, top_bar_y_offset, submenu.get(), components, y_offset);
    DrawBottomBar(draw_queue, submenu->GetCurrentOptionIndexForDisplay(), submenu->GetOptionCountForDisplay(), y_offset);
  }

  std::float_t MenuRenderer::DrawTopBar(render::DrawQueueBuffer* draw_queue, const std::string_view sub_name, std::float_t y_offset) {
    static const std::float_t sub_title_x = ui_props_.theme.x_position + ui_props_.text_props.x_margin;
    static const std::float_t sub_title_top_y_offset = ui_props_.menu_item_height / 2;
    auto sub_title_y = y_offset + sub_title_top_y_offset;

    draw_queue->AddCommand(render::RectBorder({ui_props_.theme.x_position, y_offset},{ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.background_color, ui_props_.seperator_color, false, true, false, false, ui_props_.seperator_height));
    y_offset += ui_props_.menu_item_height;
    draw_queue->AddCommand(render::PushFont(ui_props_.text_props.font_bold));
    draw_queue->AddCommand(render::Text({sub_title_x, sub_title_y}, ui_props_.text_props.text_color, std::string(sub_name), ui_props_.text_props.font_size, false, false, true));
    draw_queue->AddCommand(render::PopFont());

    return y_offset;
  }

  std::float_t MenuRenderer::DrawComponents(render::DrawQueueBuffer* draw_queue, const std::float_t top_bar_y_offset, Submenu* submenu, const Submenu::component_list_t& components, std::float_t y_offset) {
    if (components.empty()) {
      return y_offset; // No components to draw
    }

    const std::size_t total_components = components.size();
    const std::uint32_t max_visible_options = ui_props_.max_options_drawn;
    const std::uint32_t options_to_draw = std::min(static_cast<std::uint32_t>(total_components), max_visible_options);

    // Calculate the starting index for the visible window
    std::size_t start_index = 0;
    if (total_components > max_visible_options) {
      if (submenu) {
        submenu->UpdateScrollOffset(max_visible_options);
        start_index = submenu->GetScrollOffset();
      }
    }

    std::float_t y_size = ui_props_.menu_item_height * static_cast<std::float_t>(options_to_draw);

    draw_queue->AddCommand(render::Rect({ui_props_.theme.x_position, y_offset}, {ui_props_.menu_width, y_size}, ui_props_.background_color));

    const std::size_t selected_component = DrawItemSelector(draw_queue, top_bar_y_offset, submenu);

    // Draw the visible window of components
    for (std::uint32_t i = 0; i < options_to_draw; ++i) {
      if (const std::size_t component_index = start_index + i; component_index < total_components) {
        DrawComponent(draw_queue, components[component_index].component.get(), y_offset, component_index == selected_component);
      }
      y_offset += ui_props_.menu_item_height;
    }

    return y_offset;
  }

  void MenuRenderer::DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, const std::float_t y_offset, bool selected) const {
    const ImColor text_color = selected ? ui_props_.text_props.inverse_text_color : ui_props_.text_props.text_color;

    if (component->HasCenterText()) {
      static const std::float_t text_x_pos = ui_props_.theme.x_position + GetMenuCenterX();
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({text_x_pos, text_y_pos}, text_color, component->GetCenterText(), ui_props_.text_props.font_size, false, true, true));
      return;
    }

    if (component->HasLeftText()) {
      static const std::float_t left_text_x_pos = ui_props_.theme.x_position + ui_props_.text_props.x_margin;
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({left_text_x_pos, text_y_pos}, text_color, component->GetLeftText(), ui_props_.text_props.font_size, false, false, true));
    }

    if (component->HasRightText()) {
      static const std::float_t right_text_x_pos = ui_props_.theme.x_position + ui_props_.menu_width - ui_props_.text_props.x_margin;
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({right_text_x_pos, text_y_pos}, text_color, component->GetRightText(), ui_props_.text_props.font_size, true, false, true));
    }
  }

  std::float_t MenuRenderer::DrawBottomBar(render::DrawQueueBuffer* draw_queue, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset) {
    auto option_count_x = ui_props_.theme.x_position + ui_props_.menu_width - ui_props_.text_props.x_margin;
    auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;

    std::string arrow_icon = ICON_FA_ARROW_DOWN_ARROW_UP;
    if (cur_item_idx == 1) {
      arrow_icon = ICON_FA_ARROW_DOWN;
    } else if (cur_item_idx == item_count) {
      arrow_icon = ICON_FA_ARROW_UP;
    }

    draw_queue->AddCommand(render::RectBorder({ui_props_.theme.x_position, y_offset}, {ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.background_color, ui_props_.seperator_color, true, false, false, false, ui_props_.seperator_height));
    draw_queue->AddCommand(render::PushFont(ui_props_.text_props.font_bold));
    draw_queue->AddCommand(render::Text({ui_props_.theme.x_position + GetMenuCenterX(), text_y_pos}, ui_props_.text_props.text_color, arrow_icon, ui_props_.text_props.font_size, false, true, true));
    draw_queue->AddCommand(render::Text({option_count_x, text_y_pos}, ui_props_.text_props.text_color, fmt::format("{}/{}", cur_item_idx, item_count), ui_props_.text_props.font_size, true, false, true));
    draw_queue->AddCommand(render::PopFont());

    return y_offset + ui_props_.menu_item_height;
  }

  std::size_t MenuRenderer::DrawItemSelector(render::DrawQueueBuffer* draw_queue, const std::float_t y_top, const Submenu* submenu) {
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
      const std::size_t animation_duration = std::abs(start_y - new_target_y) > ui_props_.menu_item_height * 2 ? 30 : 70;

      selector_animation_ = std::make_unique<base::render::animate::Lerp<std::float_t>>(start_y, new_target_y, animation_duration);
    }

    // Animate towards target position using the Lerp class
    if (selector_animation_) {
      current_selector_y_ = selector_animation_->Animate(delta_time);
    }

    // Draw the selector background
    draw_queue->AddCommand(render::Rect({ui_props_.theme.x_position, current_selector_y_}, {ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.selector_color));

    return current_option_index;
  }

  std::float_t MenuRenderer::GetMenuCenterX() const {
    static const std::float_t center_x = ui_props_.theme.x_position + ui_props_.menu_width / 2;
    return center_x;
  }

}