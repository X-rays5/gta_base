cpmaddpackage("gh:freetype/freetype#4d8db130ea4342317581bab65fc96365ce806b77")

target_include_directories(${PROJECT_NAME} PRIVATE ${freetype_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)