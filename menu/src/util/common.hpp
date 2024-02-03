//
// Created by xray on 06/09/2023.
//

#ifndef GTA_BASE_COMMON_475366A25D554F19A9F0C4ADBC01FCAA_HPP
#define GTA_BASE_COMMON_475366A25D554F19A9F0C4ADBC01FCAA_HPP

#define GTA_BASE_ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

namespace base::util::common {
 /**
  * \brief Get the current timestamp
  * \return The current timestamp
  */
 std::uint64_t GetTimeStamp();

 /**
  * \brief Convert a address to a hex string
  * \param addr The address
  * \return The hex string
  */
 std::string AddrToHex(uint64_t addr);
 /**
  * \brief Convert a hex string to a address
  * \param hex The hex string
  * \return The address
  */
 StatusOr<std::uint64_t> HexCharToInt(char number);
 /**
  * \brief Convert a hex string to a address
  * \param file_path The hex string
  * \return The address
  */
 StatusOr<std::string> GetFileMd5Hash(const std::filesystem::path& file_path);
}

#endif //GTA_BASE_COMMON_475366A25D554F19A9F0C4ADBC01FCAA_HPP
