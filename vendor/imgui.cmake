set(ENABLE_DX11_BACKEND ON)
set(ENABLE_SDL3_BACKEND ON)
set(IMGUI_DISABLE_OBSOLETE_FUNCTIONS OFF) # Without this off the dx11 renderer backend is broken.
set(IMGUI_ENABLE_FREETYPE_LUNASVG OFF)

cpmaddpackage("gh:X-rays5/imgui_cmake#master")

target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)