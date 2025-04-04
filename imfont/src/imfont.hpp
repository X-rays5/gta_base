//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12174322
#define MANAGER_HPP_12174322
#include <unordered_map>
#include <filesystem>

namespace imfont {
  class Manager {
  public:
    Manager();
    ~Manager();

    [[nodiscard]] bool LoadFontFromDisk(const std::string& name, const std::filesystem::path& path, bool merge_fa = true);
    [[nodiscard]] bool LoadFontFromMemory(const std::string& name, void* font_data, std::int32_t font_data_size, bool merge_fa = true);
    [[nodiscard]] bool LoadFontFromMemoryCompressed(const std::string& name, const void* font_data, std::int32_t font_data_size, bool merge_fa = true);

    void PushFont(const std::string& name);
    static void PopFont();

  private:
    // std::string, ImFont*
    std::unordered_map<std::string, void*> fonts_;

  private:
    bool FinalizeLoading(const std::string& name, const void* font, bool merge_fa);
  };

  inline Manager* kMANAGER{};
}
#endif //MANAGER_HPP_12174322
