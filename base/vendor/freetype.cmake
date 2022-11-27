CPMAddPackage("gh:freetype/freetype#c943d408e08fbe6d3e12f647b4a10d94a8198e9b")

target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)