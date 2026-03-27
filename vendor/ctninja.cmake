CPMAddPackage(
        NAME ctninja
        GIT_REPOSITORY https://github.com/X-rays5-forks/ctninja.git
        GIT_TAG 020e1c505ce8f09dc223cf68ebd2f91a66c5a7b6
        DOWNLOAD_ONLY YES
)

if (ctninja_ADDED)
    set(CTNINJA_SRC_DIR ${ctninja_SOURCE_DIR}/ctninja/src)
    set(CTNINJA_INCLUDE_DIR ${ctninja_SOURCE_DIR}/ctninja/include)

    message(STATUS "ctninja source directory: ${CTNINJA_SRC_DIR}")
    message(STATUS "ctninja include directory: ${CTNINJA_INCLUDE_DIR}")

    file(GLOB_RECURSE CTNINJA_SRC_FILES
            "${CTNINJA_SRC_DIR}/*.cpp"
            "${CTNINJA_SRC_DIR}/*.h"
            "${CTNINJA_SRC_DIR}/*.asm"
    )
    file(GLOB_RECURSE CTNINJA_INCLUDE_FILES "${CTNINJA_INCLUDE_DIR}/ctninja/*.h")

    add_library(ctninja STATIC ${CTNINJA_SRC_FILES} ${CTNINJA_INCLUDE_FILES})
    add_library(ctninja::ctninja ALIAS ctninja)

    set_target_properties(ctninja PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
    )

    target_include_directories(ctninja PRIVATE ${CTNINJA_INCLUDE_DIR}/ctninja)

    target_include_directories(ctninja INTERFACE ${CTNINJA_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PUBLIC ctninja)
endif ()