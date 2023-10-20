cpmaddpackage("gh:abseil/abseil-cpp#20230802.1")

target_link_libraries(${PROJECT_NAME} PRIVATE absl::status absl::statusor)