//
// Created by X-ray on 11/12/22.
//

#pragma once
#ifndef GTA_BASE_DATBITBUFFER_HPP
#define GTA_BASE_DATBITBUFFER_HPP
#include <cstdint>
#include "../../memory/pointers.hpp"

namespace rage {
  class datBitBuffer
  {
  public:
    datBitBuffer(uint8_t* data, uint32_t size) {
      m_data = data;
      m_bitOffset = 0;
      m_maxBit = size * 8;
      m_bitsRead = 0;
      m_curBit = 0;
      m_highestBitsRead = 0;
      m_flagBits = 0;
    }
    uint32_t GetPosition() {
      return m_bitsRead;
    }
    bool Seek(uint32_t bits) {
      if (bits >= 0) {
        uint32_t length = (m_flagBits & 1) ? m_maxBit : m_curBit;
        if (bits <= length)
          m_bitsRead = bits;
      }
      return false;
    }
    bool WriteBool(bool integer) {
      return gta_base::memory::kPOINTERS->WriteBitBufBool(this, integer, 1);
    }
    bool ReadBool(bool* integer) {
      return gta_base::memory::kPOINTERS->ReadBitBufBool(this, integer, 1);
    }
    bool ReadPeerId(uint64_t* integer) {
      return this->ReadQWord(integer, 0x32);
    }
    uint64_t ReadBits(size_t numBits) {
      auto const totalBits = (m_flagBits & 1) ? m_maxBit : m_curBit;
      if ((m_flagBits & 2) || m_bitsRead + numBits > totalBits)
        return 0;
      auto const bufPos = m_bitsRead + m_bitOffset;
      auto const initialBitOffset = bufPos & 0b111;
      auto const start = &m_data[bufPos / 8];
      auto const next = &start[1];
      auto result = (start[0] << initialBitOffset) & 0xff;
      for (auto i = 0; i < ((numBits - 1) / 8); i++) {
        result <<= 8;
        result |= next[i] << initialBitOffset;
      }
      if (initialBitOffset)
        result |= next[0] >> (8 - initialBitOffset);
      m_bitsRead += static_cast<uint32_t>(numBits);
      if (m_bitsRead > m_highestBitsRead)
        m_highestBitsRead = m_bitsRead;
      return result >> ((8 - numBits) % 8);
    }
    int GetDataLength() {
      int leftoverBit = (m_curBit % 8) ? 1 : 0;
      return (m_curBit / 8) + leftoverBit;
    }
    bool ReadString(char* string, int bits) {
      return gta_base::memory::kPOINTERS->ReadBitBufString(this, string, bits);
    }
    bool WriteByte(uint8_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufDword(this, integer, bits);
    }
    bool ReadByte(uint8_t* integer, int bits) {
      uint32_t read;
      if (gta_base::memory::kPOINTERS->ReadBitBufDword(this, &read, bits)) {
        *integer = read;
        return true;
      }
      return false;
    }
    bool WriteWord(uint16_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufDword(this, integer, bits);
    }
    bool ReadWord(uint16_t* integer, int bits) {
      uint32_t read;
      if (gta_base::memory::kPOINTERS->ReadBitBufDword(this, &read, bits)) {
        *integer = read;
        return true;
      }
      return false;
    }
    bool WriteDword(uint32_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufDword(this, integer, bits);
    }
    bool ReadDword(uint32_t* integer, int bits) {
      return gta_base::memory::kPOINTERS->ReadBitBufDword(this, integer, bits);
    }
    bool WriteInt32(int32_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufInt32(this, integer, bits);
    }
    bool ReadInt32(int32_t* integer, int bits) {
      int32_t v8;
      int32_t v9;
      if (ReadDword((uint32_t*)&v8, 1u) && ReadDword((uint32_t*)&v9, bits - 1)) {
        *integer = v8 + (v9 ^ -v8);
        return true;
      }
      return false;
    }
    bool WriteQWord(uint64_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufQword(this, integer, bits);
    }
    bool ReadQWord(uint64_t* integer, int bits) {
      if (bits <= 32) {
        uint32_t v10;
        if (ReadDword(&v10, bits)) {
          *integer = v10;
          return true;
        }
      }
      else {
        uint32_t v10, v11;
        if (ReadDword(&v11, 32u) && ReadDword(&v10, bits - 32u)) {
          *integer = v11 | ((uint64_t)v10 << 32);
          return true;
        }
      }
      return false;
    }
    bool WriteInt64(int64_t integer, int bits) {
      return gta_base::memory::kPOINTERS->WriteBitBufInt64(this, integer, bits);
    }
    bool ReadInt64(int64_t* integer, int bits) {
      uint32_t v8;
      uint64_t v9;
      if (ReadDword(&v8, 1u) && ReadQWord(&v9, bits - 1)) {
        *integer = v8 + (v9 ^ -(int64_t)v8);
        return true;
      }
      return false;
    }
    bool WriteArray(uint8_t* array, int size) {
      return gta_base::memory::kPOINTERS->WriteBitBufArray(this, array, size, 0);
    }
    bool ReadArray(PVOID array, int size) {
      return gta_base::memory::kPOINTERS->ReadBitBufArray(this, array, size, 0);
    }
  public:
    uint8_t* m_data; //0x0000
    uint32_t m_bitOffset; //0x0008
    uint32_t m_maxBit; //0x000C
    uint32_t m_bitsRead; //0x0010
    uint32_t m_curBit; //0x0014
    uint32_t m_highestBitsRead; //0x0018
    uint8_t m_flagBits; //0x001C
  };
}
#endif //GTA_BASE_DATBITBUFFER_HPP
