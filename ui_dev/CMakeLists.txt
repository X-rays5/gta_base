cmake_minimum_required(VERSION 3.16)
project(ui_dev)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype)

file(GLOB IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
)

add_executable(${PROJECT_NAME} ${IMGUI_SOURCES} src/main.cpp src/ui/ui.cpp src/ui/ui.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/option/executeoption.hpp src/ui/draw.hpp src/ui/components/option/baseoption.hpp src/ui/components/submenu.hpp src/ui/components/enums.hpp src/ui/fonts/roboto_regular.hpp src/ui/components/option/submenuoption.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/freetype/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE freetype)