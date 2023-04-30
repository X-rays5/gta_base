cpmaddpackage(
  NAME GTAV-Classes
  GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
  GIT_TAG 57548ccd8742f0355fc674e8f4dedee119d640e5
  DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})