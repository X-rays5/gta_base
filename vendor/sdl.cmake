CPMAddPackage("gh:libsdl-org/SDL#release-3.4.2")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)