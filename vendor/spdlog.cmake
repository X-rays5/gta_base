cpmaddpackage("gh:gabime/spdlog@1.12.0")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)