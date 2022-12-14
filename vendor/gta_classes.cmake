CPMAddPackage(
        NAME GTAV-Classes
        GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
        GIT_TAG 257115997e5ecea9fafbc820a746d9c6f5889e57
        DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})