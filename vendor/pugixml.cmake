CPMAddPackage("gh:zeux/pugixml#v1.13")

target_include_directories(${PROJECT_NAME} PRIVATE ${pugixml_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE pugixml-static)