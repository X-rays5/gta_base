//
// Created by X-ray on 12/09/2026.
//

#include "generic_tree.hpp"

#include <glaze/toml.hpp>

#include <base-common/fs/vfs.hpp>

#include <cmath>
#include <exception>
#include <system_error>
#include <utility>

namespace base::menu::util {
  namespace {
    /// The file `file_name` names inside the tree's directory, or an empty path when it names
    /// anywhere else.
    ///
    /// The test is on the two resolved paths, so "sub/../../outside.json" is caught as surely as
    /// "C:/outside.json" is - the latter because joining a path onto an absolute one *replaces* it
    /// rather than extending it, so what comes back is outside the directory by construction. It is
    /// done before anything is created, so a refused name leaves nothing behind, not even the
    /// directories it would have needed.
    std::filesystem::path ResolveFilePath(const GenericTree& tree, const std::string& file_name) {
      if (file_name.empty()) {
        LOG_ERROR("[AS] Refusing to resolve an empty file name");
        return {};
      }

      const auto& dir = tree.GetDir();
      const auto path = dir / file_name;

      if (!common::fs::vfs::EnsureIsWithinDirectory(dir, path)) {
        LOG_ERROR("[AS] Refusing to touch {}, which is outside of {}", path.string(), dir.string());
        return {};
      }

      return path;
    }
  }

  GenericTree::GenericTree(std::filesystem::path dir) : dir_(std::move(dir)) {}

  glz::generic* GenericTree::Find(const Path& path) {
    glz::generic* node = &root_;

    for (const auto& step : path) {
      if (const auto* key = std::get_if<std::string>(&step)) {
        if (!node->is_object()) {
          return nullptr;
        }

        auto& object = node->get_object();
        const auto it = object.find(*key);
        if (it == object.end()) {
          return nullptr;
        }

        node = &it->second;
        continue;
      }

      if (!node->is_array()) {
        return nullptr;
      }

      const auto index = std::get<std::size_t>(step);
      auto& array = node->get_array();
      if (index >= array.size()) {
        return nullptr;
      }

      node = &array[index];
    }

    return node;
  }

  glz::generic& GenericTree::FindOrCreate(const Path& path) {
    glz::generic* node = &root_;

    // Only the node being descended *into* is ever changed here, and the pointer held across that
    // change is to a node in that node's parent. A member of an object or an array element lives in
    // its parent's container, so changing what one of them holds - or inserting a sibling into the
    // container it holds - cannot move it. The one thing that does move every node is inserting into
    // a container, which only ever happens to the container the new pointer is taken from, after the
    // pointer to the node that owns it has been taken.
    for (const auto& step : path) {
      if (const auto* key = std::get_if<std::string>(&step)) {
        if (!node->is_object()) {
          *node = glz::generic::object_t{};
        }

        node = &(*node)[*key];
        continue;
      }

      if (!node->is_array()) {
        *node = glz::generic::array_t{};
      }

      const auto index = std::get<std::size_t>(step);
      auto& array = node->get_array();
      if (index >= array.size()) {
        array.resize(index + 1);
      }

      node = &array[index];
    }

    return *node;
  }

  bool GenericTree::Erase(const Path& path) {
    if (path.empty()) {
      root_.reset();
      return true;
    }

    // The parent is resolved through the ordinary walk, so that the erase below acts on a container
    // that is still the one the last step named.
    glz::generic* parent = Find(Path(path.begin(), path.end() - 1));
    if (!parent) {
      return false;
    }

    const auto& last = path.back();
    if (const auto* key = std::get_if<std::string>(&last)) {
      return parent->is_object() && parent->get_object().erase(*key) > 0;
    }

    if (!parent->is_array()) {
      return false;
    }

    const auto index = std::get<std::size_t>(last);
    auto& array = parent->get_array();
    if (index >= array.size()) {
      return false;
    }

    array.erase(array.begin() + static_cast<glz::generic::array_t::difference_type>(index));
    return true;
  }

  GenericView::GenericView(std::shared_ptr<GenericTree> tree, Path path)
      : tree_(std::move(tree)), path_(std::move(path)) {}

  GenericView GenericView::Key(std::string key) const {
    Path path = path_;
    path.emplace_back(std::move(key));
    return {tree_, std::move(path)};
  }

  GenericView GenericView::Index(std::size_t index) const {
    Path path = path_;
    path.emplace_back(index);
    return {tree_, std::move(path)};
  }

