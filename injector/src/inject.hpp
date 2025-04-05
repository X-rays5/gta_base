//
// Created by X-ray on 04/04/2025.
//

#ifndef INJECT_HPP_04185117
#define INJECT_HPP_04185117
#include <base-common/util/result.hpp>

namespace base::injector {
  Status Inject(DWORD pid, const std::filesystem::path& dll);
}

#endif //INJECT_HPP_04185117
