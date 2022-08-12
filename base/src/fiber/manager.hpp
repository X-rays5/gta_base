//
// Created by X-ray on 08/12/22.
//

#pragma once
#ifndef GTA_BASE_FIBER_MANAGER_HPP
#define GTA_BASE_FIBER_MANAGER_HPP
#include <vector>
#include <memory>
#include <mutex>
#include "script.hpp"
#include "../scriptmanager/base_script.hpp"

namespace gta_base {
  namespace fiber {
    class Manager final: public scriptmanager::BaseScript {
    public:
      using script_t = std::unique_ptr<Script>;

    public:
      Manager();
      ~Manager() override;

      void AddScript(script_t script);
      void RemoveAllScripts();

      void Init() override;
      void RunTick() override;
      scriptmanager::ScriptType GetType() override;

    private:
      std::mutex mtx_;
      std::vector<script_t> scripts_;
    };
    inline Manager* kMANAGER{};
  }
}
#endif //GTA_BASE_FIBER_MANAGER_HPP
