CPMAddPackage("gh:fmtlib/fmt#12.2.0")

target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt)