cpmaddpackage(
  NAME leopard
  GIT_REPOSITORY https://github.com/hosseinmoein/Leopard.git
  GIT_TAG 2.0.0
  DOWNLOAD_ONLY YES
)

target_include_directories(${PROJECT_NAME} PRIVATE ${leopard_SOURCE_DIR}/include)