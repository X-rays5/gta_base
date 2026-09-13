# AsmJit, pinned to e24347521858616d2d070375ad0d52512e6e44ba (1.23.0).
#
# It is built by its own CMakeLists rather than by a hand-written target list: the sources are a flat
# `asmjit/` tree with the tests and benchmarks interleaved, and upstream is the only thing that knows
# which of them to compile on which platform. The one thing upstream does not offer is a way to
# rename its target, and that is the problem here.
#
# PolyHook_2 vendors AsmJit 1.17 of its own and brings it in from `asmtk/CMakeLists.txt` with an
# unconditional `include()` of that copy's CMakeLists - there is no `if (NOT TARGET asmjit)` guard to
# trip, and the external-AsmJit path is no way out either because it wants
# `find_package(asmjit CONFIG REQUIRED)` and AsmJit only ever writes an install-tree config. So
# PolyHook's copy is unavoidably going to create a target named `asmjit` and an alias
# `asmjit::asmjit`, and this file cannot do the same without a duplicate-target error at configure
# time.
#
# Hence the rewrite below: the copy CPM downloads has the handful of hardcoded target names changed
# to `asjit_asmjit` / `asjit::asmjit`, and is built from there. The two libraries coexist at link
# time regardless, because each version wraps its symbols in its own inline ABI namespace (v1_17
# there, v1_23 here), so a duplicate-symbol problem does not arise - only the duplicate target does.
#
# Must be included exactly once in the project: it adds a subdirectory, and a second include cannot.

if (TARGET asjit::asmjit)
    return()
endif ()

CPMAddPackage(
        NAME asmjit
        GITHUB_REPOSITORY asmjit/asmjit
        GIT_TAG e24347521858616d2d070375ad0d52512e6e44ba
        DOWNLOAD_ONLY YES
)

# --------------------------------------------------------------------------
# Rename upstream's targets
# --------------------------------------------------------------------------

set(ASMJIT_CMAKE_LISTS "${asmjit_SOURCE_DIR}/CMakeLists.txt")
file(READ "${ASMJIT_CMAKE_LISTS}" ASMJIT_CMAKE_LISTS_CONTENT)

# A re-configure reads a copy that has already been rewritten. The rename is a fixed set of literal
# substitutions, so writing it once is enough and leaving it alone afterwards is all that idempotence
# takes.
string(FIND "${ASMJIT_CMAKE_LISTS_CONTENT}" "asjit_asmjit" ASMJIT_ALREADY_RENAMED)
if (ASMJIT_ALREADY_RENAMED EQUAL -1)
    set(ASMJIT_RENAMED "${ASMJIT_CMAKE_LISTS_CONTENT}")

    # The alias is also what the test and benchmark targets link against. Those are not built here,
    # but renaming it everywhere keeps the file self-consistent.
    string(REPLACE "asmjit::asmjit" "asjit::asmjit" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "asmjit_embed" "asjit_asmjit_embed" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "asmjit_add_target(asmjit " "asmjit_add_target(asjit_asmjit " ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "target_compile_options(asmjit PUBLIC" "target_compile_options(asjit_asmjit PUBLIC" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "target_include_directories(asmjit BEFORE PUBLIC" "target_include_directories(asjit_asmjit BEFORE PUBLIC" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "ALIAS asmjit)" "ALIAS asjit_asmjit)" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "install(TARGETS asmjit" "install(TARGETS asjit_asmjit" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "EXPORT asmjit-config" "EXPORT asjit-asmjit-config" ASMJIT_RENAMED "${ASMJIT_RENAMED}")
    string(REPLACE "NAMESPACE asmjit::" "NAMESPACE asjit::" ASMJIT_RENAMED "${ASMJIT_RENAMED}")

    # Should upstream ever stop spelling the target this way, the substitutions above quietly do
    # nothing and the collision with PolyHook's copy comes back as a configure error somewhere far
    # from here. Failing at this point says what actually happened.
    if (NOT ASMJIT_RENAMED MATCHES "add_library\\(asjit::asmjit ALIAS asjit_asmjit\\)")
        message(FATAL_ERROR
                "asmjit: could not rename the 'asmjit' target in ${ASMJIT_CMAKE_LISTS}. Upstream "
                "no longer spells it the way this file expects, so it would collide with the copy "
                "PolyHook_2 vendors.")
    endif ()

    file(WRITE "${ASMJIT_CMAKE_LISTS}" "${ASMJIT_RENAMED}")
