option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
option(ZYDIS_BUILD_DOXYGEN "" OFF)
cpmaddpackage("gh:X-rays5-forks/zydis#d347321")

target_link_libraries(${PROJECT_NAME} PRIVATE Zydis)