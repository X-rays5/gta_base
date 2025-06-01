//
// Created by X-ray on 31/05/2025.
//

#ifndef MENU_RENDERER_HPP_05191653
#define MENU_RENDERER_HPP_05191653
#include <atomic>
#include <stack>
#include <ankerl/unordered_dense.h>
#include <magic_enum/magic_enum.hpp>
#include "submenu.hpp"
#include "theme.hpp"
#include "../hooking/wndproc.hpp"
#include "../scripts/base_script.hpp"

namespace base::menu::render {
  class DrawQueueBuffer;
}

namespace base::menu::ui {
  class MenuRenderer final : public hooking::WndProcEventListener, public scripts::BaseScript {
  public:
    MenuRenderer();
    virtual ~MenuRenderer() override;

    virtual void ScriptInit() override {}
    virtual void ScriptTick() override;

    void RenderMenu();

    virtual Type GetScriptType() const override {
      return Type::MenuRenderThread;
    }

    void AddSubmenu(const std::string& id, Submenu&& submenu) {
      submenus_.emplace(id, std::make_shared<Submenu>(submenu));
      if (submenu_stack_.empty()) {
        submenu_stack_.push(id);
      }
    }

    void AddSubmenu(const SubmenuIDs id, Submenu&& submenu) {
      AddSubmenu(std::string(magic_enum::enum_name(id)), std::move(submenu));
    }

    std::shared_ptr<Submenu> GetCurrentSubmenu() {
      if (submenu_stack_.empty()) {
        return fallback_submenu_;
      }

      return submenus_[submenu_stack_.top()];
    }

    Status PushSubmenu(const std::string& id) {
      if (submenus_.find(id) == submenus_.end()) {
        return MakeFailure<ResultCode::kIO_ERROR>("Submenu not found: {}", id);
      }

      submenu_stack_.push(id);
      return {};
    }

    bool IsOnHomeSubmenu() const {
      return submenu_stack_.size() == 1;
    }

    void PopSubmenu() {
      if (!IsOnHomeSubmenu()) {
        submenu_stack_.pop();
      }
    }

    virtual void OnWndProc(HWND, UINT msg, WPARAM wparam, LPARAM) override {
      if (msg == WM_KEYDOWN && wparam == VK_F4) {
        is_menu_opened_ = !is_menu_opened_;
        LOG_DEBUG("Menu toggled: {}", is_menu_opened_ ? "opened" : "closed");
      }
    }

  private:
    MenuRenderProperties ui_props_;
    ankerl::unordered_dense::map<std::string, std::shared_ptr<Submenu>> submenus_;
    std::stack<std::string> submenu_stack_;
    std::size_t wndproc_handler_id_ = 0;
    std::size_t script_id_ = 0;
    std::atomic<bool> is_menu_opened_ = true;
    std::shared_ptr<Submenu> fallback_submenu_ = std::make_shared<Submenu>("Fallback SubMenu");

  private:
    std::float_t DrawTopBar(render::DrawQueueBuffer* draw_queue, std::string_view sub_name, std::float_t y_offset);
    std::float_t DrawComponents(render::DrawQueueBuffer* draw_queue, const Submenu::component_list& components, std::float_t y_offset);
    void DrawComponent(render::DrawQueueBuffer* draw_queue, const components::BaseComponent* component, std::float_t y_offset) const;
    std::float_t DrawBottomBar(render::DrawQueueBuffer* draw_queue, std::size_t cur_item_idx, std::size_t item_count, std::float_t y_offset);
  };

  inline MenuRenderer* kMENU_RENDERER{};
}

#endif //MENU_RENDERER_HPP_05191653
