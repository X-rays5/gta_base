//
// Created by X-ray on 18/12/2025.
//

#pragma once

#include <windows.h>

namespace base::common::conversion {
  class CW2A {
  public:
    explicit CW2A(const wchar_t* src, const UINT codePage = CP_ACP)
      : m_buf(nullptr) {
      if (!src)
        return;

      const int len = WideCharToMultiByte(
        codePage,
        0,
        src,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
      );

      if (len <= 0)
        return;

      m_buf = new char[len];

      WideCharToMultiByte(
        codePage,
        0,
        src,
        -1,
        m_buf,
        len,
        nullptr,
        nullptr
      );
    }

    ~CW2A() {
      delete[] m_buf;
    }

    operator const char*() const {
      return m_buf;
    }

    const char* c_str() const {
      return m_buf;
    }

  private:
    char* m_buf;
  };
}
