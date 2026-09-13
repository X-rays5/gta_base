//
// Created by X-ray on 29/12/2023.
//

#include "manager.hpp"
#include <base-common/fs/vfs.hpp>
#include <glaze/glaze.hpp>
#include <enchantum/enchantum.hpp>

namespace base::menu::ui::localization {
    namespace {
        std::string GetProfilePath(const std::string& name) {
            return fmt::format("{}/{}.json", common::fs::vfs::GetTranslationDir(), name);
        }
    }

    ScriptTranslations::Snapshot ScriptTranslations::Current() const {
        return state_.load(std::memory_order_acquire);
    }

    std::shared_ptr<ScriptTranslations::State> ScriptTranslations::CopyForWrite() const {
        // The load is inside the copy rather than taken from the caller's snapshot: a writer must copy
        // the state as it is at the moment of the copy, and only one writer is here at a time.
        return std::make_shared<State>(*Current());
    }

    void ScriptTranslations::Publish(std::shared_ptr<const State> next) {
        // Release, paired with the acquire of Current: what a reader takes by loading this pointer is a
        // state that is fully built and that nothing will write to again.
        state_.store(std::move(next), std::memory_order_release);
    }

    void ScriptTranslations::DropFromOwnerIndex(State& state, const std::string& owner, const std::string_view key) {
        const auto owned = state.keys_by_owner.find(owner);
        if (owned == state.keys_by_owner.end()) {
            return;
        }

        std::erase(owned->second, key);
        if (owned->second.empty()) {
            state.keys_by_owner.erase(owned);
        }
    }

    bool ScriptTranslations::Register(std::string key, std::string value, std::string owner) {
        // Neither an unnamed key nor an unowned string is something that could be found again or removed
        // again, and the second of those is what a script's unload depends on.
        if (key.empty() || owner.empty()) {
            return false;
        }

        std::lock_guard lock(write_);
        std::shared_ptr<State> next = CopyForWrite();

        if (const auto existing = next->by_key.find(key); existing != next->by_key.end()) {
            if (existing->second.owner != owner) {
                return false;
            }

            // The script's own key, registered again - after a reload, or twice in one init. The value
            // is replaced rather than refused: what a script means by registering a key twice is that
            // this is what the string says now. The index needs nothing, since the key is already in it.
            existing->second.value = std::move(value);
            Publish(std::move(next));
            return true;
        }

        // Indexed under the key as it was given, which is the same string the map now holds a copy of:
        // this is the one place a key is written down, so the two cannot drift apart.
        next->keys_by_owner[owner].push_back(key);
        next->by_key.emplace(std::move(key), ScriptTranslation{std::move(value), std::move(owner)});
        Publish(std::move(next));
        return true;
    }

    bool ScriptTranslations::Remove(const std::string_view key, const std::string_view owner) {
        const std::string owner_key{owner};
        std::lock_guard lock(write_);

        const Snapshot current = Current();
        const auto it = current->by_key.find(key);
        if (it == current->by_key.end() || it->second.owner != owner) {
            // Not there, or not this script's to remove. Reported rather than silently done, since the
            // two are the same answer to a script that asked whether it had removed its own string.
            return false;
        }

        std::shared_ptr<State> next = std::make_shared<State>(*current);
        next->by_key.erase(key);
        DropFromOwnerIndex(*next, owner_key, key);
        Publish(std::move(next));
        return true;
    }

    void ScriptTranslations::RemoveOwnedBy(const std::string_view owner) {
        const std::string owner_key{owner};
        std::lock_guard lock(write_);

        const Snapshot current = Current();
        const auto owned = current->keys_by_owner.find(owner_key);
        if (owned == current->keys_by_owner.end()) {
            // Nothing of this script's was ever registered, so there is nothing to publish either - which
            // is the ordinary case for a script that had no strings.
            return;
        }

        std::shared_ptr<State> next = std::make_shared<State>(*current);
        for (const auto& key : owned->second) {
            next->by_key.erase(key);
        }

        next->keys_by_owner.erase(owner_key);
        Publish(std::move(next));
    }

    bool ScriptTranslations::TryGet(const std::string_view key, std::string& out) const {
        // The snapshot is taken once and held to the end of the call, so what is read out of it here
        // cannot be freed by a writer publishing over it: the writer's state is a new object, and this
        // one is still ours.
        const Snapshot snapshot = Current();

        // The fast path for the ordinary case: with no script string registered at all - which is every
        // frame of a menu whose scripts register none - this is the whole of the lookup.
        if (snapshot->by_key.empty()) {
            return false;
        }

        const auto it = snapshot->by_key.find(key);
        if (it == snapshot->by_key.end()) {
            return false;
        }

        out = it->second.value;
        return true;
    }

