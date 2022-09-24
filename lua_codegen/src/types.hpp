//
// Created by X-ray on 09/24/22.
//

#pragma once
#ifndef GTA_BASE_TYPES_HPP
#define GTA_BASE_TYPES_HPP
#include <unordered_map>

namespace lua_codegen::types {
    static std::unordered_map<const char*, const char*> types = {
      {"Void", "Void"},
      {"Any", "Int"},
      {"Hash", "Int"},
      {"Entity", "Int"},
      {"Player", "Int"},
      {"FireId", "Int"},
      {"Interior", "Int"},
      {"Ped", "Int"},
      {"Vehicle", "Int"},
      {"Cam", "Int"},
      {"Object", "Int"},
      {"Pickup", "Int"},
      {"Blip", "Int"},
      {"Camera", "Int"},
      {"ScrHandle", "Int"},
      {"Vector3", "Vector3"},
      {"int", "Int"},
    };

    bool ValidType(const char* name) {
      return types.find(name) != types.end();
    }

    const char* GetInvokerType(const char* name) {
      if (ValidType(name))
        return types[name];

      return nullptr;
    }
  }
#endif //GTA_BASE_TYPES_HPP
