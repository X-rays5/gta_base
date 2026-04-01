CPMAddPackage(
  NAME minicoro
  GITHUB_REPOSITORY "X-rays5/minicoro"
  GIT_TAG "02dad0f8b7cbb12fe6e216ae7a76db15ca55cd7b"
  DOWNLOAD_ONLY YES)

if (NOT minicoro_ADDED)
    message(FATAL_ERROR "Failed to add minicoro")
endif ()

set(MINICORO_COMPILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/tmp/minicoro)
set(MINICORO_COMPILE_FILE ${MINICORO_COMPILE_DIR}/minicoro.c)

file(WRITE ${MINICORO_COMPILE_FILE} "#include \"minicoro.h\"")

add_library(minicoro STATIC ${MINICORO_COMPILE_FILE})
add_library(minicoro::minicoro ALIAS minicoro)

target_compile_definitions(minicoro PRIVATE MINICORO_IMPL MCO_USE_ASM)

configure_file(${minicoro_SOURCE_DIR}/minicoro.h ${MINICORO_COMPILE_DIR}/minicoro.h COPYONLY)
configure_file(${minicoro_SOURCE_DIR}/minicoro.h ${CMAKE_CURRENT_BINARY_DIR}/include/minicoro/include/minicoro/minicoro.h COPYONLY)
message(STATUS "minicoro header copied to ${CMAKE_CURRENT_BINARY_DIR}/include/minicoro/include/minicoro/minicoro.h")

target_include_directories(minicoro INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include/minicoro/include)

target_link_libraries(${PROJECT_NAME} PRIVATE minicoro::minicoro)