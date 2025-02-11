set(ENABLE_DX11_BACKEND ON)
set(ENABLE_SDL3_BACKEND ON)
set(IMGUI_DISABLE_OBSOLETE_FUNCTIONS OFF) # Without this off the dx11 renderer backend is broken.

cpmaddpackage("gh:X-rays5/imgui_cmake#master")

target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)