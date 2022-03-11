//
// Created by X-ray on 3/9/2022.
//

#include <iostream>
#include "manager.hpp"
#include "components/option/executeoption.hpp"

namespace ui {
  bool initialized = false;
  void Init() {
    if (initialized) {
      return;
    }

    initialized = true;

    kMANAGER = std::make_unique<Manager>();

    kMANAGER->AddSubmenu("Home", "", components::Submenus::Home, [](components::Submenu* sub){
      sub->AddOption<components::option::ExecuteOption>("Hello World!", "This a a option", []{
        std::cout << "Hello World! from option press" << std::endl;
      });
    });
  }

  void Tick() {
    if (!initialized)
      Init();

    kMANAGER->Draw();
    kMANAGER->GetDrawList()->Draw();
  }
}