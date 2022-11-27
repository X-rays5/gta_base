CPMAddPackage("gh:gabime/spdlog#77429b2e2e70feb5d2619374f2bc99326f6570dd")

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog)