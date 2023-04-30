cpmaddpackage("gh:gabime/spdlog#e4f92bed4880e682c54bff5b5092b4968cb807b3")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)