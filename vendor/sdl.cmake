CPMAddPackage("gh:libsdl-org/SDL#release-3.2.4")

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)