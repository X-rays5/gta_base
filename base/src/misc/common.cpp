//
// Created by X-ray on 08/11/22.
//

#include "common.hpp"
#include "../memory/pointers.hpp"
#include "../d3d/renderer.hpp"
#include <robin_hood.h>
#include <network/CNetworkPlayerMgr.hpp>
#include <TlHelp32.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <wincrypt.h>

namespace gta_base::common {
  Platform GetCurrentPlatform() {
    static const Platform cur_platform = GetModuleHandleA("steam_api64.dll") ? Platform::kSteam : GetModuleHandleA("EOSSDK-Win64-Shipping.dll") ? Platform::kEpicGames : Platform::kRockstar;

    return cur_platform;
  }

  bool IsSessionStarted() {
    return (*memory::kPOINTERS->network_player_mgr_)->m_local_net_player != nullptr;
  }

  std::string AddrToHex(uint64_t addr) {
    if (!addr)
      return "0x0";

    std::ostringstream stream;
    stream << "0x" << std::hex << std::uppercase << addr;
    return stream.str();
  }

  std::vector<std::string> SplitStr(const std::string& str, const std::string& delim) {
    std::vector<std::string> res;
    size_t start = 0;
    size_t end = str.find(delim);

    while (end != std::string::npos) {
      res.push_back(str.substr(start, end - start));
      start = end + delim.length();
      end = str.find(delim, start);
    }

    res.push_back(str.substr(start, end));

    return res;
  }

  struct HexToIntTable {
    std::int64_t tab[128];
    HexToIntTable() : tab {} {
      tab['1'] = 1;
      tab['2'] = 2;
      tab['3'] = 3;
      tab['4'] = 4;
      tab['5'] = 5;
      tab['6'] = 6;
      tab['7'] = 7;
      tab['8'] = 8;
      tab['9'] = 9;
      tab['a'] = 10;
      tab['A'] = 10;
      tab['b'] = 11;
      tab['B'] = 11;
      tab['c'] = 12;
      tab['C'] = 12;
      tab['d'] = 13;
      tab['D'] = 13;
      tab['e'] = 14;
      tab['E'] = 14;
      tab['f'] = 15;
      tab['F'] = 15;
    }
    inline std::int64_t operator[](char const idx) const { return tab[(std::size_t) idx]; }
  } hex_to_int_table;

  std::int64_t HexToIntFast(char number) {
    return hex_to_int_table[(std::size_t)number];
  }

  std::string GetFileMd5Hash(const std::filesystem::path& file_path) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbHash[16];
    DWORD cbHash = GTA_BASE_ARRAY_SIZE(rgbHash);
    CHAR rgbDigits[] = "0123456789abcdef";
    std::string res;

