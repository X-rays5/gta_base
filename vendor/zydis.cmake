option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
option(ZYDIS_BUILD_DOXYGEN "" OFF)
cpmaddpackage("gh:zyantific/zydis@4.1.1")

target_link_libraries(${PROJECT_NAME} PUBLIC Zydis)

# While Zydis provides the exact same thing as below. CMake configure fails without this.
if (NOT TARGET Zydis::Zydis)
    add_library("Zydis::Zydis" ALIAS Zydis)
endif()