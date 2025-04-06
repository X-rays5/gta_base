# Find OpenCppCoverage executable
find_program(OPENCPPCOVERAGE_EXECUTABLE OpenCppCoverage.exe)
if (NOT OPENCPPCOVERAGE_EXECUTABLE)
    message(WARNING "OpenCppCoverage not found, coverage reports will not be available")
    return()
endif ()

function(add_combined_coverage_target TARGET_NAME OUTPUT_DIR)
    # Get all test targets (skip the first argument which is TARGET_NAME)
    set(TEST_TARGETS ${ARGN})

    # Convert paths to native format
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}" NATIVE_SOURCE_DIR)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" NATIVE_BUILD_DIR)
    file(TO_NATIVE_PATH "${OUTPUT_DIR}" NATIVE_OUTPUT_DIR)

    # Create a batch file to run all tests sequentially
    set(BATCH_FILE "${CMAKE_BINARY_DIR}/run_all_tests.bat")
    file(WRITE ${BATCH_FILE} "@echo off\r\n")

    # Add each test executable to the batch file with proper path handling
    foreach (TEST_TARGET ${TEST_TARGETS})
        # Use generator expression to get the actual executable path
        set(TEST_EXE_PATH "$<TARGET_FILE:${TEST_TARGET}>")

        # Use file(GENERATE) for proper path expansion of generator expressions
        set(TEST_CMD_FILE "${CMAKE_BINARY_DIR}/run_${TEST_TARGET}.cmake")
        file(GENERATE
                OUTPUT "${TEST_CMD_FILE}"
                CONTENT "file(APPEND \"${BATCH_FILE}\" \"\\\"${TEST_EXE_PATH}\\\"\\r\\nif %ERRORLEVEL% neq 0 exit /b %ERRORLEVEL%\\r\\n\")"
                CONDITION $<TARGET_EXISTS:${TEST_TARGET}>
        )

        # Process the generated file during build time
        add_custom_command(
                OUTPUT "${CMAKE_BINARY_DIR}/run_${TEST_TARGET}.stamp"
                COMMAND ${CMAKE_COMMAND} -P "${TEST_CMD_FILE}"
                DEPENDS ${TEST_TARGET}
                COMMENT "Adding ${TEST_TARGET} to test batch file"
                VERBATIM
        )

        # Add the stamp file as a dependency
        set(STAMP_FILES ${STAMP_FILES} "${CMAKE_BINARY_DIR}/run_${TEST_TARGET}.stamp")
    endforeach ()

    # Create a custom target to generate the batch file
    add_custom_target(
            ${TARGET_NAME}_batch
            DEPENDS ${STAMP_FILES}
            COMMENT "Generating test batch file"
    )

    # Add custom target to run OpenCppCoverage on all tests
    add_custom_target(${TARGET_NAME}
            COMMAND ${OPENCPPCOVERAGE_EXECUTABLE}
            --sources=${NATIVE_SOURCE_DIR}
            --excluded_sources=${NATIVE_BUILD_DIR}
            --excluded_sources=*test*
            --export_type=html:${NATIVE_OUTPUT_DIR}
            --export_type=cobertura:${NATIVE_OUTPUT_DIR}/coverage.xml
            --cover_children
            --
            cmd.exe /c "${BATCH_FILE}"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running OpenCppCoverage on all tests"
            DEPENDS ${TARGET_NAME}_batch ${TEST_TARGETS}
    )
endfunction()