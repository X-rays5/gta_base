//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "components/option/executeoption.hpp"
#include "components/option/submenuoption.hpp"

namespace ui {
  bool ui_draw_tick = false;
  bool initialized = false;
  void Init() {
    if (initialized) {
      return;
    }

    initialized = true;

    kMANAGER = std::make_unique<Manager>();

    kMANAGER->AddSubmenu("Home", components::Submenus::Home, [](components::Submenu* sub){
      sub->AddOption(components::option::SubmenuOption("Player", "a short description a short description just a bit too long", components::Submenus::Player));
      sub->AddOption(components::option::SubmenuOption("Settings", "this is a really long description as you can see weep woop oh no anyways this is a really long description as you can see weep woop oh no anyways this is a really long description as you can see weep woop oh no anyways this is a really long description as you can see weep woop oh no anyways", components::Submenus::Settings));
    });

    kMANAGER->AddSubmenu("Player", components::Submenus::Player, [](components::Submenu* sub){
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
    });

    kMANAGER->AddSubmenu("Settings", components::Submenus::Settings, [](components::Submenu* sub){
      sub->AddOption(components::option::SubmenuOption("Unload", "", components::Submenus::UnloadConfirm));
    });

    kMANAGER->AddSubmenu("Unload", components::Submenus::UnloadConfirm, [](components::Submenu* sub){
      sub->AddOption(components::option::ExecuteOption("Yes", "", []{
        // TODO: unload
      }));
      sub->AddOption(components::option::ExecuteOption("No", "", []{
        kMANAGER->PopSubmenu();
      }));
    });
  }

  void Tick() {
    if (!initialized)
      Init();

    // TODO: this should be using a thread pool since this is slow
    if (!ui_draw_tick) {
      ui_draw_tick = true;
      std::cout << "draw tick" << std::endl;

      std::thread([&]{
        kMANAGER->Draw();

        ui_draw_tick = false;
      }).detach();
    }

    kMANAGER->GetDrawList()->Draw();
  }
}