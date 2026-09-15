//
// Created by X-ray on 12/09/2026.
//

#ifndef GLAZE_GENERIC_TREE_HPP_9B3E5F1C4A7D2E60
#define GLAZE_GENERIC_TREE_HPP_9B3E5F1C4A7D2E60
#include <glaze/glaze.hpp>

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace base::menu::util {
  /// Which serialisation a node is read or written with. One enum rather than two method names, so
  /// that a save and a load share a signature and a script can pick the format at run time.
  enum class ValueFormat {
    kJSON,
    kTOML,
  };

  /// One step from a tree's root towards a node: a member of an object, or an element of an array.
  /// A variant rather than two parallel vectors because a path is a mixture of the two.
  using PathElement = std::variant<std::string, std::size_t>;
  using Path = std::vector<PathElement>;

  /**
   * A JSON/TOML document a script builds and reads by path, together with the one directory it is
   * allowed to save into.
   *
   * The directory is handed in rather than looked up here, so that the tree itself knows nothing
   * about scripts and can be tested against a temporary directory; the script binding passes
   * vfs::GetScriptDataDir(<script name>), and is what makes "any path a script names resolves inside
   * its own directory" true.
   *
   * Nodes are addressed by path rather than by pointer, and that is not a convenience. glz::generic's
   * object type is ordered_small_map, which keeps its members in a single flat block: inserting *any*
   * member can move *every* member, so a pointer taken before an unrelated insertion can point at
   * another member afterwards. A path survives that, a pointer does not - see GenericView.
   */
  class GenericTree {
  public:
    explicit GenericTree(std::filesystem::path dir);

    GenericTree(const GenericTree&) = delete;
    GenericTree(GenericTree&&) = delete;
    GenericTree& operator=(const GenericTree&) = delete;
    GenericTree& operator=(GenericTree&&) = delete;

    [[nodiscard]] const std::filesystem::path& GetDir() const {
      return dir_;
    }

    /**
     * The node `path` leads to, or null when the walk stops short - a member of something that is
     * not an object, an element of something that is not an array, an element past the end, or a
     * member that is not there. An empty path is the root, which is always there.
     */
    [[nodiscard]] glz::generic* Find(const Path& path);

    /**
     * The node `path` leads to, making it on the way if it is not there yet: a member of something
     * that is not an object replaces that node with an object, an element of something that is not
     * an array replaces it with an array, and an element past the end grows the array. This is what
     * makes a write to root["a"]["b"] land in an empty document.
     *
     * A node that has to be replaced is replaced in place, and the next step is resolved after that
     * - so no pointer into a container is held across a write to it.
     */
    glz::generic& FindOrCreate(const Path& path);

    /// Drops the node `path` leads to out of its parent, and reports whether there was one to drop.
    /// The root itself is reset rather than erased.
    bool Erase(const Path& path);

  private:
    glz::generic root_;
    std::filesystem::path dir_;
  };

  /**
   * A handle to one node of a GenericTree: the tree, plus the path from its root to that node.
   *
   * It holds no pointer to the node, and re-resolves the path on every call. That is the whole point
   * of the class - see GenericTree - so it must not be "optimised" later by resolving once and
   * keeping the result. Recomputing is the correctness, not the overhead.
   *
   * It follows too that a handle outlives the shape it was made against: a handle made while the node
   * was an object keeps working after something replaces it with a number. Reads of a node that is
   * no longer the type they wanted hand back their fallback, and writes coerce the node to the type
   * written.
   *
   * A const handle can still write, and that is deliberate: the handle's own state is its tree and
   * its path, and no operation changes either. What a write changes belongs to the document the
   * handle points into, which is shared - the same thing a const shared_ptr to a mutable object
   * means.
   */
  class GenericView {
  public:
    GenericView(std::shared_ptr<GenericTree> tree, Path path);

    GenericView(const GenericView&) = default;
    GenericView(GenericView&&) = default;
    GenericView& operator=(const GenericView&) = default;
    GenericView& operator=(GenericView&&) = default;
    ~GenericView() = default;

    /// The handle to the member `key` of this node, and to element `index` of it. Neither looks the
    /// node up - that happens when the returned handle is used - so either may be taken on a node
    /// that will only later be given that shape.
    [[nodiscard]] GenericView Key(std::string key) const;
    [[nodiscard]] GenericView Index(std::size_t index) const;

    [[nodiscard]] const std::shared_ptr<GenericTree>& GetTree() const {
      return tree_;
    }

    /// The shape of this node, for a script that has to tell an array from a string.
    ///
    /// Exactly one of the six is true for any handle: a node that is not there at all is null, since
    /// what a script asking about one wants to know is that there is nothing there yet. Whether a
    /// node is there at all, as against there and empty, is Contains and Size.
    [[nodiscard]] bool IsNull() const;
    [[nodiscard]] bool IsObject() const;
    [[nodiscard]] bool IsArray() const;
    [[nodiscard]] bool IsNumber() const;
    [[nodiscard]] bool IsBool() const;
    [[nodiscard]] bool IsString() const;

    /// Reads, each falling back to `fallback` when the node is missing or holds another type. A
    /// number is read from any of the three number types, so a value that went in as an integer
    /// still reads back after a round trip through a format that writes it as a float.
    [[nodiscard]] bool ReadBool(bool fallback = false) const;
    [[nodiscard]] std::int32_t ReadInt(std::int32_t fallback = 0) const;
    [[nodiscard]] std::int64_t ReadInt64(std::int64_t fallback = 0) const;
    [[nodiscard]] float ReadFloat(float fallback = 0.0f) const;
    [[nodiscard]] double ReadDouble(double fallback = 0.0) const;
    [[nodiscard]] std::string ReadString(std::string fallback = {}) const;

    /// Writes, each making the node on the way and replacing whatever was there. Note that a number
    /// is stored as a double: glz::generic is the f64 mode, so a value past 2^53 does not come back
    /// exact. The alternative was a separate integer mode, which would have made JSON and TOML
    /// disagree about the same document.
    void WriteBool(bool value) const;
    void WriteInt(std::int32_t value) const;
    void WriteInt64(std::int64_t value) const;
    void WriteFloat(float value) const;
    void WriteDouble(double value) const;
    void WriteString(std::string_view value) const;
    void SetNull() const;

    /// Whether this node is an object with a member `key`. False for a node of any other shape, so
    /// a script can ask before subscripting without a separate type test.
    [[nodiscard]] bool Contains(const std::string& key) const;

    /// How many members or elements this node has, or how many characters it holds when it is a
    /// string. Zero for a node that is a number, a boolean or null.
    [[nodiscard]] std::size_t Size() const;

    /// The member names of this node, in the order they were added - which is the order a JSON or
    /// TOML document writes them in, so reading them back preserves a file's layout. Empty for
    /// anything that is not an object.
    [[nodiscard]] std::vector<std::string> Keys() const;

    /// Drops this node out of its parent. Returns whether there was one to drop.
    bool Erase() const;

    /// The array this node is, for the script layer's conversion to and from a script array - the
    /// element types are a script's business, not the tree's. Both return null / coerce the node
    /// when it is not an array, so a caller that means to build one should use MakeArray.
    [[nodiscard]] glz::generic::array_t* GetArray() const;
    glz::generic::array_t& MakeArray() const;

    /// The object this node is, for conversion to and from a script dictionary, on the same terms.
    [[nodiscard]] glz::generic::object_t* GetObject() const;
    glz::generic::object_t& MakeObject() const;

    /// Writes this node to `file_name`, relative to the tree's directory, as JSON or TOML.
    ///
    /// A name that resolves outside that directory - with "..", or by being absolute - is refused
    /// and nothing is written, not even the directories it would have needed. TOML has no
    /// representation for a document whose root is not a table, so saving such a node as TOML is
    /// refused too rather than written as something that will not read back.
    bool Save(const std::string& file_name, ValueFormat format) const;

    /// Reads `file_name`, relative to the tree's directory, into this node. The node is left as it
    /// was when the file cannot be read or does not parse, so a failed load never half-applies.
    bool Load(const std::string& file_name, ValueFormat format) const;

    /// This node as text, and the reverse. Both are the in-memory half of Save and Load, and are
    /// what a script uses to hand a document to something else rather than to a file.
    [[nodiscard]] std::string Dump(ValueFormat format) const;
    bool Parse(std::string_view text, ValueFormat format) const;

  private:
    /// Deliberately not public, and deliberately not cached: handing a pointer out would invite
    /// holding it across a write, which is the one thing this class exists to make unnecessary.
    [[nodiscard]] glz::generic* Node() const;

    std::shared_ptr<GenericTree> tree_;
    Path path_;
  };
}

#endif //GLAZE_GENERIC_TREE_HPP_9B3E5F1C4A7D2E60
