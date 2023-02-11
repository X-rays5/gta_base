//
// Created by X-ray on 02/11/23.
//

#include "submenu.hpp"
#include "../../translation/translation_manager.hpp"

namespace gta_base::ui::draw::components {
  std::string components::Submenu::GetName() {
    return kTRANSLATION_MANAGER->Get(name_);
  }
}