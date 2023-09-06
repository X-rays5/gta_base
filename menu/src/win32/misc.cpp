//
// Created by xray on 06/09/2023.
//

#include "misc.hpp"
#include <ShlObj.h>
#include <robin_hood.h>

namespace base::win32 {
  namespace {
    const robin_hood::unordered_map<KNOWN_FOLDER_ID, KNOWNFOLDERID> guid_map {
      {KNOWN_FOLDER_ID::kDocuments, FOLDERID_Documents},
      {KNOWN_FOLDER_ID::kFonts, FOLDERID_Fonts},
      {KNOWN_FOLDER_ID::kLocalAppData, FOLDERID_LocalAppData},
      {KNOWN_FOLDER_ID::kRoamingAppData, FOLDERID_RoamingAppData},
    };
  }

  std::filesystem::path GetKnownFolderPath(KNOWN_FOLDER_ID folder_id) {
    PWSTR path = nullptr;
    const GUID guid = guid_map.at(folder_id);

    if (SUCCEEDED(SHGetKnownFolderPath(guid, 0, nullptr, &path))) {
      std::filesystem::path res = path;
      CoTaskMemFree(path);
      return res;
    } else {
      LOG_ERROR("Failed to get known folder path. win32 err code: {}", GetLastError());
      return {};
    }
  }

  std::vector<MODULEENTRY32> GetProcessModules(std::uint32_t pid) {
    std::vector<MODULEENTRY32> modules;
    MODULEENTRY32 mod_entry{};
    mod_entry.dwSize = sizeof(MODULEENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
      return modules;
    }

    if (Module32First(hSnapshot, &mod_entry)) {
      do {
        modules.push_back(mod_entry);
      } while (Module32Next(hSnapshot, &mod_entry));
    }

    CloseHandle(hSnapshot);
    return modules;
  }
}