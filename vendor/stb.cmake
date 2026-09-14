CPMAddPackage("gh:nothings/stb#2c980bb59875b0d32144a71867fbdebb2f77cd20")

# stb has no CMakeLists, and the implementation macro has to be defined in exactly one
# translation unit. Keeping it in its own target keeps stb's warnings away from the
# /W4 /WX and precompiled header the consumers build with.
add_library(stb_image STATIC "${CMAKE_CURRENT_LIST_DIR}/stb/stb_image_impl.cpp")
target_include_directories(stb_image PUBLIC ${stb_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC stb_image)
