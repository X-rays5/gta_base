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

    kMANAGER->AddSubmenu("Home", Submenus::Home, [](Submenu* sub){
      sub->AddOption(option::SubmenuOption("TestScroll", "", Submenus::TestScroll));
      sub->AddOption(option::SubmenuOption("Debug", "a short description a short description just a bit too long", Submenus::Debug));
      sub->AddOption(option::SubmenuOption("Settings", "this is a really long description as you can see weep woop", Submenus::Settings));
    });

    kMANAGER->AddSubmenu("TestScroll", Submenus::TestScroll, [](Submenu* sub){
      for (int i = 0; i < 100; i++) {
        sub->AddOption(option::ExecuteOption(std::format("test {}", i), "", []{}));
      }
    });

    kMANAGER->AddSubmenu("Debug", Submenus::Debug, [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("Hello World", "", []{
        std::cout << "Hello World from the player submenu" << std::endl;
      }));
      sub->AddOption(option::ExecuteOption("Test notify", "", []{
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "Test notify", "This is a test of the info notification. Seems to look pretty good");
        kNOTIFICATIONS->Create(ui::Notification::Type::kSuccess, "Test notify", "This is a test of the success notification. Seems to look pretty good");
        kNOTIFICATIONS->Create(ui::Notification::Type::kFail, "Test notify", "This is a test of the fail notification. Seems to look pretty good");
      }));
      sub->AddOption(option::ExecuteOption("Test notify 2", "", []{
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "1 line");
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2");
        kNOTIFICATIONS->Create(ui::Notification::Type::kInfo, "", "3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line ");
      }));
      sub->AddOption(option::LabelOption("Component examples"));
      sub->AddOption(option::NumberOption("Test numberoption", "", &test_value_f, 0.5, 0, 10))->OnEvent([](Event event){
        if (event == Event::kChange)
          std::cout << test_value_f << std::endl;
      });
      sub->AddOption(option::ToggleOption("Test toggleoption", "", &test_value_b));
      sub->AddOption(option::ListOption("Test listoption", "", test_value_v_idx, test_value_v));
    });

    kMANAGER->AddSubmenu("Settings", Submenus::Settings, [](Submenu* sub){
      sub->AddOption(option::SubmenuOption("Unload", "", Submenus::UnloadConfirm));
    });

    kMANAGER->AddSubmenu("Unload", Submenus::UnloadConfirm, [](Submenu* sub){
      sub->AddOption(option::ExecuteOption("Yes", "", []{
        // TODO: unload
      }));
      sub->AddOption(option::ExecuteOption("No", "", []{
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