    if (!CryptAcquireContextA(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
      LOG_ERROR("CryptAcquireContextA failed: {}", GetLastError());
      return res;
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
      LOG_ERROR("CryptCreateHash failed: {}", GetLastError());
      CryptReleaseContext(hProv, 0);
      return res;
    }

    std::ifstream file(file_path);

    if (!file.is_open())
      return res;

    std::vector<char> buffer(1024);
    while (file.read(buffer.data(), buffer.size())) {
      if (!CryptHashData(hHash, (BYTE*)buffer.data(), buffer.size(), 0)) {
        LOG_ERROR("CryptHashData failed: {}", GetLastError());
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        return res;
      }
    }

    if (!CryptHashData(hHash, (BYTE*)buffer.data(), file.gcount(), 0)) {
      LOG_ERROR("CryptHashData failed: {}", GetLastError());
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return res;
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
      LOG_ERROR("CryptGetHashParam failed: {}", GetLastError());
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return res;
    }

    for (DWORD i = 0; i < cbHash; i++) {
      res.push_back(rgbDigits[rgbHash[i] >> 4]);
      res.push_back(rgbDigits[rgbHash[i] & 0xf]);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return res;
  }

  std::string RemoveForbiddenFilenameChars(const std::filesystem::path& path) {
    robin_hood::unordered_set<char> forbidden_chars = { '\\', '/', ':', '*', '?', '"', '<', '>', '|' };

    std::string res = path.string();
    for (std::size_t i = 0; i < res.size(); i++) {
      if (forbidden_chars.contains(res[i])) {
        res.erase(i, 1);
        res.insert(i, "##");
      }
    }

    return res;
  }

  std::filesystem::path PidToPath(std::size_t pid) {
    std::filesystem::path res;

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
      return res;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
      CloseHandle(hProcessSnap);
      return res;
    }

    do {
      if (pe32.th32ProcessID == pid) {
        res = pe32.szExeFile;
        break;
      }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    return res;
  }

  std::string StripVarName(const std::string& str) {
    std::string res = str;

    // remove c_str() from the end
    if (res.find("c_str()") != std::string::npos)
      res = res.substr(0, res.find("c_str()"));

    // remove ptr access
    if (res.find("->") != std::string::npos)
      res = res.substr(res.find("->") + 2);

    // remove array access
    if (res.find('[') != std::string::npos)
      res = res.substr(0, res.find('['));

    // remove struct access
    if (res.find('.') != std::string::npos)
      res = res.substr(res.find('.') + 1);

    // remove namespace
    if (res.find("::") != std::string::npos)
      res = res.substr(res.find("::") + 2);

    return res;
  }

  std::string RemoveNonNumerical(std::string str) {
    static const robin_hood::unordered_set<char> numbers = {'0','1','2','3','4','5','6','7','8','9'};

    std::string result;
    for (char& c : str) {
      if (numbers.contains(c)) {
        result += c;
      }
    }

    return result;
  }

  std::string VkToStr(std::uint64_t vk) {
    std::uint32_t scanCode = MapVirtualKeyA(vk, MAPVK_VK_TO_VSC);

    char szName[128];
    std::int32_t result{};
    switch (vk) {
      case VK_LEFT: case VK_UP: case VK_RIGHT: case VK_DOWN:
      case VK_RCONTROL: case VK_RMENU:
      case VK_LWIN: case VK_RWIN: case VK_APPS:
      case VK_PRIOR: case VK_NEXT:
      case VK_END: case VK_HOME:
      case VK_INSERT: case VK_DELETE:
      case VK_DIVIDE:
      case VK_NUMLOCK:
        scanCode |= KF_EXTENDED;
      default:
        result = GetKeyNameTextA(scanCode << 16, szName, 128);
    }

    if(result == 0) {
      LOG_ERROR("Failed to convert vk key: {} to char. win32 err code: {}", vk, GetLastError());
      return {};
    }

    return szName;
  }

  std::string FloatPrecision(float num, std::size_t precision) {
    return fmt::format("{:.{}f}", num, precision);
  }

  std::filesystem::path GetKnownFolderPath(const KNOWNFOLDERID& folder_id) {
    PWSTR path = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(folder_id, 0, nullptr, &path))) {
      std::filesystem::path res = path;
      CoTaskMemFree(path);
      return res;
    } else {
      LOG_ERROR("Failed to get known folder path. win32 err code: {}", GetLastError());
      return {};
    }
  }

  std::filesystem::path GetBaseDir() {
    static const std::filesystem::path path = GetKnownFolderPath(FOLDERID_RoamingAppData) / globals::name;
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetTranslationDir() {
    static const std::filesystem::path path = GetBaseDir() / "translations";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetLogDir() {
    static const std::filesystem::path path = GetBaseDir() / "logs";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetLogFile() {
    return GetLogDir() / fmt::format("{}.log", globals::name);
  }

  std::filesystem::path GetLogSaveDir() {
    static const std::filesystem::path path = GetLogDir() / "save";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetDataDir() {
    static const std::filesystem::path path = GetBaseDir() / "data";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetCachedDir() {
    static const std::filesystem::path path = GetBaseDir() / "cached";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetGtaDataCacheDir() {
    static const std::filesystem::path path = GetCachedDir() / "gta_data";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetCachedPatternsDir() {
    static const std::filesystem::path path = GetCachedDir() / "patterns";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetSettingsDir() {
    static const std::filesystem::path path = GetBaseDir() / "settings";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetThemesDir() {
    static const std::filesystem::path path = GetSettingsDir() / "themes";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetHotkeysDir() {
    static const std::filesystem::path path = GetSettingsDir() / "hotkeys";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetOptionsStateDir() {
    static const std::filesystem::path path = GetSettingsDir() / "options_state";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetTeleportsDir() {
    static const std::filesystem::path path = GetDataDir() / "teleports";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetTextureDir() {
    static const std::filesystem::path path = GetDataDir() / "textures";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetScriptsDir() {
    static const std::filesystem::path path = GetBaseDir() / "scripts";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::filesystem::path GetScriptsDataDir() {
    static const std::filesystem::path path = GetDataDir() / "scripts";
    if (!std::filesystem::exists(path)) {
      std::filesystem::create_directories(path);
    }
    return path;
  }

  std::uint64_t GetEpoch() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  }

  HWND GetHwnd(const char* class_name, const char* window_name) {
    return FindWindowA(!strcmp(class_name, "") ? nullptr : class_name, !strcmp(window_name, "") ? nullptr : window_name);
  }

  HWND GetGameHwnd() {
    static const auto hwnd = GetHwnd(globals::target_window_class, globals::target_window_name);
    return hwnd;
  }

  bool IsForegroundWindow() {
    auto foreground_window = GetForegroundWindow();
    return foreground_window == GetConsoleWindow() || GetGameHwnd();
  }

  bool IsTargetProcess() {
    auto exe_name = new CHAR[MAX_PATH];
    std::uint32_t str_len = GetModuleFileNameA(nullptr, exe_name, MAX_PATH);

    if (str_len == 0) {
      LOG_ERROR("Failed to get module file name. win32 err code: {}", GetLastError());
      return false;
    }

    bool is_target_process = std::filesystem::path(std::string(exe_name, str_len)).filename().string() == globals::target_process_name;
    delete[] exe_name;
    return is_target_process;
  }

  MODULEENTRY32 GetModuleFromHModule(HMODULE mod) {
    MODULEENTRY32 mod_entry{};
    mod_entry.dwSize = sizeof(MODULEENTRY32);
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnapshot == INVALID_HANDLE_VALUE) {
      return mod_entry;
    }
    if (Module32First(hSnapshot, &mod_entry)) {
      do {
        if (mod_entry.hModule == mod) {
          break;
        }
      } while (Module32Next(hSnapshot, &mod_entry));
    }
    CloseHandle(hSnapshot);
    return mod_entry;
  }

  std::uint64_t GetModuleBaseAddress(std::uint32_t pid, const std::string& mod_name) {
    std::uint64_t mod_base_addr = 0;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap != INVALID_HANDLE_VALUE) {
      MODULEENTRY32 mod_entry;
      mod_entry.dwSize = sizeof(mod_entry);
      if (Module32First(h_snap, &mod_entry)) {
        do {
          if (!_stricmp(mod_entry.szModule, mod_name.c_str())) {
            mod_base_addr = (std::uint64_t)mod_entry.modBaseAddr;
            break;
          }
        } while (Module32Next(h_snap, &mod_entry));
      }
    }
    CloseHandle(h_snap);
    return mod_base_addr;
  }

  MODULEENTRY32 GetModuleFromAddress(std::uint32_t pid, std::uint64_t addr) {
    MODULEENTRY32 mod_entry;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
    if (h_snap != INVALID_HANDLE_VALUE) {
      mod_entry.dwSize = sizeof(mod_entry);
      if (Module32First(h_snap, &mod_entry)) {
        do {
          if ((std::uint64_t)mod_entry.modBaseAddr <= addr && addr <= (std::uint64_t)mod_entry.modBaseAddr + mod_entry.modBaseSize) {
            break;
          }
        } while (Module32Next(h_snap, &mod_entry));
      }
    }
    CloseHandle(h_snap);
    return mod_entry;
  }

  static robin_hood::unordered_map<std::uint32_t, KeyState> key_state{};
  bool IsKeyDown(std::uint32_t key) {
    auto key_entry = key_state.find(key);

    if (key_entry != key_state.end()) {
      return key_entry->second.down;
    }

    return false;
  }

  bool IsKeyJustReleased(std::uint32_t key, std::uint64_t since_up) {
    auto key_entry = key_state.find(key);

    if (key_entry != key_state.end()) {
      if (!key_entry->second.down) {
        return (GetEpoch() - key_entry->second.released_at) <= since_up;
      }
    }

    return false;
  }

  void SetKeyState(std::uint32_t key, bool down) {
    auto key_entry = key_state.find(key);
    if (key_entry != key_state.end()) {
      key_entry->second.down = down;
      if (!down)
        key_entry->second.released_at = GetEpoch();
    } else {
      key_state[key] = {down, 0};
    }
  }

  // https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples#example-for-directx11-users
  bool LoadImage(const std::filesystem::path& file, ImageTexture* out_texture) {
    auto path = GetTextureDir() / file;

#ifdef NDEBUG
    LOG_INFO("Loading image: {}", file.string());
#else
    LOG_DEBUG("Loading image: {}", path.string());
#endif

    // Load from disk into a raw RGBA buffer
    int image_width = 0;
    int image_height = 0;
    int image_comp = 0;
    auto img_file = fopen(path.string().c_str(), "rb");
    if (!img_file) {
      LOG_ERROR("Failed to open image file: {}", path.string());
      return false;
    }

    unsigned char* image_data = stbi_load_from_file(img_file, &image_width, &image_height, &image_comp, 0);
    fclose(img_file);
    if (image_data == nullptr)
      return false;

    // Create texture
    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = image_width;
    desc.Height = image_height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;

    ID3D11Texture2D* pTexture = nullptr;
    D3D11_SUBRESOURCE_DATA subResource;
    subResource.pSysMem = image_data;
    subResource.SysMemPitch = desc.Width * image_comp;
    d3d::kRENDERER->GetDevice()->CreateTexture2D(&desc, &subResource, &pTexture);

    // Create texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    ZeroMemory(&srvDesc, sizeof(srvDesc));
    srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;
    srvDesc.Texture2D.MostDetailedMip = 0;
    d3d::kRENDERER->GetDevice()->CreateShaderResourceView(pTexture, &srvDesc, &out_texture->texture);
    pTexture->Release();

    out_texture->width = image_width;
    out_texture->height = image_height;
    stbi_image_free(image_data);

    return true;
  }
}