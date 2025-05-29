CPMAddPackage("gh:google/googletest@1.16.0")

target_include_directories(${PROJECT_NAME} PRIVATE ${googletest_SOURCE_DIR}/googletest/include)
target_link_libraries(${PROJECT_NAME} PRIVATE gtest gtest_main)