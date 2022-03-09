//
// Created by X-ray on 3/9/2022.
//

#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

namespace ui {
  void Tick() {
    auto d3d = ImGui::GetBackgroundDrawList();

    d3d->AddTriangle(ImVec2(50,50), ImVec2(50, 100), ImVec2(100, 75), ImColor(243, 23, 56));
  }
}