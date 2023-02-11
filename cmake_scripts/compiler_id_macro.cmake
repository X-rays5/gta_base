string(TOUPPER ${CMAKE_CXX_COMPILER_ID} COMPILER_ID)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${COMPILER_ID})
message(STATUS "compiler id: ${COMPILER_ID}")