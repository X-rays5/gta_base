//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_AT_ARRAY_HPP
#define GTA_BASE_AT_ARRAY_HPP
#include <cstdint>
#include "sys_mem_allocator.hpp"
#include "../types.hpp"
#undef max

namespace rage {
#pragma pack(push, 8)
  template <typename T>
  class atArray
  {
  public:
    atArray() {
      m_data = nullptr;
      m_count = 0;
      m_size = 0;
    }

    atArray(const atArray& right) {
      m_count = right.m_count;
      m_size = right.m_size;

      m_data = (T*)rage::GetAllocator()->allocate(m_size * sizeof(T), 16, 0);
      std::uninitialized_copy(right.m_data, right.m_data + right.m_count, m_data);
    }

    explicit atArray(int capacity) {
      m_data = (T*)rage::GetAllocator()->allocate(capacity * sizeof(T), 16, 0);
      m_count = 0;
      m_size = capacity;
    }

    void clear()
    {
      m_count = 0;
      m_size = 0;

      if (m_data)
      {
        rage::GetAllocator()->free(m_data);

        m_data = nullptr;
      }
    }

    void append(atArray<T> value_array)
    {
      auto value_array_size = value_array.size();
      auto old_capacity = m_count;

      if ((value_array_size + m_count) > std::numeric_limits<uint16_t>::max())
        LOG_FATAL("RAGE atArray::append was given too large of an atArray to append");

      auto size = (uint16_t)value_array_size;
      expand(m_count + size);
      m_size += size;

      auto i = old_capacity;
      for (auto weapon_hash : value_array)
      {
        m_data[i] = weapon_hash;
        i++;
      }
    }

    void append(std::vector<T> value_array)
    {
      auto value_array_size = value_array.size();
      auto old_capacity = m_count;

      if ((value_array_size + m_count) > std::numeric_limits<uint16_t>::max())
        LOG_FATAL("RAGE atArray::append was given too large of a vector to append");

      auto size = (uint16_t)value_array_size;
      expand(m_count + size);
      m_size += size;

      auto i = old_capacity;
      for (auto weapon_hash : value_array)
      {
        m_data[i] = weapon_hash;
        i++;
      }
    }

    void append(const std::initializer_list<T> value_array)
    {
      auto value_array_size = value_array.size();
      auto old_capacity = m_count;

      if ((value_array_size + m_count) > std::numeric_limits<uint16_t>::max())
        LOG_FATAL("RAGE atArray::append was given too large of a list to append");

      auto size = (uint16_t)value_array_size;
      expand(m_count + size);
      m_size += size;

      auto i = old_capacity;
      for (const T* it = value_array.begin(); it != value_array.end(); ++it)
      {
        m_data[i] = *it;
        i++;
      }
    }

    void append(T value)
    {
      set(m_count, value);
    }

    void set(uint16_t offset, const T& value)
    {
      if (offset >= m_count)
      {
        expand(offset + 1);
      }

      if (offset >= m_size)
      {
        m_size = offset + 1;
      }

      m_data[offset] = value;
    }

    void expand(uint16_t newSize)
    {
      if (m_count >= newSize)
      {
        return;
      }

      T* newOffset = (T*)rage::GetAllocator()->allocate(newSize * sizeof(T), 16, 0);

      // initialize the new entries
      std::uninitialized_fill(newOffset, newOffset + newSize, T());

      // copy the existing entries
      if (m_data)
      {
        std::copy(m_data, m_data + m_size, newOffset);

        rage::GetAllocator()->free(m_data);
      }

      m_data = newOffset;
      m_count = newSize;
    }

    [[nodiscard]] T* begin() const {
      return &m_data[0];
    }

    [[nodiscard]] T* end() const {
      return &m_data[m_size];
    }

    [[nodiscard]] T* data() const {
      return m_data;
    }

    [[nodiscard]] std::uint16_t size() const {
      return m_size;
    }

    [[nodiscard]] std::uint16_t count() const {
      return m_count;
    }

    T& operator[](std::uint16_t index) const {
      return m_data[index];
    }

    friend std::ostream& operator<<(std::ostream& o, const atArray<T>& j)
    {
      o << "Array Size: " << j.size() << std::endl;
      for(int i = 0; i < j.size(); i++)
      {
        T item = j[i];
        if (std::is_pointer<T>())
          o << "\tArray Pointer: " << HEX_TO_UPPER(item) << " Item: [" << HEX_TO_UPPER(*(T*)item) << "]";
        else
          o << "\tArray Item: " << item;
        if (i != j.size() - 1)
          o << std::endl;
      }
      return o;
    }

  private:
    T* m_data;
    std::uint16_t m_size;
    std::uint16_t m_count;
  };
  static_assert(sizeof(rage::atArray<Hash>) == 0x10, "rage::atArray is not properly sized");
#pragma pack(pop)
}
#endif //GTA_BASE_AT_ARRAY_HPP