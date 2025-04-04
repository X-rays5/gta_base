cpmaddpackage("gh:X-rays5-forks/discord-rpc#master")

target_include_directories(${PROJECT_NAME} PUBLIC ${discord-rpc_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC discord-rpc)