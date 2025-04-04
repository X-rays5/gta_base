CPMAddPackage("gh:libsdl-org/SDL#release-3.2.10")

target_link_libraries(${PROJECT_NAME} PUBLIC SDL3::SDL3)