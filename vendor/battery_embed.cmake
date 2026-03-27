SET(B_PRODUCTION_MODE ON CACHE BOOL "" FORCE)

cpmaddpackage("gh:batterycenter/embed@1.2.19")

target_compile_definitions(${PROJECT_NAME} PUBLIC B_PRODUCTION_MODE)