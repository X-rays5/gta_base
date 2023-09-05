cpmaddpackage("gh:gabime/spdlog#3aceda041b2bfbccaf14d5c83eda16fc67bea364")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)