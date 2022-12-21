CPMAddPackage(
        NAME GTAV-Classes
        GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
        GIT_TAG 5ab30af647d7e64404d76ea6168e7f78f5e365af
        DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})