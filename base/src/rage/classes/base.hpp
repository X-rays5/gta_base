//
// Created by X-ray on 08/08/22.
//

#pragma once
#ifndef GTA_BASE_BASE_HPP
#define GTA_BASE_BASE_HPP
namespace rage {
  class datBase {
  public:
    virtual ~datBase() = default;
  };

  class pgBase
  {
  public:
    virtual ~pgBase() = default;
  private:
    void *m_pgunk;
  };
}

#endif //GTA_BASE_BASE_HPP
