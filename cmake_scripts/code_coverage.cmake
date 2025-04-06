# Find OpenCppCoverage executable
find_program(OPENCPPCOVERAGE_EXECUTABLE OpenCppCoverage.exe)
if (NOT OPENCPPCOVERAGE_EXECUTABLE)
    message(WARNING "OpenCppCoverage not found, coverage reports will not be available")
    return()
endif ()

function(add_coverage_target TARGET_NAME TEST_TARGET OUTPUT_DIR)
    # Convert paths to native format (with backslashes on Windows)
    file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}" NATIVE_SOURCE_DIR)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}" NATIVE_BUILD_DIR)  # Removed the "/build" suffix
    file(TO_NATIVE_PATH "${OUTPUT_DIR}" NATIVE_OUTPUT_DIR)

    add_custom_target(${TARGET_NAME}
            COMMAND ${OPENCPPCOVERAGE_EXECUTABLE}
            --sources=${NATIVE_SOURCE_DIR}
            --excluded_sources=${NATIVE_BUILD_DIR}
            --excluded_sources=*test*
            --export_type=html:${NATIVE_OUTPUT_DIR}
            --export_type=cobertura:${NATIVE_OUTPUT_DIR}/coverage.xml
            --cover_children
            --
            $<TARGET_FILE:${TEST_TARGET}>
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Running OpenCppCoverage on ${TEST_TARGET}"
    )
endfunction()