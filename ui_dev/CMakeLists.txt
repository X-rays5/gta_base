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

FetchContent_Declare(rapidjson GIT_REPOSITORY https://github.com/Tencent/rapidjson.git GIT_TAG 27c3a8dc0e2c9218fe94986d249a12b5ed838f1d)
FetchContent_MakeAvailable(rapidjson)

add_executable(${PROJECT_NAME} ${IMGUI_SOURCES} src/main.cpp src/ui/ui.cpp src/ui/ui.hpp src/ui/manager.cpp src/ui/manager.hpp src/ui/components/option/executeoption.hpp src/ui/components/option/baseoption.hpp src/ui/components/submenu.hpp src/ui/components/enums.hpp src/ui/components/option/submenuoption.hpp src/ui/notification/notification.cpp src/ui/notification/notification.hpp src/ui/translate/translate.cpp src/ui/translate/translate.hpp src/ui/util/timedinput.cpp src/ui/util/timedinput.hpp src/ui/util/draw.hpp src/ui/util/util.cpp src/ui/util/util.hpp src/ui/fonts/roboto_mono.hpp src/ui/components/option/numberoption.hpp src/ui/components/option/labeloption.hpp src/ui/util/texturemanager.cpp src/ui/util/texturemanager.hpp src/main.hpp src/ui/components/option/toggleoption.hpp src/ui/components/option/listoption.hpp src/ui/translation_manager.cpp src/ui/translation_manager.hpp src/ui/components/keyboard.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb)
target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE freetype)