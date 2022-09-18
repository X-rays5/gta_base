//
// Created by X-ray on 09/18/22.
//

#include "debug.hpp"
#include "tab_includes.hpp"

namespace gta_base {
  namespace ui {
    namespace tabs {
      namespace {
        float test_value_f = 1;
        float test_value_f_toggle_number = 1;
        bool test_value_b_toggle_number = false;
        bool test_value_b = false;
        std::vector<std::string> test_value_v = {"test", "test2"};
        std::size_t test_value_v_idx = 0;
        bool test_value_b_toggle_list;
        std::size_t test_value_v_idx_toggle_list;
      }

      void DebugTab() {
        kMANAGER->AddSubmenu(Submenus::Debug, "Debug", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("Test Components", "", Submenus::DebugTestComponents));
          sub->AddOption(option::ExecuteOption("Test notify", "", []{
            kNOTIFICATIONS->Create(Notification::Type::kInfo, "Test notify", "This is a test of the info notification. Seems to look pretty good");
            kNOTIFICATIONS->Create(Notification::Type::kSuccess, "Test notify", "This is a test of the success notification. Seems to look pretty good");
            kNOTIFICATIONS->Create(Notification::Type::kFail, "Test notify", "This is a test of the fail notification. Seems to look pretty good");
          }));
          sub->AddOption(option::ExecuteOption("Test notify 2", "", []{
            kNOTIFICATIONS->Create(Notification::Type::kInfo, "", "1 line");
            kNOTIFICATIONS->Create(Notification::Type::kInfo, "", "2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2 line 2");
            kNOTIFICATIONS->Create(Notification::Type::kInfo, "", "3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line 3 line ");
          }));
        });

        kMANAGER->AddSubmenu(Submenus::DebugTestComponents, "Test Components", [](Submenu* sub){
          sub->AddOption(option::SubmenuOption("Test SubmenuOption", "", Submenus::Home));
          sub->AddOption(option::LabelOption("Test LabelOption"));
          sub->AddOption(option::ExecuteOption("Test ExecuteOption", "", []{
            LOG_DEBUG("Test ExecuteOption");
          }));
          sub->AddOption(option::ToggleOption("Test ToggleOption", "", &test_value_b));
          sub->AddOption(option::NumberOption<float>("Test NumberOption", "", &test_value_f, 0.5, 0, 10))->OnEvent([](Event event){
            if (event == Event::kChange)
              LOG_DEBUG("Test NumberOption {}", test_value_f);
          });
          sub->AddOption(option::ToggleNumberOption<float>("Test ToggleNumberOption", "", test_value_b_toggle_number, test_value_f_toggle_number, 0.5, 0, 10))->OnEvent([](Event event){
            if (event == Event::kChange)
              LOG_DEBUG("Test ToggleNumberOption {} {}", test_value_b_toggle_number, test_value_f_toggle_number);
          });
          sub->AddOption(option::ListOption("Test ListOption", "", test_value_v_idx, test_value_v));
          sub->AddOption(option::ToggleListOption("Test ToggleListOption", "", test_value_b_toggle_list, test_value_v_idx_toggle_list, test_value_v));
        });
      }
    }
  }
}