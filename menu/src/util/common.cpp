//
// Created by xray on 06/09/2023.
//

#include "common.hpp"
#include <fstream>
#include <wincrypt.h>

namespace base::util::common {
  StatusOr<std::string> GetFileMd5Hash(const std::filesystem::path& file_path) {
    HCRYPTPROV hProv = 0;
    HCRYPTHASH hHash = 0;
    BYTE rgbHash[16];
    auto cbHash = static_cast<DWORD>(std::size(rgbHash));
    constexpr CHAR rgbDigits[] = "0123456789abcdef";
    std::string res;

    if (!CryptAcquireContextA(&hProv, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
      return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("CryptAcquireContextA failed: {}", GetLastError()));
    }

    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
      CryptReleaseContext(hProv, 0);
      return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("CryptCreateHash failed: {}", GetLastError()));
    }

    std::ifstream file(file_path);

    if (!file.is_open())
      return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("Failed to open file: {}", file_path.string()));

    std::vector<char> buffer(1024);
    while (file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()))) {
      if (!CryptHashData(hHash, reinterpret_cast<BYTE*>(buffer.data()), static_cast<DWORD>(buffer.size()), 0)) {
        CryptReleaseContext(hProv, 0);
        CryptDestroyHash(hHash);
        return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("CryptHashData failed: {}", GetLastError()));
      }
    }

    if (!CryptHashData(hHash, reinterpret_cast<BYTE*>(buffer.data()), static_cast<DWORD>(file.gcount()), 0)) {
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("CryptHashData failed: {}", GetLastError()));
    }

    if (!CryptGetHashParam(hHash, HP_HASHVAL, rgbHash, &cbHash, 0)) {
      CryptReleaseContext(hProv, 0);
      CryptDestroyHash(hHash);
      return MakeFailure<ResultCode::kINTERNAL_ERROR>(fmt::format("CryptGetHashParam failed: {}", GetLastError()));
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
