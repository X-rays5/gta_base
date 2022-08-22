//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "ui.hpp"
#include "manager.hpp"
#include "notification/notification.hpp"
#include "components/option/executeoption.hpp"
#include "components/option/submenuoption.hpp"
#include "components/option/numberoption.hpp"
#include "components/option/labeloption.hpp"
#include "components/option/toggleoption.hpp"
#include "components/option/listoption.hpp"

namespace ui {
  bool ui_draw_tick = false;
  bool initialized = false;
  float test_value_f = 1;
  bool test_value_b = false;
  std::vector<std::string> test_value_v = {"test", "test2"};
  std::size_t test_value_v_idx = 0;
  void Init() {
    if (initialized) {
      return;
    }

    initialized = true;

    kMANAGER = std::make_unique<Manager>();

    kMANAGER->AddSubmenu("Home", components::Submenus::Home, [](components::Submenu* sub){
      sub->AddOption(components::option::SubmenuOption("TestScroll", "", components::Submenus::TestScroll));
      sub->AddOption(components::option::SubmenuOption("Debug", "a short description a short description just a bit too long", components::Submenus::Debug));
      sub->AddOption(components::option::SubmenuOption("Settings", "this is a really long description as you can see weep woop", components::Submenus::Settings));
    });

    kMANAGER->AddSubmenu("TestScroll", components::Submenus::TestScroll, [](components::Submenu* sub){
      for (int i = 0; i < 100; i++) {
        sub->AddOption(components::option::ExecuteOption(std::format("test {}", i), "", []{}));
      }
    });

    kMANAGER->AddSubmenu("Debug", components::Submenus::Debug, [](components::Submenu* sub){
      sub->AddOption(components::option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(components::option::ExecuteOption("Test notify", "", []{
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Test notify", "This is a test of the info notification. Seems to look pretty good");
        kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Test notify", "This is a test of the success notification. Seems to look pretty good");
        kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Test notify", "This is a test of the fail notification. Seems to look pretty good");
      }));
      sub->AddOption(components::option::ExecuteOption("Test notify 2", "", []{
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "1 line");
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2");
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line ");
      }));
      sub->AddOption(components::option::LabelOption("Component examples"));
      sub->AddOption(components::option::NumberOption("Test numberoption", "", &test_value_f, 0.5, 0, 10))->OnEvent([](components::Event event){
        if (event == components::Event::kChange)
          std::cout << test_value_f << std::endl;
      });
      sub->AddOption(components::option::ToggleOption("Test toggleoption", "", &test_value_b));
      sub->AddOption(components::option::ListOption("Test listoption", "", test_value_v_idx, test_value_v));
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
      std::thread([&]{
        kMANAGER->Draw();

        ui_draw_tick = false;
      }).detach();
    }

    kMANAGER->GetDrawList()->Draw();
  }
}