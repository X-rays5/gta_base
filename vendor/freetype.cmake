CPMAddPackage("gh:freetype/freetype#ace97a02a4461bbdae29da4019c105eead95e277")

target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)