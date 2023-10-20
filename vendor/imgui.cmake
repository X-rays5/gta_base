set(ENABLE_DX10_BACKEND ON)
set(ENABLE_DX11_BACKEND ON)

cpmaddpackage("gh:X-rays5/imgui_cmake#master")

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)