endif ()

# --------------------------------------------------------------------------
# Build it
# --------------------------------------------------------------------------

# Ordinary variables rather than cache entries, deliberately. Every one of these names is declared
# with option() by PolyHook's AsmJit 1.17 as well, and the cache is shared - so a `CACHE ... FORCE`
# here would hand PolyHook's copy our settings before it ever reads its own. A normal variable is
# inherited by the subdirectory and, since CMP0077, wins over option() without touching the cache.
set(ASMJIT_EMBED OFF)
set(ASMJIT_STATIC ON)
set(ASMJIT_NO_INSTALL ON)

# AsmJit's own test suite, off by default. It is the one thing in this file the build does not need:
# the library is verified against upstream's tests when the pin moves, or when one of our translations
# starts behaving differently, and not on every configure.
#
# Upstream's option is spelled "tests and benchmarks" and means both, so turning this on builds and
# registers more than the correctness tests: it also builds `asmjit_bench_codegen`, and registers
# `asmjit_bench_overhead` and `asmjit_bench_regalloc` as tests, which they are not. That is upstream's
# split and is left alone rather than second-guessed - the label below covers the whole set as one
# thing, so what `ctest -L asmjit` runs is exactly what this option registers.
#
# Unlike the three above this is a cache entry, and the difference is the collision argument rather
# than a change of mind. `ASMJIT_TEST` is a name PolyHook's copy declares too, so it stays an ordinary
# variable for the reason given above; `ASJIT_BUILD_ASMJIT_TESTS` is ours alone, so nothing can
# collide with it and `-DASJIT_BUILD_ASMJIT_TESTS=ON` on a configure line is worth having. Undeclared
# it reads as false, so a consumer of this file that knows nothing about the option gets the library
# and no tests.
option(ASJIT_BUILD_ASMJIT_TESTS "Build AsmJit's own test suite and register it with CTest" OFF)

# The two names are upstream's and ours and each means what it says, so the translation is spelled
# out rather than passed through.
if (ASJIT_BUILD_ASMJIT_TESTS)
    set(ASMJIT_TEST ON)
else ()
    set(ASMJIT_TEST OFF)
endif ()

add_subdirectory(
        "${asmjit_SOURCE_DIR}"
        "${CMAKE_CURRENT_BINARY_DIR}/asmjit"
)

if (NOT TARGET asjit::asmjit)
    message(FATAL_ERROR "asmjit: ${asmjit_SOURCE_DIR}/CMakeLists.txt did not create 'asjit::asmjit'.")
endif ()

# One label over the whole set, so upstream's suite is a selector rather than a list of names:
# `ctest -L asmjit` is the suite and `ctest -LE asmjit` is this project's own tests without it. The
# names are read back off the directory rather than written here, because which targets upstream
# declares as tests is upstream's to change - a name listed here that a later revision dropped would
# be a configure error in a file that is not about the tests. The inner `if` is what makes the
# theoretical case of the option being on and upstream registering nothing a no-op rather than an
# error.
if (ASJIT_BUILD_ASMJIT_TESTS)
    get_property(ASMJIT_TESTS DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/asmjit" PROPERTY TESTS)
    if (ASMJIT_TESTS)
        # Test properties are scoped to the directory a test was created in, and these were created
        # in the subdirectory above, so the scope has to be named. Without `DIRECTORY` this fails at
        # configure time with "Can not find test to add properties to".
        set_tests_properties(${ASMJIT_TESTS}
                DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/asmjit"
                PROPERTIES LABELS asmjit)
    endif ()
endif ()
