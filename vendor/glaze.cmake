CPMAddPackage(
    NAME glaze
    GITHUB_REPOSITORY stephenberry/glaze
    GIT_TAG v8.3.0
    DOWNLOAD_ONLY YES
)

include(${glaze_SOURCE_DIR}/cmake/glaze-asio.cmake)
glaze_setup_asio()

set(glaze_ENABLE_SSL ON CACHE BOOL "Enable SSL support in glaze" FORCE)
set(glaze_INCLUDES_WITH_SYSTEM OFF CACHE BOOL "" FORCE)

# This file is included once per consumer (common, menu, injector); add_subdirectory
# must run once or glaze's add_library(glaze_glaze) trips CMP0002 on the second call.
if (NOT TARGET glaze::glaze)
    add_subdirectory(${glaze_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/glaze)
endif ()

target_link_libraries(${PROJECT_NAME} PUBLIC glaze::glaze glaze::asio)