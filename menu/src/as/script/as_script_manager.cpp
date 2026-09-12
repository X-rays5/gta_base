//
// Created by X-ray on 11/09/2026.
//

#include "as_script_manager.hpp"
#include "as_script.hpp"
#include "../../script/script_manager.hpp"
#include <base-common/fs/vfs.hpp>

#include <ranges>
#include <shared_mutex>
#include <utility>

namespace base::menu::as::script {
  namespace {
    std::vector<std::filesystem::path> GetAllManifestTomlInSubDirectories(const std::filesystem::path& dir) {
      std::vector<std::filesystem::path> metadata_files;
      for (const auto& entry : std::filesystem::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file() && entry.path().filename() == "manifest.toml") {
          metadata_files.push_back(entry.path());
        }
      }
      return metadata_files;
    }

    /**
     * The scripts' way into the game tick loop. It is a ScriptBase rather than a hook of its own so
     * that it is ticked by the same pass as the game task executor, on the same thread and in the
     * same order.
     */
    class ScriptTickHost final : public base::menu::script::ScriptBase {
    public:
      explicit ScriptTickHost(ScriptManager* manager) : ScriptBase("AngelScript"), manager_(manager) {}

      base::menu::script::ScriptBase::Type GetType() override {
        return Type::GameScript;
      }

    protected:
      void OnInit() override {}

      void OnTick() override {
        // Every script's engine is built and released through this call, which is what makes it the
        // game thread's business and nobody else's.
        manager_->TickScripts();
      }

