//
// Created by X-ray on 12/09/2026.
//

#ifndef AS_VALUE_HPP_2F7B9C41E5A83D60
#define AS_VALUE_HPP_2F7B9C41E5A83D60
#include <angelscript.h>

#include <cstddef>
#include <cstdint>
#include <string>

#include "../../util/glaze/generic_tree.hpp"

namespace base::menu::as::bindings::value {
  /**
   * The script-facing `Value`: a name for one node of a document a script builds itself, and the one
   * way a script has of keeping structured data - across calls of its own, or in a file.
   *
   * It owns nothing but a GenericView, which is a tree and a path into it, so a Value is
   * indistinguishable from the node it names and taking one adds nothing to the tree. Every Value
   * naming a node of the same document names it in the same tree, which is what makes a write
   * through one visible through the others - and is why `root["a"]["b"] = 1` needs nothing kept
   * alive in between: each Value in the chain is the script's, and goes when the statement does.
   *
   * It is a *value* type rather than a reference type, and the engine's own language settings are
   * why. `asEP_DISALLOW_VALUE_ASSIGN_FOR_REF_TYPE` is on (see as_script.cpp), so a write through
   * anything the engine holds by handle - `root["a"] = 1`, or `root = 5` - refuses to compile at
   * all, to catch a `handle = value` written where `@handle = value` was meant. A value type is
   * written through directly, which is exactly what a subscript is for; and as it is passed and
   * copied by value, it needs no ADDREF/RELEASE and no reference count to be handed around, and
   * AngelScript destroys its own copies.
   */
  class ScriptValue {
  public:
    explicit ScriptValue(menu::util::GenericView view);
    ~ScriptValue();

    // AngelScript's copy of this type is the placement-new thunk registered as its copy constructor,
    // not a C++ copy: that thunk builds the copy from View(). Nothing here copies a ScriptValue.
    ScriptValue(const ScriptValue&) = delete;
    ScriptValue(ScriptValue&&) = delete;
    ScriptValue& operator=(const ScriptValue&) = delete;
    ScriptValue& operator=(ScriptValue&&) = delete;

    /// The node this Value names, which the bindings read and write through. Naming a node that is
    /// not there is an ordinary state rather than an error: every read answers with its fallback, and
    /// every write makes the node on the way to it.
    [[nodiscard]] const menu::util::GenericView& View() const {
      return view_;
    }

    /// A Value naming the member `key` of this node, or its element `index`. Neither looks the node
    /// up, and neither writes anything, so either may be taken before the node has been given the
    /// shape it implies - root["a"]["b"] = 1 builds both levels, and builds them in that order.
    [[nodiscard]] ScriptValue Subscript(std::string key) const;
    [[nodiscard]] ScriptValue Subscript(std::size_t index) const;

    /// How many Values exist right now.
    ///
    /// AngelScript destroys a temporary at the end of the statement that made it, so a script that
    /// subscripts in a loop and keeps none of them must leave nothing behind - a leak here would be
    /// one object per subscript, every frame. This is how a test holds that to be true rather than
    /// taking it on trust.
    [[nodiscard]] static std::int64_t LiveInstances();

  private:
    menu::util::GenericView view_;
  };

  /// Binds `Value` and the `ValueFormat` it is saved and loaded with.
  void RegisterValue(AngelScript::asIScriptEngine* engine);
}

#endif //AS_VALUE_HPP_2F7B9C41E5A83D60
