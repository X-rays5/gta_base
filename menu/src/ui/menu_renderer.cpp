//
// Created by X-ray on 31/05/2025.
//

#include "menu_renderer.hpp"

#include "../render/renderer.hpp"
#include "../scripts/script_manager.hpp"
#include "../hooking/wndproc.hpp"
#include "components/label_component.hpp"

namespace base::menu::ui {
  MenuRenderer::MenuRenderer() {
    fallback_submenu_->AddComponent(components::LabelComponent("Invalid SubMenu"));

    wndproc_handler_id_ = hooking::kWNDPROC->AddWndProcHandler(this);
    script_id_ = scripts::kSCRIPTMANAGER->AddScript(this);

    kMENU_RENDERER = this;
  }

  MenuRenderer::~MenuRenderer() {
    kMENU_RENDERER = nullptr;

    scripts::kSCRIPTMANAGER->RemoveScript(script_id_, GetScriptType());
    hooking::kWNDPROC->RemoveWndProcHandler(wndproc_handler_id_);
  }

  void MenuRenderer::ScriptTick() {
    if (is_menu_opened_)
      RenderMenu();
  }

  void MenuRenderer::RenderMenu() {
    render::DrawQueueBuffer* draw_queue = render::kRENDERER->GetDrawQueueBuffer();
    auto submenu = GetCurrentSubmenu();
    if (!draw_queue || !submenu) {
      LOG_ERROR("Draw queue or submenu is null, cannot render menu.");
      return;
    }

    const Submenu::component_list& components = submenu->GetComponents();

    std::float_t y_offset = ui_props_.theme.y_position;
    y_offset = DrawTopBar(draw_queue, submenu->GetName(), y_offset);
    y_offset = DrawComponents(draw_queue, components, y_offset);
    DrawBottomBar(draw_queue, 0, components.size(), y_offset);
  }

  std::float_t MenuRenderer::DrawTopBar(render::DrawQueueBuffer* draw_queue, const std::string_view sub_name, std::float_t y_offset) {
    static std::float_t sub_title_x = ui_props_.theme.x_position + ui_props_.text_props.x_margin;
    static std::float_t sub_title_top_y_offset = ui_props_.menu_item_height / 2;
    auto sub_title_y = y_offset + sub_title_top_y_offset;

    draw_queue->AddCommand(render::RectBorder({ui_props_.theme.x_position, y_offset},{ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.background_color, ui_props_.seperator_color, false, true, false, false, ui_props_.seperator_height));
    y_offset += ui_props_.menu_item_height;
    draw_queue->AddCommand(render::PushFont("roboto-bold"));
    draw_queue->AddCommand(render::Text({sub_title_x, sub_title_y}, ui_props_.text_props.text_color, std::string(sub_name), ui_props_.text_props.font_size, false, false, true));
    draw_queue->AddCommand(render::PopFont());

    return y_offset;
  }

  std::float_t MenuRenderer::DrawComponents(render::DrawQueueBuffer* draw_queue, const Submenu::component_list& components, std::float_t y_offset) {
    const auto options_to_draw = components.size() > ui_props_.max_options_drawn ? ui_props_.max_options_drawn : components.size();
    auto y_size = ui_props_.menu_item_height * options_to_draw;

    draw_queue->AddCommand(render::Rect({ui_props_.theme.x_position, y_offset}, {ui_props_.menu_width, y_size}, ui_props_.background_color));
    for (std::size_t i = 0; i < options_to_draw; ++i) {
      DrawComponent(draw_queue, components[i].component.get(), y_offset);
      y_offset += ui_props_.menu_item_height;
    }

    return y_offset;
  }

  void MenuRenderer::DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, const std::float_t y_offset) const {
    if (component->HasCenterText()) {
      static auto text_x_pos = ui_props_.theme.x_position + ui_props_.menu_width / 2;
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({text_x_pos, text_y_pos}, ui_props_.text_props.sec_text_color, std::string(component->GetCenterText()), ui_props_.text_props.font_size, false, true, true));
      return;
    }

    if (component->HasLeftText()) {
      static auto left_text_x_pos = ui_props_.theme.x_position + ui_props_.text_props.x_margin;
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({left_text_x_pos, text_y_pos}, ui_props_.text_props.text_color, std::string(component->GetLeftText()), ui_props_.text_props.font_size, false, false, true));
    }

    if (component->HasRightText()) {
      static auto right_text_x_pos = ui_props_.theme.x_position + ui_props_.menu_width - ui_props_.text_props.x_margin;
      auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;
      draw_queue->AddCommand(render::Text({right_text_x_pos, text_y_pos}, ui_props_.text_props.text_color, std::string(component->GetRightText()), ui_props_.text_props.font_size, true, false, true));
    }
  }

  std::float_t MenuRenderer::DrawBottomBar(render::DrawQueueBuffer* draw_queue, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset) {
    auto option_count_x = ui_props_.theme.x_position + ui_props_.menu_width - ui_props_.text_props.x_margin;
    auto text_y_pos = y_offset + ui_props_.menu_item_height / 2;

    draw_queue->AddCommand(render::RectBorder({ui_props_.theme.x_position, y_offset}, {ui_props_.menu_width, ui_props_.menu_item_height}, ui_props_.background_color, ui_props_.seperator_color, true, false, false, false, ui_props_.seperator_height));
    draw_queue->AddCommand(render::PushFont("roboto-bold"));
    draw_queue->AddCommand(render::Text({option_count_x, text_y_pos}, ui_props_.text_props.text_color, fmt::format("{}/{}", cur_item_idx, item_count), ui_props_.text_props.font_size, true, false, true));
    draw_queue->AddCommand(render::PopFont());

    return y_offset + ui_props_.menu_item_height;
  }

}