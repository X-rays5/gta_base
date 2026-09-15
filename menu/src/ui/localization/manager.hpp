//
// Created by X-ray on 29/12/2023.
//

#ifndef MANAGER_HPP_12190029
#define MANAGER_HPP_12190029
#include <ankerl/unordered_dense.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#define TRANSLATE_LITERAL(str) base::ui::localization::kMANAGER->Localize(xorstr_(str));

namespace base::menu::ui::localization {
  struct TransparentHash {
    using is_transparent = void; // enables heterogeneous lookup

    size_t operator()(const std::string& key) const noexcept {
      return ankerl::unordered_dense::hash<std::string>{}(key);
    }

    size_t operator()(const std::string_view key) const noexcept {
      return ankerl::unordered_dense::hash<std::string_view>{}(key);
    }
  };

  struct TransparentEqual {
    using is_transparent = void;

    // std::string vs std::string
    bool operator()(const std::string& lhs, const std::string& rhs) const noexcept {
      return lhs == rhs;
    }

    // std::string vs string_view
    bool operator()(const std::string& lhs, const std::string_view rhs) const noexcept {
      return lhs == rhs;
    }

    bool operator()(const std::string_view lhs, const std::string& rhs) const noexcept {
      return lhs == rhs;
    }

    // string_view vs string_view
    bool operator()(const std::string_view lhs, const std::string_view rhs) const noexcept {
      return lhs == rhs;
    }
  };

  using translation_map_t = ankerl::unordered_dense::map<std::string, std::string, TransparentHash, TransparentEqual>;

  extern const translation_map_t default_translation;

  struct Translation {
    translation_map_t loaded_translation_ = default_translation;

    [[nodiscard]] Status Load(const std::string& name);

    [[nodiscard]] Status Save(const std::string& name);

    [[nodiscard]] Status Merge(translation_map_t tmp_translation);

    static void WriteDefaultTranslation();
  };

  /**
   * One string a script registered, and the script that did.
   *
   * The owner is kept beside the value rather than only in the index below, so that every key carries
   * what it takes to decide who may remove it.
   */
  struct ScriptTranslation {
    std::string value;
    std::string owner;
  };

  using script_translation_map_t = ankerl::unordered_dense::map<std::string, ScriptTranslation, TransparentHash, TransparentEqual>;

  /**
   * Every string the loaded scripts have registered, which is the whole of what a script may add to the
   * localization system.
   *
   * Deliberately not a Translation, and never merged into one: a Translation is what Save writes to
   * disk, so a script's strings living anywhere else is the mechanism by which they are never saved.
   * That is a property of where the data is kept rather than of what any writer remembers not to do,
   * and a script cannot break it however it registers.
   *
   * Read without a lock. Localize is called for every piece of text of every component of every frame,
   * so the read side is a single atomic load of a snapshot that nobody ever writes to again: a write
   * copies the state, changes the copy and publishes it. Writers are rare - a script registers its
   * strings once, when it loads - so paying for the copy there is the right way round.
   */
  class ScriptTranslations {
  public:
    /**
     * Registers one string, owned by `owner`. True when the key is now the script's.
     *
     * False when the key is already another script's: the first script to take a key keeps it, since the
     * alternative is one script silently reading another's text. A script registering its own key again -
     * after a reload, or twice in one init - updates the value instead, which is what registering it
     * again must mean. Whether the key is one of the menu's own is not this class's to answer; the
     * Manager is what knows the built-in translation.
     */
    bool Register(std::string key, std::string value, std::string owner);

    /**
     * Takes away the string registered under `key`, which must be `owner`'s. False when there is no such
     * key or when it belongs to somebody else, so that a script cannot remove another's string by naming
     * it - and so that a second Remove is harmless.
     */
    bool Remove(std::string_view key, std::string_view owner);

    /**
     * Takes away every string `owner` registered. This is what a script's own destruction calls, since a
     * script that unloads is not one that tidies up after itself.
     */
    void RemoveOwnedBy(std::string_view owner);

    /**
     * The string registered under `key` into `out`, and whether there was one. The lookup writes into a
     * caller-owned string rather than returning one, so that a miss - which is the ordinary case, and
     * the one every built-in key takes - does not build a string to throw away.
     */
    bool TryGet(std::string_view key, std::string& out) const;

    /// How many strings are registered. Diagnostics and tests.
    [[nodiscard]] std::size_t Count() const;

  private:
    /**
     * What a reader sees. Both halves are published together: the index exists so that a script's own
     * strings can be found again without walking every key, and a reader never needs it at all.
     */
    struct State {
      script_translation_map_t by_key;
      std::unordered_map<std::string, std::vector<std::string>> keys_by_owner;
    };

    using Snapshot = std::shared_ptr<const State>;

    [[nodiscard]] Snapshot Current() const;

    /// A copy of the state as it is now, for a writer to change. Called with write_ held.
    [[nodiscard]] std::shared_ptr<State> CopyForWrite() const;

    /// Hands the state to readers. Called with write_ held, and last on any path that changed it.
    void Publish(std::shared_ptr<const State> next);

    /// Drops `key` from `owner`'s entry of the index, and the entry with it when it becomes empty.
    static void DropFromOwnerIndex(State& state, const std::string& owner, std::string_view key);

    std::atomic<Snapshot> state_{std::make_shared<const State>()};

    /**
     * Serialises the writers. A plain mutex rather than a spinlock, because a writer is a script
     * registering its strings once while the readers it is publishing to take no lock at all.
     */
    mutable std::mutex write_;
  };

  class Manager {
  public:
    /**
     * A script's string, which is registered only if the menu has no string of its own under that key.
     *
     * `owner` is the script that made it, by name: it is what removing the string matches on, and what
     * makes one script's key not another's to take. False when the menu already ships that key or when
     * another script holds it.
     */
    bool RegisterScriptTranslation(std::string key, std::string value, std::string owner);

    /// The script's string, removed again. False when it is not that script's to remove.
    bool RemoveScriptTranslation(std::string_view key, std::string_view owner);

    /// Every string the named script registered, removed. What a script's destruction calls.
    void RemoveScriptTranslationsOwnedBy(std::string_view owner);

    /// How many script strings are registered. Diagnostics and tests.
    [[nodiscard]] std::size_t ScriptTranslationCount() const;

    Manager();

    ~Manager();

    [[nodiscard]] std::string Localize(std::string_view key);
    [[nodiscard]] Status SetActiveTranslation(const std::string& name, bool save_current = true);

  private:
    Translation translation_;
    std::string active_translation_ = "default";

    /**
     * A script's strings, kept beside the loaded translation and never part of it - see
     * ScriptTranslations for why that is the whole of what keeps them off disk.
     */
    ScriptTranslations script_translations_;
  };

  inline Manager* kMANAGER{};
}

namespace base::menu {
  [[nodiscard]] inline std::string operator ""_l10n(const char* key) {
    return ui::localization::kMANAGER->Localize(key);
  }
}


#endif //MANAGER_HPP_12190029
