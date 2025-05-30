//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12190029
#define MANAGER_HPP_12190029
#include <ankerl/unordered_dense.h>

#define TRANSLATE_LITERAL(str) base::ui::localization::kMANAGER->Localize(xorstr_(str));

namespace base::ui::localization {
    using translation_map_t = ankerl::unordered_dense::map<std::string, std::string>;

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

        [[nodiscard]] std::string Localize(const char* key);
        [[nodiscard]] Status SetActiveTranslation(const std::string& name, bool save_current = true);

    private:
        Translation translation_;
        std::string active_translation_;
    };

    inline Manager* kMANAGER{};
}

namespace base {
    [[nodiscard]] inline std::string operator ""_l10n(const char* key) {
        return ui::localization::kMANAGER->Localize(key);
    }
}


#endif //MANAGER_HPP_12190029