    std::size_t ScriptTranslations::Count() const {
        return Current()->by_key.size();
    }

    bool Manager::RegisterScriptTranslation(std::string key, std::string value, const std::string owner) {
        // The menu's own keys first, and refused here rather than in the overlay: the overlay is what a
        // script may add to, and what the menu ships is not that. The loaded translation is what is
        // asked, not the default one, because that is the map Localize answers from.
        if (translation_.loaded_translation_.contains(key)) {
            return false;
        }

        return script_translations_.Register(std::move(key), std::move(value), std::move(owner));
    }

    bool Manager::RemoveScriptTranslation(const std::string_view key, const std::string_view owner) {
        return script_translations_.Remove(key, owner);
    }

    void Manager::RemoveScriptTranslationsOwnedBy(const std::string_view owner) {
        script_translations_.RemoveOwnedBy(owner);
    }

    std::size_t Manager::ScriptTranslationCount() const {
        return script_translations_.Count();
    }

    Status Translation::Load(const std::string& name) {
        std::string json_buffer;
        translation_map_t tmp_translation;

        std::string profile_path = GetProfilePath(name);
        if (!std::filesystem::is_regular_file(profile_path)) {
            LOG_INFO("Requested translation file '{}' doesn't exist, loading default translation", name);
            WriteDefaultTranslation();
            profile_path = GetProfilePath("default");
        }

        if (const auto ec = glz::read_file_json(tmp_translation, profile_path, json_buffer)) {
            return MakeFailure<ResultCode::kIO_ERROR>(glz::format_error(ec, json_buffer));
        }

        auto res = Merge(tmp_translation);
        if (res.error()) {
            LOG_ERROR(res);
        }

        return res;
    }

    Status Translation::Save(const std::string& name) {
        if (const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(loaded_translation_, GetProfilePath(name), std::string{})) {
            return MakeFailure<ResultCode::kIO_ERROR>(std::string(enchantum::to_string(ec.ec)));
        }

        return {};
    }

    Status Translation::Merge(translation_map_t tmp_translation) {
        auto merged = glz::merge{loaded_translation_, tmp_translation};

        std::string tmp_buff;
        auto ec = glz::write_json(merged, tmp_buff);
        if (ec) {
            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to merge default and loaded translation files");
        }

        ec = glz::read_json(loaded_translation_, tmp_buff);
        if (ec) {
            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to merge default and loaded translation files");
        }

        return {};
    }

    void Translation::WriteDefaultTranslation() {
        if (const auto ec = glz::write_file_json<glz::opts{.prettify = true}>(default_translation, GetProfilePath("default"), std::string{})) {
            LOG_ERROR("Failed to write default translation to disk: {}", enchantum::to_string(ec.ec));
        }
    }

    Manager::Manager() {
        if (const auto status = translation_.Load("default"); status.error()) {
            LOG_CRITICAL("Failed to load translation from disk: {}", status.error());
        }

        kMANAGER = this;
    }

    Manager::~Manager() {
        kMANAGER = nullptr;

        if (const auto status = translation_.Save(active_translation_); status.error()) {
            LOG_ERROR("Failed to save active translation to disk: {}", status.error());
        }
    }

    std::string Manager::Localize(const std::string_view key) {
        // A script's string first. The two cannot hold the same key - Register refuses a key the loaded
        // translation has - so the order is not a precedence between two answers but the shortest way to
        // the only answer there is.
        if (std::string script_string; script_translations_.TryGet(key, script_string)) {
            return script_string;
        }

        const auto it = translation_.loaded_translation_.find(key);
        return std::string(it != translation_.loaded_translation_.end() ? it->second : key);
    }

    Status Manager::SetActiveTranslation(const std::string& name, const bool save_current) {
        if (save_current && translation_.Save(name).error()) {
            LOG_ERROR("Failed to save current translation");

            return MakeFailure<ResultCode::kINTERNAL_ERROR>("Unable to save current translation file");
        }

        Status res = translation_.Load(name);
        if (res.error()) {
            LOG_ERROR("Failed to load translation '{}': {}", name, res.error());
            return res;
        }

        active_translation_ = name;
        return res;
    }
}
