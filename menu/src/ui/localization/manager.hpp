//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12190029
#define MANAGER_HPP_12190029
#include <ankerl/unordered_dense.h>

namespace base::ui::localization {
    using translation_map_t = ankerl::unordered_dense::map<std::string, std::string>;

    extern const translation_map_t default_translation;

    struct Translation {
        translation_map_t loaded_translation_ = default_translation;

        [[nodiscard]] Status Load();

        [[nodiscard]] Status Save();

        [[nodiscard]] Status Merge(translation_map_t tmp_translation);
    };

    class Manager {
    public:
        Manager();

        ~Manager();

        [[nodiscard]] std::string Localize(const char* key) {
            const auto it = translation_.loaded_translation_.find(key);

            return (it != translation_.loaded_translation_.end()) ? it->second : key;
        }

    private:
        Translation translation_;
    };

    inline Manager* kMANAGER{};

    [[nodiscard]] inline std::string operator ""_l10n(const char* key) {
        return kMANAGER->Localize(key);
    }
}
#endif //MANAGER_HPP_12190029
