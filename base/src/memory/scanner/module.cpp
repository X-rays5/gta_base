#include "module.hpp"

namespace gta_base::memory::scanner {
Module::Module(HMODULE mod) : Range(mod, 0) {
  auto dosHeader = base_.as<IMAGE_DOS_HEADER *>();
  auto ntHeader = base_.add(dosHeader->e_lfanew).as<IMAGE_NT_HEADERS *>();
  size_ = ntHeader->OptionalHeader.SizeOfImage;
}

Module::Module(std::nullptr_t) : Module(GetModuleHandle(nullptr)) {}

Module::Module(std::string_view name) : Module(GetModuleHandleA(name.data())) {}

Module::Module(std::wstring_view name) : Module(GetModuleHandleW(name.data())) {}

[[maybe_unused]] Handle Module::get_export(std::string_view symbol_name) {
  return GetProcAddress(base_.as<HMODULE>(), symbol_name.data());
}
}