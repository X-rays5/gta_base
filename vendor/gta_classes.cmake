CPMAddPackage(
        NAME GTAV-Classes
        GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
        GIT_TAG 620948d00e390246f3f36b28595da80eb33b7d29
        DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})