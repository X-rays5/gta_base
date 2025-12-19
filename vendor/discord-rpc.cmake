cpmaddpackage("gh:X-rays5-forks/discord-rpc#6bd5ea640fe669e0912e047d0b8c2b846ea81ed0")

target_include_directories(${PROJECT_NAME} PUBLIC ${discord-rpc_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC discord-rpc)