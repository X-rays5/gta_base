find_package(imgui CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

include(../vendor/battery_embed.cmake)
include(../vendor/unordered_dense.cmake)