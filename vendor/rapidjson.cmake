CPMAddPackage("gh:Tencent/rapidjson#80b6d1c83402a5785c486603c5611923159d0894")
target_include_directories(${PROJECT_NAME} PRIVATE ${rapidjson_SOURCE_DIR}/include)