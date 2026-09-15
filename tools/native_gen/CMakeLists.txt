cmake_minimum_required(VERSION 3.26)
project(native_gen CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

# Core library containing all source files except main.cpp
add_library(${PROJECT_NAME}_core OBJECT
        src/constants.hpp
        src/globals.hpp
        src/cli/cli.cpp
        src/cli/cli.hpp
        src/config/config.cpp
        src/config/config.hpp
        src/db/fetch.cpp
        src/db/fetch.hpp
        src/db/model.cpp
        src/db/model.hpp
        src/gen/emitters.cpp
        src/gen/emitters.hpp
        src/gen/generator.cpp
        src/gen/generator.hpp
        src/util/log.cpp
        src/util/log.hpp
        src/util/util.cpp
        src/util/util.hpp
)
add_library(base::${PROJECT_NAME}_core ALIAS ${PROJECT_NAME}_core)

# Main executable that uses the core library
add_executable(${PROJECT_NAME}
        src/main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_core)

# The tool logs through base::common, the same as the rest of the repository. common links
# spdlog, fmt, glaze and xorstr PUBLIC, so they arrive from here and none of their vendor
# recipes need including again.
target_link_libraries(${PROJECT_NAME}_core PUBLIC base::common)

# Configure core library
set_property(TARGET ${PROJECT_NAME}_core PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME}_core PRIVATE /bigobj /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME}_core PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

# Configure main executable. Deliberately not a WIN32_EXECUTABLE - this is a console tool.
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tools_bin")
set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
target_compile_options(${PROJECT_NAME} PRIVATE /bigobj /diagnostics:caret /W4 /permissive- /WX)
target_compile_definitions(${PROJECT_NAME} PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)

# Ship the default config beside the binary so it works without --config from any directory.
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/config.toml"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/config.toml"
        COMMENT "Copying native_gen config.toml")
