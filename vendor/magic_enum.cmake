CPMAddPackage("gh:Neargye/magic_enum#f34f967")

target_include_directories(${PROJECT_NAME} PRIVATE ${magic_enum_SOURCE_DIR}/include)