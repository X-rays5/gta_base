cpmaddpackage("gh:abseil/abseil-cpp#20230802.1")

target_link_libraries(${PROJECT_NAME} PRIVATE
                      absl::base
                      absl::status
                      absl::statusor
                      absl::synchronization)