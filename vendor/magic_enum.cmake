CPMAddPackage("gh:Neargye/magic_enum#159a35006d8e18172eb687b432b77e6f5baa61f6")

target_include_directories(${PROJECT_NAME} PRIVATE ${magic_enum_SOURCE_DIR}/include)