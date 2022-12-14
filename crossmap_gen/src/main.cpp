//
// Created by X-ray on 12/14/22.
//
#include "get_files.hpp"
#include "script_file.hpp"

int main(int argc, char* argv[]) {
  FLAGS_alsologtostderr = true;
  google::InitGoogleLogging(argv[0]);
  //google::InstallFailureSignalHandler();

  auto script_files = crossmap_gen::files::GetFiles("./");
  LOG(INFO) << "Found " << script_files.size() << " script files";

  for (auto&& file : script_files) {
    try {
      crossmap_gen::script::File script_file(file.new_path);
    } catch (const std::exception& e) {
      LOG(ERROR) << e.what();
    }
  }

  return 0;
}