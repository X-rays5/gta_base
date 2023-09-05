//
// Created by X-ray on 05/09/2023.
//

#include "logger.hpp"

namespace base::logging {
  namespace {
    bool SetConsoleMode(HANDLE console_handle) {
      DWORD console_mode;
      GetConsoleMode(console_handle, &console_mode);
      console_mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
      console_mode &= ~(ENABLE_QUICK_EDIT_MODE);

      return ::SetConsoleMode(console_handle, console_mode);
    }

    bool EnsureConsole() {
      if (AttachConsole(GetCurrentProcessId()) || AllocConsole()) {
        auto console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!console_handle)
          return false;

        SetConsoleTitleA("GTAV Base");
        SetConsoleOutputCP(CP_UTF8);

        SetConsoleMode(console_handle);

        return true;
      }

      return false;
    }
  }

  Manager::Manager() {
    Init();
    kMANAGER = this;
  }
  Manager::~Manager() {
    kMANAGER = nullptr;
    Shutdown();
  }

  void Manager::Init() {
    bool result = EnsureConsole();
    if (!result) {
      MessageBoxA(nullptr, xorstr_("There was an error creating/obtaining a console window."), xorstr_("Critical logging error"), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
      abort();
    }

  }

  void Manager::Shutdown() {

  }
}