cpmaddpackage("gh:gabime/spdlog#a2b4262090fd3f005c2315dcb5be2f0f1774a005")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog_header_only)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)