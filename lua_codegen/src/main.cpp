//
// Created by xray on 07-03-22.
//
#include <filesystem>
#include <iostream>
#include <string>
#include "convert.hpp"

std::filesystem::path GetTargetFilePath() {
  std::cout << "Please input a path to the file to convert: ";
  std::string input;
  std::getline(std::cin, input);

  if (!std::filesystem::is_regular_file(input))
    return GetTargetFilePath();

  return input;
}

int main(int argc, char* argv[]) {
  std::filesystem::path target_file;
  if (argc < 2) {
    target_file = GetTargetFilePath();
  } else {
    if (!std::filesystem::is_regular_file(argv[1]))
      target_file = GetTargetFilePath();
    else
      target_file = argv[1];
  }

  lua_codegen::convert::ConvertFile(target_file);

  return 0;
}