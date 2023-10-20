//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#define BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
#include <vector>
#include <memory>
#include <imgui.h>
#include "draw_util.hpp"

namespace base::render {
  class BaseDrawCommand {
  public:
    virtual void Draw() = 0;
  };

  class DrawList {
  public:
    template<typename T> requires std::is_base_of_v<BaseDrawCommand, T>
    FORCE_INLINE void AddCommand(T command) {
      draw_commands_.push_back(std::make_unique<T>(command));
    }

    FORCE_INLINE void Draw() {
      for (auto&& command : draw_commands_) {
        command->Draw();
      }
    }

  private:
    std::vector<std::unique_ptr<BaseDrawCommand>> draw_commands_;

  private:

  };

  class Draw {
  public:

    FORCE_INLINE void RenderFrame() {
      draw_list_.Draw();
    }

    static FORCE_INLINE ImDrawList* GetImGuiDrawList() {
      return ImGui::GetForegroundDrawList();
    }

  private:
    DrawList draw_list_;

  private:

  };
}
#endif //BASE_MODULES_DRAW_A89C088DF5454E269488B233901B0790_HPP
