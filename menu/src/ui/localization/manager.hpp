//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12190029
#define MANAGER_HPP_12190029
#include <ankerl/unordered_dense.h>

#define TRANSLATE_LITERAL(str) base::ui::localization::kMANAGER->Localize(xorstr_(str));

namespace base::menu::ui::localization {
  struct TransparentHash {
    using is_transparent = void; // enables heterogeneous lookup

    size_t operator()(const std::string& key) const noexcept {
      return ankerl::unordered_dense::hash<std::string>{}(key);
    }

    size_t operator()(const std::string_view key) const noexcept {
      return ankerl::unordered_dense::hash<std::string_view>{}(key);
    }
  };

  struct TransparentEqual {
    using is_transparent = void;

    // std::string vs std::string
    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
      return lhs == rhs;
    }

    // std::string vs string_view
    bool operator()(const std::string& lhs, const std::string_view rhs) const noexcept {
      return lhs == rhs;
    }

    bool operator()(const std::string_view lhs, const std::string& rhs) const noexcept {
      return lhs == rhs;
    }

    // string_view vs string_view
    bool operator()(const std::string_view lhs, const std::string_view rhs) const noexcept {
      return lhs == rhs;
    }
  };

  using translation_map_t = ankerl::unordered_dense::map<std::string, std::string, TransparentHash, TransparentEqual>;

  extern const translation_map_t default_translation;

  struct Translation {
    translation_map_t loaded_translation_ = default_translation;

    [[nodiscard]] Status Load(const std::string& name);

    [[nodiscard]] Status Save(const std::string& name);

    [[nodiscard]] Status Merge(translation_map_t tmp_translation);

    static void WriteDefaultTranslation();
  };

  class Manager {
  public:
    Manager();

    ~Manager();

    [[nodiscard]] std::string Localize(std::string_view key);
    [[nodiscard]] Status SetActiveTranslation(const std::string& name, bool save_current = true);

  private:
    Translation translation_;
    std::string active_translation_ = "default";
  };

  inline Manager* kMANAGER{};
}

namespace base::menu {
  [[nodiscard]] inline std::string operator ""_l10n(const char* key) {
    return ui::localization::kMANAGER->Localize(key);
  }
}


#endif //MANAGER_HPP_12190029
