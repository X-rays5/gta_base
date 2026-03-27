cpmaddpackage("gh:gabime/spdlog@1.17.0")

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_FMT_EXTERNAL)