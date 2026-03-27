set(ENABLE_DX12_BACKEND ON)
set(ENABLE_SDL3_BACKEND ON)
set(IMGUI_ENABLE_FREETYPE_LUNASVG OFF)

cpmaddpackage("gh:X-rays5/imgui_cmake#1259c90cebfa4bfc2fd7c0f79b0ad275022264df")

target_link_libraries(${PROJECT_NAME} PUBLIC imgui::imgui)