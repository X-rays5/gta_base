//
// Created by X-ray on 05/09/2023.
//

#pragma once
#ifndef GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
#define GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP

namespace base::logging {
  class Manager {
  public:
    Manager();
    ~Manager();

    static void Shutdown();

  private:
    static void Init();
  };
}

#endif //GTA_BASE_LOGGER_B10F4F65142242508AED522F616DA0DA_HPP
