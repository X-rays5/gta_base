cpmaddpackage("gh:X-rays5-forks/discord-rpc#b48a52c2b94f216e09e78d4a8b9ed616a22a93ed")

target_include_directories(${PROJECT_NAME} PRIVATE ${discord-rpc_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PRIVATE discord-rpc)