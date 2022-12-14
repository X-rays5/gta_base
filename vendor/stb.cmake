CPMAddPackage("gh:nothings/stb#8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55")

target_include_directories(${PROJECT_NAME} PRIVATE ${stb_SOURCE_DIR})