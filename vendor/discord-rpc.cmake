cpmaddpackage("gh:X-rays5-forks/discord-rpc#7b5ef33efc78b593c17ed371566fb5def882b312")

target_include_directories(${PROJECT_NAME} PUBLIC ${discord-rpc_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC discord-rpc)