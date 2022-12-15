CPMAddPackage("gh:gabime/spdlog#edc51df1bdad8667b628999394a1e7c4dc6f3658")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)