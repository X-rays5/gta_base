cmake_minimum_required(VERSION 3.16)
project(ui_dev)

set(CMAKE_CXX_STANDARD 20)

file(GLOB IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/*.cpp"
)

include(FetchContent)

FetchContent_Declare(robin_hood GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git GIT_TAG bd2f7194ae5a52337a9dcaf06edd87338ed1d754)
FetchContent_MakeAvailable(robin_hood)

FetchContent_Declare(freetype GIT_REPOSITORY https://github.com/freetype/freetype.git GIT_TAG 1a242558be670626ed2ec62efb1909c000b2cae7)
FetchContent_MakeAvailable(freetype)

add_executable(${PROJECT_NAME} ${IMGUI_SOURCES} src/main.cpp src/ui/ui.cpp src/ui/ui.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/option/executeoption.hpp src/ui/components/option/baseoption.hpp src/ui/components/submenu.hpp src/ui/components/enums.hpp src/ui/components/option/submenuoption.hpp src/ui/notification/notification.cpp src/ui/notification/notification.hpp src/ui/translate/translate.cpp src/ui/translate/translate.hpp src/ui/util/timedinput.cpp src/ui/util/timedinput.hpp src/ui/util/draw.hpp src/ui/util/util.cpp src/ui/util/util.hpp src/ui/fonts/roboto_mono.hpp src/ui/components/option/numberoption.hpp src/ui/components/option/labeloption.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE freetype)