cmake_minimum_required(VERSION 3.16)
project(lua_codegen)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(rapidjson GIT_REPOSITORY https://github.com/Tencent/rapidjson.git GIT_TAG 27c3a8dc0e2c9218fe94986d249a12b5ed838f1d)
FetchContent_MakeAvailable(rapidjson)

add_executable(${PROJECT_NAME} src/main.cpp src/convert.cpp src/convert.hpp src/types.hpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)