set(KIERO_SUPPORT_D3D10 ON)
set(KIERO_SUPPORT_D3D10_1 ON)
set(KIERO_SUPPORT_D3D11 ON)

cpmaddpackage("gh:X-rays5/kiero#master")

target_link_libraries(${PROJECT_NAME} PRIVATE kiero)