  glz::generic* GenericView::Node() const {
    return tree_->Find(path_);
  }

  bool GenericView::IsNull() const {
    const auto* node = Node();
    return !node || node->is_null();
  }

  bool GenericView::IsObject() const {
    const auto* node = Node();
    return node && node->is_object();
  }

  bool GenericView::IsArray() const {
    const auto* node = Node();
    return node && node->is_array();
  }

  bool GenericView::IsNumber() const {
    const auto* node = Node();
    return node && node->is_number();
  }

  bool GenericView::IsBool() const {
    const auto* node = Node();
    return node && node->is_boolean();
  }

  bool GenericView::IsString() const {
    const auto* node = Node();
    return node && node->is_string();
  }

  bool GenericView::ReadBool(const bool fallback) const {
    const auto* node = Node();
    if (!node || !node->is_boolean()) {
      return fallback;
    }

    return node->get_boolean();
  }

  std::int32_t GenericView::ReadInt(const std::int32_t fallback) const {
    const auto* node = Node();
    if (!node || !node->is_number()) {
      return fallback;
    }

    // Rounded rather than truncated towards zero, so that a value written by a script as an integer
    // and read back through a format that stores it as a float is not one short of itself.
    return static_cast<std::int32_t>(std::llround(node->get_number()));
  }

  std::int64_t GenericView::ReadInt64(const std::int64_t fallback) const {
    const auto* node = Node();
    if (!node || !node->is_number()) {
      return fallback;
    }

    return static_cast<std::int64_t>(std::llround(node->get_number()));
  }

  float GenericView::ReadFloat(const float fallback) const {
    const auto* node = Node();
    if (!node || !node->is_number()) {
      return fallback;
    }

    return static_cast<float>(node->get_number());
  }

  double GenericView::ReadDouble(const double fallback) const {
    const auto* node = Node();
    if (!node || !node->is_number()) {
      return fallback;
    }

    return node->get_number();
  }

  std::string GenericView::ReadString(std::string fallback) const {
    const auto* node = Node();
    if (!node || !node->is_string()) {
      return fallback;
    }

    return node->get_string();
  }

  void GenericView::WriteBool(const bool value) const {
    tree_->FindOrCreate(path_) = value;
  }

  void GenericView::WriteInt(const std::int32_t value) const {
    tree_->FindOrCreate(path_) = static_cast<double>(value);
  }

  void GenericView::WriteInt64(const std::int64_t value) const {
    tree_->FindOrCreate(path_) = static_cast<double>(value);
  }

  void GenericView::WriteFloat(const float value) const {
    tree_->FindOrCreate(path_) = static_cast<double>(value);
  }

  void GenericView::WriteDouble(const double value) const {
    tree_->FindOrCreate(path_) = value;
  }

  void GenericView::WriteString(const std::string_view value) const {
    tree_->FindOrCreate(path_) = value;
  }

  void GenericView::SetNull() const {
    tree_->FindOrCreate(path_) = nullptr;
  }

  bool GenericView::Contains(const std::string& key) const {
    const auto* node = Node();
    return node && node->contains(key);
  }

  std::size_t GenericView::Size() const {
    const auto* node = Node();
    return node ? node->size() : 0;
  }

  std::vector<std::string> GenericView::Keys() const {
    std::vector<std::string> keys;

    const auto* node = Node();
    if (!node || !node->is_object()) {
      return keys;
    }

    const auto& object = node->get_object();
    keys.reserve(object.size());
    for (const auto& member : object) {
      keys.push_back(member.first);
    }

    return keys;
  }

  bool GenericView::Erase() const {
    return tree_->Erase(path_);
  }

  glz::generic::array_t* GenericView::GetArray() const {
    auto* node = Node();
    return node ? node->get_if<glz::generic::array_t>() : nullptr;
  }

  glz::generic::array_t& GenericView::MakeArray() const {
    auto& node = tree_->FindOrCreate(path_);
    if (!node.is_array()) {
      node = glz::generic::array_t{};
    }

    return node.get_array();
  }

  glz::generic::object_t* GenericView::GetObject() const {
    auto* node = Node();
    return node ? node->get_if<glz::generic::object_t>() : nullptr;
  }

  glz::generic::object_t& GenericView::MakeObject() const {
    auto& node = tree_->FindOrCreate(path_);
    if (!node.is_object()) {
      node = glz::generic::object_t{};
    }

    return node.get_object();
  }

