CPMAddPackage("gh:Neargye/magic_enum#v0.8.2")

target_include_directories(${PROJECT_NAME} PRIVATE ${magic_enum_SOURCE_DIR}/include)