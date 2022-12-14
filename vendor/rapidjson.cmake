CPMAddPackage("gh:Tencent/rapidjson#27c3a8dc0e2c9218fe94986d249a12b5ed838f1d")
target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)