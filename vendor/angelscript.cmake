cmake_minimum_required(VERSION 3.16)

CPMAddPackage(
        NAME Angelscript
        GITHUB_REPOSITORY anjo76/angelscript
        GIT_TAG v2.38.0
        DOWNLOAD_ONLY YES
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(AS_NO_EXCEPTIONS OFF CACHE BOOL "" FORCE)
set(AS_DISABLE_INSTALL ON CACHE BOOL "" FORCE)

add_subdirectory(
        ${Angelscript_SOURCE_DIR}/sdk/angelscript/projects/cmake
        ${CMAKE_CURRENT_BINARY_DIR}/angelscript
)

target_link_libraries(${PROJECT_NAME} PRIVATE angelscript)
target_compile_definitions(${PROJECT_NAME} PRIVATE AS_USE_NAMESPACE)

set(ANGELSCRIPT_ADD_ON_DIR
        ${Angelscript_SOURCE_DIR}/sdk/add_on
)

set(ANGELSCRIPT_AUTOWRAPPER_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/autowrapper/aswrappedcall.h
)

set(ANGELSCRIPT_DATETIME_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/datetime/datetime.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/datetime/datetime.h
)

set(ANGELSCRIPT_ANY_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptany/scriptany.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptany/scriptany.h
)

set(ANGELSCRIPT_ARRAY_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptarray/scriptarray.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptarray/scriptarray.h
)

set(ANGELSCRIPT_SCRIPTBUILDER_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptbuilder/scriptbuilder.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptbuilder/scriptbuilder.h
)

set(ANGELSCRIPT_SCRIPTDICTIONARY_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptdictionary/scriptdictionary.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptdictionary/scriptdictionary.h
)

set(ANGELSCRIPT_SCRIPTHELPER_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scripthelper/scripthelper.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scripthelper/scripthelper.h
)

set(ANGELSCRIPT_STD_STRING_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptstdstring/scriptstdstring.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptstdstring/scriptstdstring.h
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptstdstring/scriptstdstring_utils.cpp
)

set(ANGELSCRIPT_MATH_ADD_ON
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptmath/scriptmath.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptmath/scriptmath.h
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptmath/scriptmathcomplex.cpp
        ${ANGELSCRIPT_ADD_ON_DIR}/scriptmath/scriptmathcomplex.h
)

add_library(angelscript_add_ons STATIC
        ${ANGELSCRIPT_AUTOWRAPPER_ADD_ON}
        ${ANGELSCRIPT_DATETIME_ADD_ON}
        ${ANGELSCRIPT_ANY_ADD_ON}
        ${ANGELSCRIPT_ARRAY_ADD_ON}
        ${ANGELSCRIPT_SCRIPTBUILDER_ADD_ON}
        ${ANGELSCRIPT_SCRIPTDICTIONARY_ADD_ON}
        ${ANGELSCRIPT_SCRIPTHELPER_ADD_ON}
        ${ANGELSCRIPT_STD_STRING_ADD_ON}
        ${ANGELSCRIPT_MATH_ADD_ON}
)

target_compile_definitions(angelscript_add_ons PUBLIC AS_USE_NAMESPACE)

target_link_libraries(angelscript_add_ons
        PUBLIC
        angelscript
)

# --------------------------------------------------------------------------
# Public add-on headers
# --------------------------------------------------------------------------

set(ANGELSCRIPT_ADD_ON_INCLUDE_DIR
        ${CMAKE_CURRENT_BINARY_DIR}/include
)

set(ANGELSCRIPT_ADD_ON_HEADERS
        ${ANGELSCRIPT_AUTOWRAPPER_ADD_ON}
        ${ANGELSCRIPT_DATETIME_ADD_ON}
        ${ANGELSCRIPT_ANY_ADD_ON}
        ${ANGELSCRIPT_ARRAY_ADD_ON}
        ${ANGELSCRIPT_SCRIPTBUILDER_ADD_ON}
        ${ANGELSCRIPT_SCRIPTDICTIONARY_ADD_ON}
        ${ANGELSCRIPT_SCRIPTHELPER_ADD_ON}
        ${ANGELSCRIPT_STD_STRING_ADD_ON}
        ${ANGELSCRIPT_MATH_ADD_ON}
)

foreach(HEADER IN LISTS ANGELSCRIPT_ADD_ON_HEADERS)
    if(HEADER MATCHES "\\.h$")
        file(RELATIVE_PATH RELATIVE_HEADER
                ${ANGELSCRIPT_ADD_ON_DIR}
                ${HEADER}
        )

        configure_file(
                ${HEADER}
                ${ANGELSCRIPT_ADD_ON_INCLUDE_DIR}/angelscript/${RELATIVE_HEADER}
                COPYONLY
        )
    endif()
endforeach()

target_include_directories(angelscript_add_ons
        PUBLIC
        $<BUILD_INTERFACE:${ANGELSCRIPT_ADD_ON_INCLUDE_DIR}>
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        angelscript_add_ons
)