set(PROJECT_NAME_COPY ${PROJECT_NAME})
set(PROJECT_NAME ${PROJECT_NAME}_core)

include(../vendor/sdl.cmake)
include(../vendor/glaze.cmake)
include(../vendor/magic_enum.cmake)
include(../vendor/imgui.cmake)
target_link_libraries(imgui PRIVATE SDL3::SDL3)

set(PROJECT_NAME ${PROJECT_NAME_COPY})