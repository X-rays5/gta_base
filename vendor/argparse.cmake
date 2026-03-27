CPMAddPackage("gh:p-ranav/argparse@3.2")

target_link_libraries(${PROJECT_NAME} PUBLIC argparse::argparse)