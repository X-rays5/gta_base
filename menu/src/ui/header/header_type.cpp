//
// Created by X-ray on 14/09/2026.
//

#include "header_type.hpp"
#include "header_image.hpp"
#include "image_header.hpp"
#include "text_header.hpp"

namespace base::menu::ui {
  std::unique_ptr<BaseHeader> MakeHeader(const HeaderType type, const std::string_view image_path) {
    if (type == HeaderType::kImage) {
      return std::make_unique<ImageHeader>(header_image::Load(image_path));
    }

    return std::make_unique<TextHeader>();
  }
}
