set(ENABLE_DX10_BACKEND ON)
set(ENABLE_DX11_BACKEND ON)

cpmaddpackage("gh:X-rays5/imgui_cmake#9ddcb03103791a65d2f2d7021572613e6cfe80e7")

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)