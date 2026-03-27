CPMAddPackage("gh:Neargye/magic_enum#7d87efb4a3dddbbe8caa9ca14eff05ede1102ab8")

target_include_directories(${PROJECT_NAME} PUBLIC ${magic_enum_SOURCE_DIR}/include)