CPMAddPackage(
        NAME spel64
        GIT_REPOSITORY https://github.com/sub1to/spel64
        GIT_TAG f83689c29294f9882c3f4ceea5504021584c423e
        DOWNLOAD_ONLY YES
)

if (spel64_ADDED)
    set(spel64_LIB_DIR ${spel64_SOURCE_DIR}/spel64)

    file(GLOB_RECURSE spel64_headers CONFIGURE_DEPENDS ${spel64_LIB_DIR}/*.h)
    file(GLOB_RECURSE spel64_asm CONFIGURE_DEPENDS ${spel64_LIB_DIR}/*.asm)
    file(GLOB_RECURSE spel64_inc CONFIGURE_DEPENDS ${spel64_LIB_DIR}/*.inc)
    file(GLOB_RECURSE spel64_cpp CONFIGURE_DEPENDS ${spel64_LIB_DIR}/*.cpp)

    add_library(spel64 STATIC
            ${spel64_headers}
            ${spel64_asm}
            ${spel64_inc}
            ${spel64_cpp}
    )
    add_library(spel64::spel64 ALIAS spel64)

    file(COPY ${spel64_LIB_DIR}/spel64.h DESTINATION ${spel64_SOURCE_DIR}/headers/spel64)
    target_include_directories(spel64 INTERFACE ${spel64_SOURCE_DIR}/headers)

    target_link_libraries(${PROJECT_NAME} PUBLIC spel64)
else()
    message(FATAL_ERROR "Could not find spel64")
endif()