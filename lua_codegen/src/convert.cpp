//
// Created by X-ray on 09/24/22.
//

#include "convert.hpp"
#include <fstream>
#include <iostream>
#include <future>
#include <rapidjson/document.h>

namespace lua_codegen::convert {
  struct NativeParam {
    std::string name;
    std::string type;
  };

  struct Native {
    std::string addr;
    std::string name;
    std::string comment;
    std::vector<NativeParam> params;
    std::string return_type;
    std::vector<std::string> old_names;
  };

  static const std::filesystem::path cpp_out = "./cpp_out/native_def";
  static const std::filesystem::path lua_out = "./lua_out";

  void CorrectTypes(std::vector<Native>& natives) {
    for (auto&& native : natives) {
      if (native.return_type == "BOOL")
        native.return_type = "bool";

      for (auto&& param : native.params) {
        if (param.type == "BOOL")
          param.type = "bool";
      }
    }
  }

  void ToCppFile(const std::string& namespace_name, const std::vector<Native>& natives) {
    std::stringstream ss;

    ss << "namespace " << namespace_name << " {\n";
    for (auto&& native : natives) {
      if (!native.comment.empty())
        ss << "/*\n" << native.comment << "\n*/\n";
      if (!native.old_names.empty()) {
        ss << "/*";
        for (auto&& old_name : native.old_names)
          ss << old_name << " ";
        ss << "*/\n";
      }
      ss << "// https://nativedb.dotindustries.dev/natives/" << native.addr << "\n";
      ss << "inline " << native.return_type << " " << native.name << "(";
      for (auto&& param : native.params) {
        ss << param.type << " " << param.name << ", ";
      }
      if (!native.params.empty())
        ss.seekp(-2, std::ios_base::end);
      ss << ") {";
      ss << "return invoke<" << native.return_type << ">(" << native.addr << ", ";
      for (auto&& param : native.params) {
        if (param.type == "Vector3") {
          ss << param.name << ".x, " << param.name << ".y, " << param.name << ".z, ";
        } else {
          ss << param.name << ", ";
        }
      }
      ss.seekp(-2, std::ios_base::end);
      ss << ");";
      ss << "}\n";
    }
    ss << "}";

    auto out = cpp_out / (namespace_name + "_natives.hpp");
    if (std::filesystem::is_regular_file(out))
      std::filesystem::remove(out);

    std::ofstream writer(out);
    writer << "#include \"invoker.hpp\"\n";
    writer << "#include \"types.hpp\"\n";
    writer << ss.str();
    writer.close();
  }

  void ToLuaFile(const std::string& namespace_name, const std::vector<Native>& natives) {
    std::stringstream ss;

    ss << namespace_name << " = {\n";
    for (auto&& native : natives) {
      if (!native.comment.empty())
        ss << "--[[\n" << native.comment << "\n]]--\n";
      if (!native.old_names.empty()) {
        ss << "--[[";
        for (auto&& old_name : native.old_names)
          ss << old_name << " ";
        ss << "]]--\n";
      }
      ss << "-- https://nativedb.dotindustries.dev/natives/" << native.addr << "\n";
      ss << native.name << " = function(";
      for (auto&& param : native.params) {
        ss << param.name << ", ";
      }
      ss << ") end,\n";
    }
    ss << "}";

    auto out = lua_out / (namespace_name + "_natives.lua");
    if (std::filesystem::is_regular_file(out))
      std::filesystem::remove(out);

    std::ofstream writer(out);
    writer << ss.str();
    writer.close();
  }

  void ConvertNamespace(rapidjson::GenericMember<rapidjson::UTF8<>, rapidjson::MemoryPoolAllocator<>>& _namespace) {
    if (!_namespace.value.IsObject()) {
      std::cerr << "namespace is not a object" << std::endl;
    }

    std::vector<Native> natives;
    for (auto&& native : _namespace.value.GetObject()) {
      Native deserialized_native;
      deserialized_native.addr = native.name.GetString();
      deserialized_native.name = native.value["name"].GetString();
      deserialized_native.comment = native.value["comment"].GetString();
      for (auto&& param : native.value["params"].GetArray()) {
        deserialized_native.params.emplace_back(NativeParam{param["name"].GetString(), param["type"].GetString()});
      }
      deserialized_native.return_type = native.value["return_type"].GetString();
      if (native.value.HasMember("old_names")) {
        for (auto&& old_name : native.value["old_names"].GetArray()) {
          deserialized_native.old_names.emplace_back(old_name.GetString());
        }
      }

      natives.emplace_back(deserialized_native);
    }

    if (!std::filesystem::is_directory(cpp_out))
      std::filesystem::create_directories(cpp_out);
    if (!std::filesystem::is_directory(lua_out))
      std::filesystem::create_directories(lua_out);

    CorrectTypes(natives);

    ToCppFile(_namespace.name.GetString(), natives);
    ToLuaFile(_namespace.name.GetString(), natives);
  }

  void AllInclude() {
    auto out = cpp_out / "../natives.hpp";
    if (std::filesystem::is_regular_file(out))
      std::filesystem::remove(out);

    std::stringstream ss;
    ss << "#pragma once\n";
    for (auto&& file : std::filesystem::directory_iterator(cpp_out)) {
      if (file.path().extension() == ".hpp") {
        ss << "#include \"" << "native_def/" << file.path().filename().string() << "\"\n";
      }
    }

    std::ofstream writer(out);
    writer << ss.str();
    writer.close();
  }

  bool ConvertFile(const std::filesystem::path& path) {
    std::string file;
    std::string line;
    std::ifstream reader(path);
    while (std::getline(reader, line))
      file.append(line);

    rapidjson::Document json;
    if (json.Parse(file.c_str()).HasParseError()) {
      std::cerr << "rapidjson error: " << json.GetParseError() << std::endl;
      return false;
    }
    if (!json.IsObject()) {
      std::cerr << "json root is not a obj" << std::endl;
      return false;
    }

    std::cout << "Parsed json starting" << std::endl;
    {
      for (auto&& _namespace : json.GetObject())
        ConvertNamespace(_namespace);
    }
    std::cout << path << " has been converted" << std::endl;

    AllInclude();

    return true;
  }
}