CPMAddPackage("gh:open-source-parsers/jsoncpp#8190e061bc2d95da37479a638aa2c9e483e58ec6")

target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_static)