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
    void MergeFa(const std::float_t font_size) {
      static constexpr ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, '\0'};
      ImFontConfig icons_config;
      icons_config.MergeMode = true;
      icons_config.PixelSnapH = true;
      icons_config.FontDataOwnedByAtlas = false;
      icons_config.GlyphOffset.y = 5;

      static const auto font_awesome = b::embed<"assets/fonts/fa-sharp-solid-900.ttf">();

      if (!ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<void*>(static_cast<const void*>(font_awesome.data())), font_awesome.size(), font_size, &icons_config, icons_ranges)) {
        LOG_ERROR("Failed to merge fa");
      }
    }
  }

  Manager::Manager(const std::float_t default_font_size) {
    ImGui::GetIO().Fonts->Clear();
    static const auto roboto_mono_regular = b::embed<"assets/fonts/RobotoMono-Regular.ttf">();
    if (!LoadFontFromMemory("roboto", const_cast<void*>(static_cast<const void*>(roboto_mono_regular.data())), roboto_mono_regular.size(), default_font_size)) {
      const ImFont* font = ImGui::GetIO().Fonts->AddFontDefault();
      FinalizeLoading("default", font, default_font_size, true);
    }

    static const auto roboto_mono_bold = b::embed<"assets/fonts/RobotoMono-Bold.ttf">();
    if (!LoadFontFromMemory("roboto-bold", const_cast<void*>(static_cast<const void*>(roboto_mono_bold.data())), roboto_mono_bold.size(), default_font_size)) {
      LOG_ERROR("Failed to load roboto-bold font from memory.");
    }

    kMANAGER = this;
  }

  Manager::~Manager() {
    kMANAGER = nullptr;

    ImGui::GetIO().Fonts->Clear();
    ImGui::GetIO().Fonts->AddFontDefault();
    if (!ImGui::GetIO().Fonts->IsBuilt())
      ImGui::GetIO().Fonts->Build();
  }

  bool Manager::LoadFontFromDisk(const std::string& name, const std::filesystem::path& path, const std::float_t font_size, const bool merge_fa) {
    base::common::concurrency::ScopedSpinlock lock(lock_);

    if (name.empty()) {
      LOG_ERROR("Tried to load font with an empty name.");
      return false;
    }

    if (font_size <= 0.f) {
      LOG_ERROR("Tried to load font {} with a zero or negative size.", name);
      return false;
    }

    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font with the name {}", name);
      return false;
    }

    if (!std::filesystem::is_regular_file(path)) {
      LOG_ERROR("Tried to load {} font that doesn't exist from: {}", name, path.string());
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    const ImFont* font = io.Fonts->AddFontFromFileTTF(path.string().c_str(), font_size);
    if (!font) {
      LOG_ERROR("Failed to load {} font from disk.", name);
      return false;
    }

    return FinalizeLoading(name, font, font_size, merge_fa);
  }

  bool Manager::LoadFontFromMemory(const std::string& name, void* font_data, const std::int32_t font_data_size, const std::float_t font_size, const bool merge_fa) {
    base::common::concurrency::ScopedSpinlock lock(lock_);

    if (name.empty()) {
      LOG_ERROR("Tried to load font with an empty name.");
      return false;
    }

    if (font_size <= 0.f) {
      LOG_ERROR("Tried to load font {} with a zero or negative size.", name);
      return false;
    }

    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font {}.", name);
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;

    const ImFont* font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, font_size, &cfg);
    if (!font) {
      LOG_ERROR("Failed to load {} font from memory.", name);
      return false;
    }

    return FinalizeLoading(name, font, font_size, merge_fa);
  }

  bool Manager::LoadFontFromMemoryCompressed(const std::string& name, const void* font_data, const std::int32_t font_data_size, const std::float_t font_size, const bool merge_fa) {
    base::common::concurrency::ScopedSpinlock lock(lock_);

    if (name.empty()) {
      LOG_ERROR("Tried to load font with an empty name.");
      return false;
    }

    if (font_size <= 0.f) {
      LOG_ERROR("Tried to load font {} with a zero or negative size.", name);
      return false;
    }

    if (fonts_.contains(name)) {
      LOG_WARN("Tried to load an already loaded font {}.", name);
      return false;
    }

    const ImGuiIO& io = ImGui::GetIO();

    ImFontConfig cfg;
    cfg.FontDataOwnedByAtlas = false;

    const ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(font_data, font_data_size, font_size, &cfg);
    if (!font) {
      LOG_ERROR("Failed to load {} font from memory.", name);
      return false;
    }

    return FinalizeLoading(name, font, font_size, merge_fa);
  }

  void Manager::PushFont(const std::string& name) {
    base::common::concurrency::ScopedSpinlock lock(lock_);
    if (const auto it = fonts_.find(name); it != fonts_.end() && it->second) {
      ImGui::PushFont(static_cast<ImFont*>(it->second));
    } else {
      LOG_WARN("Tried to push non existing font {}", name);
    }
  }

  void Manager::PopFont() {
    ImGui::PopFont();
  }

  ImFont* Manager::GetFont(const std::string& name) {
    base::common::concurrency::ScopedSpinlock lock(lock_);
    if (const auto it = fonts_.find(name); it != fonts_.end() && it->second) {
      return static_cast<ImFont*>(it->second);
    }

    LOG_WARN("Tried to get non existing font {}", name);
    return nullptr;
  }

  bool Manager::FinalizeLoading(const std::string& name, const void* font, const std::float_t font_size, const bool merge_fa) {
    if (merge_fa)
      MergeFa(font_size);

    if (!ImGui::GetIO().Fonts->IsBuilt())
      ImGui::GetIO().Fonts->Build();

    fonts_[name] = const_cast<void*>(font);

    LOG_INFO("Successfully loaded font {} with size {}.", name, font_size);
    return true;
  }
}
