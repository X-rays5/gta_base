cmake_minimum_required(VERSION 3.26)
project(menu_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/animated_image_test.cpp
        src/as_bind_test.cpp
        src/as_coro_test.cpp
        src/as_docs_test.cpp
        src/as_gui_test.cpp
        src/as_natives_test.cpp
        src/as_notify_test.cpp
        src/as_option_test.cpp
        src/as_script_manager_test.cpp
        src/as_value_test.cpp
        src/command_line_test.cpp
        src/draw_helpers_test.cpp
        src/game_handle_test.cpp
        src/general_task_executor_test.cpp
        src/generic_tree_test.cpp
        src/header_image_test.cpp
        src/header_type_test.cpp
        src/localization_overlay_test.cpp
        src/script_submenu_id_test.cpp
        src/submenu_test.cpp
        src/text_input_popup_test.cpp
)

# Link against the core library to share dependencies and avoid recompilation
# The add-ons are linked explicitly because the core library keeps AngelScript private, and the
# tests drive the engine directly. minicoropp is here for the same reason but for a different half
# of it: as_script_context.hpp includes <minicoropp/coroutine.hpp> in its own interface, while the
# core library links minicoropp privately, so a test that drives a script coroutine needs the
# headers on its own include path.
target_link_libraries(${PROJECT_NAME} PRIVATE base::menu_core angelscript_add_ons minicoropp::minicoropp)

include(../../vendor/gtest.cmake)
enable_testing()

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})