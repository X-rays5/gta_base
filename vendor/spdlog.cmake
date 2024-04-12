cpmaddpackage("gh:X-rays5-forks/spdlog#a2b4262")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)