//
// Created by xray on 06/09/2023.
//

#include "common.hpp"
#include <fstream>
#include <wincrypt.h>

namespace base::util::common {
  std::uint64_t GetTimeStamp() { return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); }

  std::string AddrToHex(uint64_t addr) {
    if (!addr)
      return "0x0";

    std::ostringstream stream;
    stream << "0x" << std::hex << std::uppercase << addr;
    return stream.str();
  }

  struct HexToIntTable {
    std::int64_t tab[128];

    HexToIntTable()
      :
      tab{} {
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

    std::int64_t operator[](char const idx) const { return tab[idx]; }
  } hex_to_int_table;

  absl::StatusOr<std::uint64_t> HexCharToInt(const char number) {
    if (number == '0')
      return 0;

    auto res = hex_to_int_table[number];
    if (res == 0)
      return absl::InvalidArgumentError("Invalid hex character.");

    return res;
  }

  absl::StatusOr<std::string> GetFileMd5Hash(const std::filesystem::path& file_path) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbHash[16];
    DWORD cbHash = static_cast<DWORD>(std::size(rgbHash));
    constexpr CHAR rgbDigits[] = "0123456789abcdef";
    std::string res;

    if (!CryptAcquireContextA(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) { return absl::InternalError(fmt::format("CryptAcquireContextA failed: {}", GetLastError())); }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
      CryptReleaseContext(hProv, 0);
      return absl::InternalError(fmt::format("CryptCreateHash failed: {}", GetLastError()));
    }

    std::ifstream file(file_path);

    if (!file.is_open())
      return absl::InternalError(fmt::format("Failed to open file: {}", file_path.string()));

    std::vector<char> buffer(1024);
    while (file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
      if (!CryptHashData(hHash, reinterpret_cast<BYTE*>(buffer.data()), static_cast<DWORD>(buffer.size()), 0)) {
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        return absl::InternalError(fmt::format("CryptHashData failed: {}", GetLastError()));
      }
    }

    if (!CryptHashData(hHash, reinterpret_cast<BYTE*>(buffer.data()), static_cast<DWORD>(file.gcount()), 0)) {
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return absl::InternalError(fmt::format("CryptHashData failed: {}", GetLastError()));
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return absl::InternalError(fmt::format("CryptGetHashParam failed: {}", GetLastError()));
    }

    for (DWORD i = 0; i < cbHash; i++) {
      res.push_back(rgbDigits[rgbHash[i] >> 4]);
      res.push_back(rgbDigits[rgbHash[i] & 0xf]);
    }

    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);

    return res;
  }
}
