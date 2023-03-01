//
// Created by X-ray on 02/16/23.
//

#include "manager.hpp"

#include <utility>

namespace gta_base::key_input {
  namespace {
    FORCE_INLINE bool IsKeyboardPressed(Keyboard* keyboard, const std::vector<Keyboard::INPUT>& keyboard_input) {
      for (auto&& key : keyboard_input) {
        if (!keyboard->IsKeyDown(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsKeyboardJustReleased(Keyboard* keyboard, const std::vector<Keyboard::INPUT>& keyboard_input) {
      for (auto&& key : keyboard_input) {
        if (!keyboard->KeyJustReleased(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsControllerPressed(Controller* controller, const std::vector<Controller::INPUT>& controller_input) {
      for (auto&& key : controller_input) {
        if (!controller->IsKeyDown(key))
          return false;
      }

      return true;
    }

    FORCE_INLINE bool IsControllerJustReleased(Controller* controller, const std::vector<Controller::INPUT>& controller_input) {
      for (auto&& key : controller_input) {
        if (!controller->KeyJustReleased(key))
          return false;
      }

      return true;
    }

    template<typename T>
    std::optional<std::vector<T>> ProcessLoadArray(std::uint32_t id, rapidjson::Value& input_arr) {
      if (!input_arr.IsArray()) {
        LOG_ERROR("input_arr is not an arr");
        return std::nullopt;
      }

      std::vector<T> input;
      for (auto&& keyboard_bind : input_arr.GetArray()) {
        if (!keyboard_bind.Is<std::uint32_t>() && magic_enum::enum_contains<T>(keyboard_bind.Get<std::uint32_t>())) {
          LOG_ERROR("Bind with id {} is malformed. Skipping...", id);
          return std::nullopt;
        }
        input.push_back(static_cast<T>(keyboard_bind.Get<std::uint32_t>()));
      }

      return input;
    }
  }

  Manager::Manager() {
    kINPUT_MGR = this;
  }

  Manager::~Manager() {
    kINPUT_MGR = nullptr;
  }

  bool Manager::IsControlPressed(std::uint32_t key_bind_id) {
    if (IsInputBlocked())
      return false;

    misc::ScopedSpinlock lock(key_binds_lock_);

    auto it = key_binds_.find(key_bind_id);
    if (it != key_binds_.end()) {
      if (IsKeyboardPressed(&keyboard_, it->second.keyboard_key_bind))
        return true;
      else if (IsControllerPressed(&controller_, it->second.controller_key_bind))
        return true;
    }

    return false;
  }
  bool Manager::IsControlJustPressed(std::uint32_t key_bind_id) {
    if (IsInputBlocked())
      return false;

    misc::ScopedSpinlock lock(key_binds_lock_);

    auto it = key_binds_.find(key_bind_id);
    if (it != key_binds_.end()) {
      if (IsKeyboardJustReleased(&keyboard_, it->second.keyboard_key_bind))
        return true;
      else if (IsControllerJustReleased(&controller_, it->second.controller_key_bind))
        return true;
    }

    return false;
  }

  bool Manager::AddKeybinding(std::uint32_t key_bind_id, std::vector<Keyboard::INPUT> keyboard_bind, std::vector<Controller::INPUT> controller_bind, bool silent) {
    misc::ScopedSpinlock lock(key_binds_lock_);
    if (key_binds_.contains(key_bind_id)) {
      LOG_ERROR_CONDITIONAL(!silent, "Key bind with id {} already exists.", key_bind_id);
      return false;
    }

    key_binds_[key_bind_id] = {std::move(keyboard_bind), std::move(controller_bind)};

    return true;
  }

  void Manager::RemoveKeybinding(std::uint32_t key_bind_id) {
    key_binds_lock_.Lock();
    key_binds_.erase(key_bind_id);
    key_binds_lock_.Unlock();
  }

  void Manager::Save(Manager* manager, const std::filesystem::path& file) {
    std::filesystem::create_directories(file);

    rapidjson::Document json;
    json.SetObject();

    rapidjson::Value key_bind_arr;
    key_bind_arr.SetArray();

    manager->key_binds_lock_.Lock();
    for (auto&& key_bind : manager->key_binds_) {
      rapidjson::Value key_bind_mem;
      key_bind_mem.SetObject();

      rapidjson::Value keyboard_binds;
      keyboard_binds.SetArray();
      for (auto&& key : key_bind.second.keyboard_key_bind) {
        keyboard_binds.PushBack(static_cast<std::uint32_t>(key), json.GetAllocator());
      }

      rapidjson::Value controller_binds;
      controller_binds.SetArray();
      for (auto&& key : key_bind.second.controller_key_bind) {
        controller_binds.PushBack(static_cast<std::uint32_t>(key), json.GetAllocator());
      }

      key_bind_mem.AddMember("keyboard", keyboard_binds, json.GetAllocator());
      key_bind_mem.AddMember("controller", controller_binds, json.GetAllocator());

      key_bind_arr.PushBack(key_bind_mem, json.GetAllocator());
    }
    manager->key_binds_lock_.Unlock();

    json.AddMember("key_binds", key_bind_arr, json.GetAllocator());

    json::ToFile(json, file);
  }

  void Manager::Load(Manager* manager, const std::filesystem::path& file) {
    if (!std::filesystem::exists(file)) {
      LOG_ERROR("Loading key_binds failed: {} doesn't exist", file.string());
      return;
    }

    auto json = json::FromFile(file);
    if (!json.IsObject()) {
      LOG_ERROR("Loading key_binds failed: json is not an object");
      return;
    }

    if (!json.HasMember("key_binds") && json["key_binds"].IsArray()) {
      LOG_ERROR("Loading key_binds failed: key_binds member missing or not an array");
      return;
    }

    auto key_binds = json["key_binds"].GetArray();
    for (std::size_t i = 0; i < key_binds.Size(); i++) {
      if (!key_binds[i].IsObject()) {
        LOG_ERROR("key_bind at idx {} is not an object", i);
        continue;
      }
      auto bind = key_binds[i].GetObj();
      if (!bind.HasMember("id") && bind["id"].Is<std::uint32_t>()) {
        LOG_ERROR("key_bind at idx {} is missing id member", i);
        continue;
      }

      if ((!bind.HasMember("keyboard") || bind["keyboard"].IsArray()) || (!bind.HasMember("controller") || bind["controller"].IsArray())) {
        LOG_ERROR("key_bind at idx {} is missing keyboard or controller member");
        continue;
      }

      auto bind_id = bind["id"].Get<std::uint32_t>();

      auto keyboard_vec = ProcessLoadArray<Keyboard::INPUT>(bind_id, bind["keyboard"]);
      if (!keyboard_vec.has_value())
        continue;
      auto controller_vec = ProcessLoadArray<Controller::INPUT>(bind_id, bind["controller"]);
      if (!controller_vec.has_value())
        continue;

      if (!manager->AddKeybinding(bind_id, *keyboard_vec, *controller_vec)) {
        LOG_ERROR("Bind with id {} already exists replacing");
        manager->RemoveKeybinding(bind_id);
        manager->AddKeybinding(bind_id, *keyboard_vec, *controller_vec);
      }
    }
  }
}
