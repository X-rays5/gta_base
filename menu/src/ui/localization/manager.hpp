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
        translation_map_t loaded_translation_;

        Status Load();

        Status Save();

        void Merge();
    };

    class Manager {
    public:
        Manager();

        ~Manager();

        std::string Localize(const char* key) {
            const auto it = translation_.loaded_translation_.find(key);

            return (it != translation_.loaded_translation_.end()) ? it->second : key;
        }

    private:
        Translation translation_;

    private:
    };

    inline Manager* kMANAGER{};

    inline std::string operator ""_l10n(const char* key) {
        return kMANAGER->Localize(key);
    }
}
#endif //MANAGER_HPP_12190029
