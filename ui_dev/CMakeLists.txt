cmake_minimum_required(VERSION 3.16)
project(ui_dev)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_impl_dx11.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_impl_win32.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp"
)

add_executable(ui_dev ${IMGUI_SOURCES} src/main.cpp src/ui/ui.cpp src/ui/ui.hpp)

target_include_directories(ui_dev PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)