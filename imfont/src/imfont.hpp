//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12174322
#define MANAGER_HPP_12174322
#include <filesystem>
#include <ankerl/unordered_dense.h>
#include <base-common/concurrency/spinlock.hpp>

struct ImFont;

namespace imfont {
  class Manager {
  public:
    explicit Manager(std::float_t default_font_size = 40);
    ~Manager();

    [[nodiscard]] bool LoadFontFromDisk(const std::string& name, const std::filesystem::path& path, std::float_t font_size = 40, bool merge_fa = true);
    [[nodiscard]] bool LoadFontFromMemory(const std::string& name, void* font_data, std::int32_t font_data_size, std::float_t font_size = 40, bool merge_fa = true);
    [[nodiscard]] bool LoadFontFromMemoryCompressed(const std::string& name, const void* font_data, std::int32_t font_data_size, std::float_t font_size = 40, bool merge_fa = true);

    void PushFont(const std::string& name);
    static void PopFont();

    ImFont* GetFont(const std::string& name);

  private:
    // std::string, ImFont*
    ankerl::unordered_dense::map<std::string, void*> fonts_;
    base::common::concurrency::Spinlock lock_;

  private:
    bool FinalizeLoading(const std::string& name, const void* font, const std::float_t font_size, bool merge_fa);
  };

  inline Manager* kMANAGER{};
}
#endif //MANAGER_HPP_12174322
