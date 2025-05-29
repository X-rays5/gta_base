CPMAddPackage(
        NAME ctninja
        GIT_REPOSITORY https://github.com/X-rays5-forks/ctninja.git
        GIT_TAG 96c9bc4041833c60b6dea5641145e0d65f04aa36
        DOWNLOAD_ONLY YES
)

if (ctninja_ADDED)
    set(CTNINJA_SRC_DIR ${ctninja_SOURCE_DIR}/ctninja/src)
    set(CTNINJA_INCLUDE_DIR ${ctninja_SOURCE_DIR}/ctninja/include)

    file(GLOB_RECURSE CTNINJA_SRC_FILES
            "${CTNINJA_SRC_DIR}/*.cpp"
            "${CTNINJA_SRC_DIR}/*.h"
            "${CTNINJA_SRC_DIR}/*.asm"
    )
    file(GLOB_RECURSE CTNINJA_INCLUDE_FILES "${CTNINJA_INCLUDE_DIR}/*.h")

    add_library(ctninja STATIC ${CTNINJA_SRC_FILES} ${CTNINJA_INCLUDE_FILES})
    add_library(ctninja::ctninja ALIAS ctninja)

    set_target_properties(ctninja PROPERTIES
            CXX_STANDARD 20
            CXX_STANDARD_REQUIRED ON
    )

    foreach (CTNINJA_INCLUDE_FILE ${CTNINJA_INCLUDE_FILES})
        get_filename_component(CTNINJA_INCLUDE_FILE_NAME ${CTNINJA_INCLUDE_FILE} NAME)
        file(COPY ${CTNINJA_INCLUDE_FILE} DESTINATION ${ctninja_SOURCE_DIR}/include/ctninja)
    endforeach ()

    target_include_directories(ctninja PRIVATE ${CTNINJA_INCLUDE_DIR})

    target_include_directories(ctninja INTERFACE ${ctninja_SOURCE_DIR}/include)
    target_link_libraries(${PROJECT_NAME} PUBLIC ctninja)
endif ()