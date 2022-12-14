//
// Created by X-ray on 12/14/22.
//

#include "script_file.hpp"
#include <fstream>
#include "opcodes.hpp"

namespace crossmap_gen::script {
  File::File(const std::filesystem::path& path) {
    if (!std::filesystem::is_regular_file(path)) {
      throw std::invalid_argument(path.string() + " doesn't exist");
    }

    file_path_ = path;
    ParseFile();
  }

  void File::ParseFile() {
    std::ifstream script_file(file_path_, std::ios::binary);

    // skip header
    char buffer[256];
    std::int64_t header_offset{};
    script_file.read(buffer, 4);
    if (std::string(buffer) == "RSC7") {
      header_offset = 0x10;
      script_file.seekg(header_offset, std::ios::cur);
    }
    script_file.seekg(3 * 4, std::ios::cur);

    script_file.read(buffer, 4);
    std::uint32_t code_blocks_offset = (*reinterpret_cast<std::uint32_t*>(buffer)) & 0xFFFFFF;
    script_file.seekg(2 * 4, std::ios::cur);

    script_file.read(buffer, 4);
    std::uint32_t code_length = *reinterpret_cast<std::uint32_t*>(buffer);
    script_file.seekg(3 * 4, std::ios::cur);

    script_file.read(buffer, 4);
    std::uint32_t native_count = *reinterpret_cast<std::uint32_t*>(buffer);
    script_file.seekg(4 * 4, std::ios::cur);

    script_file.read(buffer, 4);
    std::uint32_t native_offset = *reinterpret_cast<std::uint32_t*>(buffer) & 0xFFFFFF;

    script_file.seekg(native_offset + header_offset, std::ios::beg);

    LOG(INFO) << "Found " << native_count << " natives";

    std::vector<std::uint64_t> used_natives;
    for (std::uint32_t i = 0; i < native_count; i++) {
      script_file.read(buffer, 8);
      std::uint64_t val = *reinterpret_cast<std::uint64_t*>(buffer);
      std::uint64_t rotate = (code_length + i) % 64;

      std::uint64_t call_data = ((val << rotate) | (val >> (64 - rotate))) & 0xFFFFFFFFFFFFFFFF;
      used_natives.emplace_back(call_data);
    }

    std::uint64_t code_blocks = (code_length + 0x3FFF) >> 14;
    script_file.seekg(code_blocks_offset + header_offset, std::ios::beg);

    std::vector<std::uint32_t> code_offsets;
    for (std::uint64_t i = 0; i < code_blocks; i++) {
      script_file.read(buffer, 8);
      std::uint32_t val = (*reinterpret_cast<std::uint32_t*>(buffer) & 0xFFFFFF) + header_offset;
      code_offsets.emplace_back(val);
    }

    std::string bytecode;
    for (std::uint32_t i = 0; i < code_blocks; i++) {
      script_file.seekg(code_offsets[i], std::ios::beg);
      auto buf_size = (i + 1) * 0x4000 >= code_length && code_length % 0x4000 || 0x4000;
      auto buf = std::make_unique<char[]>(buf_size);
      script_file.read(buf.get(), buf_size);
      bytecode.append(buf.get(), buf_size);
    }

    std::vector<Native> native_call_data;
    std::uint32_t offset{};
    std::uint32_t last_offset{};
    while(offset < bytecode.size()) {
      auto cur_byte = bytecode[offset];
      if (cur_byte == opcodes::PUSH_CONST_U8) {
        offset += 1;
      } else if (cur_byte == opcodes::PUSH_CONST_U8_U8 || cur_byte == opcodes::LEAVE) {
        offset += 2;
      } else if (cur_byte == opcodes::PUSH_CONST_U8_U8_U8) {
        offset += 3;
      } else if (cur_byte == opcodes::PUSH_CONST_U32 || cur_byte == opcodes::PUSH_CONST_F) {
        offset += 4;
      } else if (cur_byte == opcodes::NATIVE) {
        std::uint8_t native_return_arg_and_data = bytecode[offset + 1];
        std::uint8_t native_index = (bytecode[offset + 2] << 8) | bytecode[offset + 3];
        auto offset_tmp = offset - last_offset;
        native_call_data.emplace_back(Native{native_index, last_offset > 0 ? offset_tmp : 0, native_return_arg_and_data});
        last_offset = offset;
        offset += 3;
      } else if (cur_byte == opcodes::ENTER) {
        offset += bytecode[offset + 4] + 4;
      } else if (cur_byte >= opcodes::ARRAY_U8 && cur_byte <= opcodes::IOFFSET_U8_STORE && cur_byte != opcodes::IOFFSET) {
        offset += 1;
      } else if (cur_byte >= opcodes::PUSH_CONST_S16 && cur_byte <= opcodes::ILE_JZ) {
        offset += 2;
      } else if (cur_byte >= opcodes::CALL && cur_byte <= opcodes::GLOBAL_U24) {
        offset += 3;
      } else if (cur_byte == opcodes::GLOBAL_U24_LOAD) {
        offset += 1 + bytecode[offset + 1] * 6;
      } else if (cur_byte)
    }
  }
}