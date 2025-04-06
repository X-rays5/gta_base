//
// Created by X-ray on 05/04/2025.
//

#ifndef TEST_GLOBALS_HPP_04165902
#define TEST_GLOBALS_HPP_04165902

const std::string test_dummy_window_name = "Test Window";
const std::string test_dummy_window_class = "TestWindowClass";

inline HWND CreateDummyWindow(const std::string& title, const std::string& class_name) {
  WNDCLASS wc = {0};
  wc.lpfnWndProc = DefWindowProc;
  wc.hInstance = GetModuleHandle(nullptr);
  wc.lpszClassName = class_name.c_str();

  RegisterClass(&wc);

  const HWND hwnd = CreateWindowExA(
    0,
    class_name.c_str(),
    title.c_str(),
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 300, 300,
    nullptr,
    nullptr,
    wc.hInstance,
    nullptr
  );

  ShowWindow(hwnd, SW_SHOW);

  return hwnd;
}

inline void DestroyDummyWindow(const HWND hwnd) {
  DestroyWindow(hwnd);
  UnregisterClass(test_dummy_window_class.c_str(), GetModuleHandle(nullptr));
}

#endif //TEST_GLOBALS_HPP_04165902
