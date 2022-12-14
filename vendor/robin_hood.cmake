CPMAddPackage(NAME robin_hood
        GIT_REPOSITORY https://github.com/martinus/robin-hood-hashing.git
        GIT_TAG fb1483621fda28d4afb31c0097c1a4a457fdd35b
)

target_include_directories(${PROJECT_NAME} PRIVATE ${robin_hood_SOURCE_DIR}/src/include)