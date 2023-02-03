cpmaddpackage(
  NAME GTAV-Classes
  GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
  GIT_TAG f155790d38c131f4df0933283a577423e0ddd2da
  DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})