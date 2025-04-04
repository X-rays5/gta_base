//
// Created by X-ray on 11/02/2025.
//

#ifndef CONVERSION_HPP_02232556
#define CONVERSION_HPP_02232556
#include <cstdint>
#include <string>
#include "result.hpp"

namespace base::common::conversion {
  /**
 * \brief Convert an address to a hex string
 * \param addr The address
 * \return The hex string
 */
  std::string AddrToHex(uint64_t addr);
  /**
   * \brief Convert a hex string to an address
   * \param number The hex string
   * \return The address
   */
  StatusOr<std::uint64_t> HexCharToInt(char number);
}
#endif //CONVERSION_HPP_02232556
