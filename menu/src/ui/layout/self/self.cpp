//
// Created by X-ray on 27/03/2026.
//

#include "self.hpp"
#include "../../../feature/feature_settings.hpp"
#include "../../../script/game_task_executor.hpp"
#include "../../components/components.hpp"
#include "../../../options/option_registry.hpp"

namespace base::menu::ui::layout {
  void InitHealthSub() {
    Submenu health_submenu("ui/sub/self_health", [](Submenu* sub) {
      if (!feature::kSETTINGS)
        return;

      const auto settings = feature::kSETTINGS->getSettings().lock();

      if (options::kOPTION_REGISTRY) {
        options::kOPTION_REGISTRY->options().lock()->god_mode_option->CreateOptionUi("ui/option/self_godmode", sub);
      }
      sub->AddComponent(components::ToggleComponent("ui/option/self_semigodmode", &settings->self.health.semi_god_mode));
      sub->AddComponent(components::ExecuteComponent("ui/action/self_heal", [] {
        script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
          natives::ENTITY::SET_ENTITY_HEALTH(natives::PLAYER::PLAYER_PED_ID(), natives::ENTITY::GET_ENTITY_MAX_HEALTH(natives::PLAYER::PLAYER_PED_ID()), natives::PLAYER::PLAYER_PED_ID(), NULL);
        });
      }));
      sub->AddComponent(components::ExecuteComponent("ui/action/suicide", [](script::GameTaskExecutor::GameTask*) {
        script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
          natives::ENTITY::SET_ENTITY_HEALTH(natives::PLAYER::PLAYER_PED_ID(), 0, natives::PLAYER::PLAYER_PED_ID(), NULL);
        });
      }));
      sub->AddComponent(components::ExecuteComponent("ui/action/self_max_armour", [](script::GameTaskExecutor::GameTask*) {
        script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
          natives::PED::SET_PED_ARMOUR(natives::PLAYER::PLAYER_PED_ID(), natives::PLAYER::GET_PLAYER_MAX_ARMOUR(natives::PLAYER::PLAYER_ID()));
        });
      }));
      sub->AddComponent(components::ExecuteComponent("ui/action/self_remove_armour", [](script::GameTaskExecutor::GameTask*) {
        script::kGAME_TASK_EXECUTOR->QueueTask([](script::GameTaskExecutor::GameTask*) {
          natives::PED::SET_PED_ARMOUR(natives::PLAYER::PLAYER_PED_ID(), 0);
        });
      }));
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSELF_HEALTH, std::move(health_submenu));
  }

  void InitSelfLayout() {
    InitHealthSub();

    Submenu self_submenu("ui/sub/self", [](Submenu* sub) {
      if (!feature::kSETTINGS)
        return;

      const auto settings = feature::kSETTINGS->getSettings().lock();

      sub->AddComponent(components::SubLinkComponent(SubmenuIDs::kSELF_HEALTH));
      sub->AddComponent(components::ToggleNumberRangeComponent<std::uint8_t>("ui/option/force_wanted_level", &settings->self.wanted_level_force, 0, 5, 1, &settings->self.force_wanted_level));
      sub->AddComponent(components::ToggleComponent("ui/option/never_wanted", &settings->self.never_wanted));
    });

    kMENU_RENDERER->AddSubmenu(SubmenuIDs::kSELF, std::move(self_submenu));
  }
}