    private:
      ScriptManager* manager_;
    };
  }

  ScriptManager::ScriptManager() {
    if (kAS_SCRIPT_MANAGER) {
      throw std::runtime_error("ScriptManager instance already exists");
    }

    AngelScript::asPrepareMultithread();

    kAS_SCRIPT_MANAGER = this;

    // Only registered when the game's script manager exists, which is what lets a manager be built in
    // a test without one. It is initialised before this one and shut down after it, so it is there
    // for both this call and the one in the destructor.
    if (base::menu::script::kSCRIPT_MANAGER) {
      host_id_ = base::menu::script::kSCRIPT_MANAGER->AddScript(std::make_unique<ScriptTickHost>(this));
    }
  }

  ScriptManager::~ScriptManager() {
    if (host_id_ && base::menu::script::kSCRIPT_MANAGER) {
      base::menu::script::kSCRIPT_MANAGER->RemoveScript(*host_id_);
      host_id_.reset();
    }

    {
      std::unique_lock lock(scripts_mutex_);

      // Stopped as well as released: a script that is being unloaded while a game task still runs it
      // is left alone from that task's next tick onwards.
      for (const auto& script : scripts_ | std::views::values) {
        if (script) {
          script->RequestUnload();
        }
      }

      // Hooks are off by the time this runs, so nothing is going to tick the queue empty. This is
      // therefore the one time a script is released off the game thread, and only at shutdown.
      scripts_.clear();
      pending_destruction_.clear();
    }

    kAS_SCRIPT_MANAGER = nullptr;

    AngelScript::asUnprepareMultithread();
  }

  StatusOr<std::weak_ptr<Script>> ScriptManager::LoadScript(const ScriptManifest& manifest) {
    const std::string name = manifest.GetName();
    if (name.empty()) {
      return MakeFailure<ResultCode::kINVALID_ARGUMENT>("Script manifest at '{}' has no name, was its metadata read?", manifest.GetPath().string());
    }

    // A missing main file would leave behind a script whose module never built anything usable.
    if (manifest.GetMainFile().empty()) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Script '{}' has no readable main file", name);
    }

    {
      std::shared_lock lock(scripts_mutex_);
      if (scripts_.contains(name)) {
        return MakeFailure<ResultCode::kALREADY_EXISTS>("Script with name '{}' is already loaded", name);
      }
    }

    std::shared_ptr<Script> script;
    try {
      // Built outside the map's lock, because building a script means building an engine, which is
      // far too much work to hold a lock that a tick pass may want shared access to. The lock here
      // guards the documentation registry that comes with those bindings.
      const std::scoped_lock construction_guard(construction_mutex_);

      // shared_ptr rather than make_shared: the constructor is protected, and make_shared would have
      // to name it itself, which friendship does not reach into.
      script = std::shared_ptr<Script>(new Script(manifest));
    } catch (const std::exception& e) {
      // A script that does not compile is a load that failed, not a script that sits there loaded and
      // then throws on its first tick.
      return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to load script '{}': {}", name, e.what());
    }

    if (!script->IsValid()) {
      return MakeFailure<ResultCode::kINTERNAL_ERROR>("Failed to load script '{}'", name);
    }

    std::unique_lock lock(scripts_mutex_);

    const auto [it, inserted] = scripts_.emplace(name, std::move(script));
    if (!inserted) {
      // Another loader built the same script in the meantime. The one that loses is released here, on
      // a pool thread, which is safe precisely because it was never ticked: it has no coroutine, and
      // so nothing of the game thread's in it.
      return MakeFailure<ResultCode::kALREADY_EXISTS>("Script with name '{}' is already loaded", name);
    }

    LOG_DEBUG("Loaded script '{}'", name);
    return std::weak_ptr(it->second);
  }

  StatusOr<std::weak_ptr<Script>> ScriptManager::GetScript(const std::string& name) {
    std::shared_lock lock(scripts_mutex_);

    const auto it = scripts_.find(name);
    if (it == scripts_.end()) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Script with name '{}' is not loaded", name);
    }

    return std::weak_ptr(it->second);
  }

  Status ScriptManager::UnloadScript(const std::string& name) {
    std::unique_lock lock(scripts_mutex_);

    const auto it = scripts_.find(name);
    if (it == scripts_.end()) {
      return MakeFailure<ResultCode::kNOT_FOUND>("Script with name '{}' is not loaded", name);
    }

    // Stopped before it is forgotten: the flag is what keeps the next pass from resuming a script
    // that is on its way out, which is the case that matters for a script that unloaded itself.
    it->second->RequestUnload();

    // Queued rather than released. This can be called from a script, which is running on the game
    // thread inside its own engine, or from the menu's thread pool - and only the game thread may
    // shut an engine down, because a script may be parked in one. The pass in TickScripts releases
    // it, by which time nothing is executing it.
    pending_destruction_.emplace_back(std::move(it->second));
    scripts_.erase(it);

    LOG_DEBUG("Unloaded script '{}'", name);
    return {};
  }

  std::vector<std::weak_ptr<Script>> ScriptManager::GetAllScripts() const {
    std::shared_lock lock(scripts_mutex_);

    std::vector<std::weak_ptr<Script>> scripts;
    scripts.reserve(scripts_.size());

    for (const auto& val : scripts_ | std::views::values) {
      if (val) {
        scripts.emplace_back(val);
      }
    }

    return scripts;
  }

  ScriptState ScriptManager::GetScriptState(const std::string& name) const {
    std::shared_lock lock(scripts_mutex_);

    const auto it = scripts_.find(name);
    if (it == scripts_.end() || !it->second) {
      return ScriptState::kNotLoaded;
    }

    // The state is read here rather than through a handle, so that the menu never holds a reference
    // that would make it the script's last owner - see GetScript.
    return it->second->GetState();
  }

  void ScriptManager::TickScripts() {
    std::vector<std::shared_ptr<Script>> snapshot = SnapshotScripts();

    for (const auto& script : snapshot) {
      script->Tick();
    }

    // Dropped before the queue is drained, so that a script unloaded from inside this pass is
    // released by the drain below rather than whenever this snapshot happens to go out of scope.
    snapshot.clear();

    DrainPendingDestruction();
  }

  void ScriptManager::DrainPendingDestruction() {
    std::vector<std::shared_ptr<Script>> pending;
    {
      std::unique_lock lock(scripts_mutex_);
      pending.swap(pending_destruction_);
    }

    // The engines are shut down here, outside the lock, on the game thread - the only thread that may
    // release a script that has a parked coroutine in it. A script a queued game task still holds is
    // kept alive by that task and released when it ends, which is on this thread as well.
    pending.clear();
  }

  std::vector<std::shared_ptr<Script>> ScriptManager::SnapshotScripts() const {
    std::shared_lock lock(scripts_mutex_);

    std::vector<std::shared_ptr<Script>> scripts;
    scripts.reserve(scripts_.size());

    for (const auto& val : scripts_ | std::views::values) {
      if (val) {
        scripts.push_back(val);
      }
    }

    return scripts;
  }

  std::string ScriptManager::RunningScriptName() {
    AngelScript::asIScriptContext* const context = AngelScript::asGetActiveContext();
    if (!context) {
      return {};
    }

    AngelScript::asIScriptFunction* const func = context->GetFunction();
    if (!func) {
      return {};
    }

    const char* const name = func->GetModuleName();
    return name ? std::string(name) : std::string{};
  }

  std::vector<ScriptManifest> ScriptManager::GetScriptsOnDisk() {
    const std::filesystem::path scripts_dir = common::fs::vfs::GetScriptsDir();
    std::vector<std::filesystem::path> manifest_toml = GetAllManifestTomlInSubDirectories(scripts_dir);

    std::vector<ScriptManifest> manifests;
    for (const auto& manifest_path : manifest_toml) {
      ScriptManifest manifest(manifest_path.parent_path());
      auto status = manifest.ReadMetaData();
      if (status.has_error()) {
        LOG_ERROR("Failed to read script metadata from manifest: {}. Error: {}", manifest_path, status.error());
      }

      manifests.push_back(manifest);
    }

    return manifests;
  }

  void ScriptManager::WriteAsPredefined() {
    Script::DumpAsPredefined(common::fs::vfs::GetScriptsDir() / "as.predefined");
  }
}
