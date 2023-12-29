option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
option(ZYDIS_BUILD_DOXYGEN "" OFF)
cpmaddpackage("gh:zyantific/zydis#ba1e3cf3a64f668790164c005d1ee52f3d77cf76")

target_link_libraries(${PROJECT_NAME} PRIVATE Zydis)