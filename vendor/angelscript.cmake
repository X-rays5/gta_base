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

# PUBLIC, not PRIVATE: this file is included from asjit, which owns the engine build and re-exports
# it. PRIVATE here would stop the engine library, the add-on headers and AS_USE_NAMESPACE all at
# asjit's own target, and nothing downstream would see them.
target_link_libraries(${PROJECT_NAME} PUBLIC angelscript)
target_compile_definitions(${PROJECT_NAME} PUBLIC AS_USE_NAMESPACE)

# The engine library itself, which the two lines above do not reach - they name the *consumer*.
#
# Without this the tree is half-namespaced: `angelscript_add_ons` and every consumer are compiled with
# AS_USE_NAMESPACE, so they see `AngelScript::asIScriptEngine`, while angelscript.lib's own objects are
# not, so its internal C++ symbols - everything that is not the `extern "C"` API - are unqualified.
# Nothing notices until something reaches one of them, and then the reference is to
# `AngelScript::as_powi` and the definition is `as_powi`.
#
# PUBLIC to match everywhere else, and because it is what the comment above already claims the tree
# does. The `extern "C"` public API is unaffected either way: C linkage has no namespace to lose.
target_compile_definitions(angelscript PUBLIC AS_USE_NAMESPACE)

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

target_compile_definitions(angelscript_add_ons PUBLIC AS_USE_NAMESPACE AS_USE_STLNAMES=1)

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

# This is the line that carries everything downstream: angelscript_add_ons already exports
# AS_USE_NAMESPACE, AS_USE_STLNAMES=1 and the engine library PUBLIC, so linking it PUBLIC is what
# lets menu_core go on compiling as AngelScript::asIScriptEngine after the include() moved to asjit.
target_link_libraries(${PROJECT_NAME}
        PUBLIC
        angelscript_add_ons
)