  bool GenericView::Save(const std::string& file_name, const ValueFormat format) const {
    const auto* node = Node();
    if (!node) {
      LOG_ERROR("[AS] Cannot save {}: this value does not lead to a node", file_name);
      return false;
    }

    const auto path = ResolveFilePath(*tree_, file_name);
    if (path.empty()) {
      return false;
    }

    // Refused rather than written as a document that would not read back: TOML has no way to spell a
    // document whose root is not a table, and a reader of one that is not rejects the file outright.
    // A document only has to reach this state deliberately - the first subscripted write makes the
    // root an object - so this is a mistake worth naming rather than papering over.
    if (format == ValueFormat::kTOML && !node->is_object()) {
      LOG_ERROR("[AS] Cannot save {} as TOML: a TOML document is a table, so a root that is not an object has no TOML "
                "form. Save it as JSON.",
                file_name);
      return false;
    }

    std::error_code dir_ec;
    std::filesystem::create_directories(path.parent_path(), dir_ec);
    if (dir_ec) {
      LOG_ERROR("[AS] Cannot create the directory for {}: {}", path.string(), dir_ec.message());
      return false;
    }

    // glz::generic grows its maps with std::length_error, and a C++ exception must never unwind
    // through the VM that reaches this from a script.
    try {
      const auto result = format == ValueFormat::kJSON ? glz::write_file_json(*node, path.string(), std::string{})
                                                       : glz::write_file_toml(*node, path.string(), std::string{});
      if (result) {
        LOG_ERROR("[AS] Failed to save {}: {}", path.string(), glz::format_error(result));
        return false;
      }
    } catch (const std::exception& e) {
      LOG_ERROR("[AS] Failed to save {}: {}", path.string(), e.what());
      return false;
    }

    return true;
  }

  bool GenericView::Load(const std::string& file_name, const ValueFormat format) const {
    const auto path = ResolveFilePath(*tree_, file_name);
    if (path.empty()) {
      return false;
    }

    // Not an error: a file a script has not written yet is what its first run loads, and the answer
    // the script acts on is the false this returns.
    if (!std::filesystem::is_regular_file(path)) {
      LOG_DEBUG("[AS] Cannot load {}: there is no such file", path.string());
      return false;
    }

    // Parsed into a document of its own first and moved in only once it has read cleanly, so that a
    // file that does not parse leaves this node as it was instead of half-replacing it.
    glz::generic parsed;
    try {
      const auto result = format == ValueFormat::kJSON ? glz::read_file_json(parsed, path.string(), std::string{})
                                                       : glz::read_file_toml(parsed, path.string(), std::string{});
      if (result) {
        LOG_ERROR("[AS] Failed to load {}: {}", path.string(), glz::format_error(result));
        return false;
      }
    } catch (const std::exception& e) {
      LOG_ERROR("[AS] Failed to load {}: {}", path.string(), e.what());
      return false;
    }

    tree_->FindOrCreate(path_) = std::move(parsed);
    return true;
  }

  std::string GenericView::Dump(const ValueFormat format) const {
    const auto* node = Node();
    if (!node) {
      return {};
    }

    if (format == ValueFormat::kTOML && !node->is_object()) {
      LOG_ERROR("[AS] Cannot write a root that is not an object as TOML. Write it as JSON.");
      return {};
    }

    std::string text;
    try {
      const auto result = format == ValueFormat::kJSON ? glz::write_json(*node, text) : glz::write_toml(*node, text);
      if (result) {
        LOG_ERROR("[AS] Failed to write this value: {}", glz::format_error(result));
        return {};
      }
    } catch (const std::exception& e) {
      LOG_ERROR("[AS] Failed to write this value: {}", e.what());
      return {};
    }

    return text;
  }

  bool GenericView::Parse(const std::string_view text, const ValueFormat format) const {
    glz::generic parsed;
    try {
      const auto result = format == ValueFormat::kJSON ? glz::read_json(parsed, text) : glz::read_toml(parsed, text);
      if (result) {
        LOG_ERROR("[AS] Failed to read this value: {}", glz::format_error(result));
        return false;
      }
    } catch (const std::exception& e) {
      LOG_ERROR("[AS] Failed to read this value: {}", e.what());
      return false;
    }

    tree_->FindOrCreate(path_) = std::move(parsed);
    return true;
  }
}
