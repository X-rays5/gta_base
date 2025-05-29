//
// Created by X-ray on 11/02/2025.
//

#include "hex.hpp"

namespace base::common::conversion {
  std::string AddrToHex(const uint64_t addr) {
    if (!addr)
      return "0x0";

    std::ostringstream stream;
    stream << "0x" << std::hex << std::uppercase << addr;
    return stream.str();
  }

  struct HexToIntTable {
    std::array<std::int64_t, 128> tab;

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

    std::int64_t operator[](char const idx) const {
      return tab[idx];
    }
  } hex_to_int_table;

  StatusOr<std::uint64_t> HexCharToInt(const char number) {
    if (number == '0')
      return 0;

    auto res = hex_to_int_table[number];
    if (res == 0)
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Invalid hex character.");

    return res;
  }
}
