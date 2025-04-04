CPMAddPackage("gh:X-rays5-forks/result#master")

# Need to move the file into result/result.hpp to prevent collisions
file(GLOB_RECURSE result_files "${result_SOURCE_DIR}/include/*.hpp")
file(MAKE_DIRECTORY ${result_SOURCE_DIR}/include/result)
foreach(file ${result_files})
    message(STATUS "Moving ${file} to ${result_SOURCE_DIR}/include/result")
    get_filename_component(filename ${file} NAME)
    file(RENAME ${file} ${result_SOURCE_DIR}/include/result/${filename})
endforeach()

target_include_directories(${PROJECT_NAME} PUBLIC ${result_SOURCE_DIR}/include)