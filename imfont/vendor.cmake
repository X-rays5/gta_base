find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)

include(../vendor/unordered_dense.cmake)
include(../vendor/battery_embed.cmake)