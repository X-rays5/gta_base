option(ZYDIS_BUILD_TOOLS "" OFF)
option(ZYDIS_BUILD_EXAMPLES "" OFF)
CPMAddPackage("gh:zyantific/zydis#v4.0.0")

target_link_libraries(${PROJECT_NAME} PRIVATE Zydis)