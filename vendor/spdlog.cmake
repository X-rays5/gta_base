cpmaddpackage("gh:gabime/spdlog#927cc29444a294d76e83dfb898e797dc431ce094")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_FMT_EXTERNAL)