SET(B_PRODUCTION_MODE ON CACHE BOOL "" FORCE)

cpmaddpackage("gh:X-rays5-forks/embed@1.2.15")

target_compile_definitions(${PROJECT_NAME} PRIVATE B_PRODUCTION_MODE)