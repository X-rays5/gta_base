cpmaddpackage("gh:X-rays5-forks/spdlog#48bcf39a661a13be22666ac64db8a7f886f2637e") # 1.15.2

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog_header_only)
target_compile_definitions(${PROJECT_NAME} PUBLIC SPDLOG_FMT_EXTERNAL)