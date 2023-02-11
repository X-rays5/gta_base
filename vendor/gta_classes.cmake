cpmaddpackage(
  NAME GTAV-Classes
  GIT_REPOSITORY https://github.com/Yimura/GTAV-Classes
  GIT_TAG 2862a02e24ebfedc60432887e735efef56d83635
  DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${GTAV-Classes_SOURCE_DIR})