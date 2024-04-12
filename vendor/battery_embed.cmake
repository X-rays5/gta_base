SET(B_PRODUCTION_MODE ON CACHE BOOL "" FORCE)

cpmaddpackage("gh:batterycenter/embed@1.2.15")

target_compile_definitions(${PROJECT_NAME} PRIVATE B_PRODUCTION_MODE)