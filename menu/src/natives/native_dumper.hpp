//
// Created for dumping native handlers at runtime
// This utility extracts the actual RVA addresses from initialized native handlers
//

#pragma once

#include "invoker.hpp"
#include "native_lookup.hpp"

namespace base::menu::natives {

  struct NativeHandlerRva {
    std::string name;
    std::string return_type;
    std::string params;
    std::uintptr_t rva;
  };

  class NativeDumper {
  public:
    /**
     * Dumps all native handlers to a JSON file with their RVA addresses
     * Must be called after Invoker initialization (after kINVOKER is created)
     *
     * @param output_path Path where to save the JSON file (e.g., "C:\\native_handlers.json")
     */
#ifndef NDEBUG
    static void DumpHandlersToFile(const std::string& output_path) {
      if (!kINVOKER) {
        LOG_ERROR("NativeDumper: Invoker not initialized!");
        return;
      }

      HMODULE game_module = GetModuleHandleA(nullptr);
      const std::uint64_t base_address = reinterpret_cast<std::uint64_t>(game_module);

      const auto& handlers = kINVOKER->GetNativeHandlers();

      std::vector<NativeHandlerRva> handler_vector;
      handler_vector.reserve(handlers.size());
      for (size_t i = 0; i < handlers.size(); ++i) {
        auto handler = handlers.at(i);
        if (!handler) continue;

        const auto rva = reinterpret_cast<std::uintptr_t>(handler) - base_address;
         const auto sig = GetNativeSignature(static_cast<std::uint64_t>(i));
        handler_vector.push_back({sig.name, sig.return_type, sig.params, rva});
      }

      auto ec = glz::write_file_json(handler_vector, output_path, std::string{});
      if (ec) {
        LOG_ERROR("NativeDumper: Failed to write to file '{}': {}", output_path, ec);
        return;
      }

      LOG_INFO("NativeDumper: Successfully dumped {} native handlers to '{}'", handler_vector.size(), output_path);
    }
#else
    static void DumpHandlersToFile(const std::string& output_path) {
      LOG_INFO("NativeDumper: Dumping is only available in debug builds.");
    }
#endif
  };

} // namespace base::menu::natives


