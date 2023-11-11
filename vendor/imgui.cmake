#set(ENABLE_DX10_BACKEND ON)
#set(ENABLE_DX11_BACKEND ON)

#cpmaddpackage("gh:X-rays5/imgui_cmake#master")

#target_link_libraries(${PROJECT_NAME} PRIVATE imgui::imgui)

include(FetchContent)

set(LIB_NAME "imgui")
message("Setting up ${LIB_NAME}")

FetchContent_Declare(
    ${LIB_NAME}
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        c6e0284ac58b3f205c95365478888f7b53b077e2
	GIT_PROGRESS TRUE
)

FetchContent_GetProperties(${LIB_NAME})
if(NOT imgui_POPULATED)
    FetchContent_Populate(${LIB_NAME})

    file(GLOB SRC_IMGUI
        "${imgui_SOURCE_DIR}/*.cpp"
        "${imgui_SOURCE_DIR}/*.h"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_win32.*"
         "${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.*"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.*"
        "${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.*"
    )

    add_library(imgui STATIC ${SRC_IMGUI})
    source_group(TREE ${imgui_SOURCE_DIR} PREFIX "imgui" FILES ${SRC_IMGUI} )
    target_include_directories(imgui PRIVATE
        "${imgui_SOURCE_DIR}"
        "${imgui_SOURCE_DIR}/backends"
        "${imgui_SOURCE_DIR}/misc/cpp"
    )
endif()