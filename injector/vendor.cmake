set(PROJECT_NAME_COPY ${PROJECT_NAME})
set(PROJECT_NAME ${PROJECT_NAME}_core)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

include(../vendor/glaze.cmake)
include(../vendor/enchantum.cmake)

set(PROJECT_NAME ${PROJECT_NAME_COPY})