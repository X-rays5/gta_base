//
// Created by X-ray on 29/12/2023.
//

#include "imfont.hpp"
#include <base-common/logging/logging_macro.hpp>
#include <battery/embed.hpp>
#include <imgui/imgui.h>
#include "IconsFontAwesome6.hpp"

namespace imfont {
  namespace {
    void MergeFa() {
      constexpr std::array<ImWchar, 2> icons_ranges = {ICON_MIN_FA, ICON_MAX_FA};
      ImFontConfig icons_config;
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      icons_config.FontDataOwnedByAtlas = false;
      icons_config.GlyphOffset.y = 5;

      static const auto font_awesome = b::embed<"assets/fonts/fa-solid-900.ttf">();

      if (!ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<void*>(static_cast<const void*>(font_awesome.data())), font_awesome.size(), 40, &icons_config, icons_ranges.data())) {
        LOG_ERROR("Failed to merge fa");
      }
    }
  }

  Manager::Manager() {
    ImGui::GetIO().Fonts->Clear();
    static const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
    if (!LoadFontFromMemory("roboto", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size())) {
      const ImFont* font = ImGui::GetIO().Fonts->AddFontDefault();
      FinalizeLoading("default", font, true);
    }

    kMANAGER = this;
  }

  Manager::~Manager() {
    kMANAGER = nullptr;
  }

  bool Manager::LoadFontFromDisk(const std::string& name, const std::filesystem::path& path, const bool merge_fa) {
    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font with the name {}", name);
      return false;
    }

    if (!std::filesystem::is_regular_file(path)) {
      LOG_ERROR("Tried to load {} font that doesn't exist from: {}", name, path.string());
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    const ImFont* font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), 40);
    if (!font) {
      LOG_ERROR("Failed to load {} font from disk.", name);
      return false;
    }

    return FinalizeLoading(name, font, merge_fa);
  }

  bool Manager::LoadFontFromMemory(const std::string& name, void* font_data, const std::int32_t font_data_size, const bool merge_fa) {
    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font {}.", name);
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;

    const ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 40, &cfg);
    if (!font) {
      LOG_ERROR("Failed to load {} font from memory.", name);
      return false;
    }

    return FinalizeLoading(name, font, merge_fa);
  }

  bool Manager::LoadFontFromMemoryCompressed(const std::string& name, const void* font_data, const std::int32_t font_data_size, const bool merge_fa) {
    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font {}.", name);
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;

    const ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(font_data, font_data_size, 40, &cfg);
    if (!font) {
      LOG_ERROR("Failed to load {} font from memory.", name);
      return false;
    }

    return FinalizeLoading(name, font, merge_fa);
  }

  void Manager::PushFont(const std::string& name) {
    if (auto it = fonts_.find(name); it != fonts_.end() && it->second) {
      ImGui::PushFont(static_cast<ImFont*>(it->second));
    } else {
      LOG_WARN("Tried to push non existing font {}", name);
    }
  }

  void Manager::PopFont() {
    ImGui::PopFont();
  }

  bool Manager::FinalizeLoading(const std::string& name, const void* font, const bool merge_fa) {
    if (merge_fa)
      MergeFa();

    if (!ImGui::GetIO().Fonts->IsBuilt())
      ImGui::GetIO().Fonts->Build();

    fonts_[name] = const_cast<void*>(font);

    return true;
  }
}
