//
// Created by X-ray on 12/09/2026.
//

#include "as_game_task.hpp"

#include "../script/as_script_context.hpp"
#include "../script/as_script_manager.hpp"
#include "../../script/game_task_executor.hpp"
#include "../../script/general_task_executor.hpp"
#include "../../script/task_executor.hpp"
#include "../util/as_bind.hpp"

#include <minicoropp/coroutine.hpp>

#include <memory>
#include <string>
#include <string_view>

// `thread::queue_game_task(function@)` and `thread::queue_general_task(function@)`, which queue a
// function of the script onto one of the two task threads: the game's, ticked by the game every frame,
// or the general one, which has a loop of its own and parks while it has nothing to do.
//
// A task body is a coroutine to itself, ticked by whichever executor it was queued onto rather than by
// the script that queued it, and it keeps the thread:: calls the script already knows: the executor
// resumes the task body once per pass, and the body turns each of those turns into one Tick() of a
// ScriptContext. A thread::yield/sleep/suspend inside the task is then recorded by that context
// exactly as it would be in GameTick, and the waiting is done by the task body yielding to the
// executor instead of the frame - which for the general thread is what lets it park.
//
// The engine the function belongs to has to outlive it, and unloading a script is the one thing that
// could take it away mid-task. The task therefore holds a weak handle to the script and locks it for
// its whole life: an unloaded script's task lets go and returns rather than touching a freed engine,
// and a running task keeps its script alive until it is done. With a general task the two ends are
// genuinely concurrent - the unload runs on the game thread - so that reference is the only thing
// holding the script up, not a tidiness the ordering happened to give.
//
// What the task holds is the function's declaration rather than the function. The parameter is
// registered as an auto handle (`@+`), so the engine releases the reference the script handed over as
// soon as this call returns, and a pointer kept past it would name a function nothing holds - the
// whole queueing would be a use-after-free the first time a script was unloaded with a task still
// waiting. A declaration is resolved again in the module the task's own script owns, which is a
// lookup that can only answer while that script is alive, and which names one function exactly where
// a bare name would also match an overload.
namespace base::menu::as::bindings::game_task {
  namespace {
    using script::Script;
    using script::ScriptContext;
    using script::ScriptManager;

    /**
     * Hand `fn` to `executor`, whichever thread that runs on, and drive it there until it ends or its
     * script is unloaded.
     *
     * `binding` is the script-facing name of the call, for the log lines: the two natives are the same
     * code with a different queue at the end of it, so a message that named one of them would be wrong
     * half the time.
     */
    void QueueTaskOnto(AngelScript::asIScriptFunction* fn, menu::script::TaskExecutor* executor, const std::string_view binding) {
      if (!fn) {
        LOG_ERROR("[AS] {} called with a null function, which has no effect", binding);
        return;
      }

      if (!executor) {
        LOG_ERROR("[AS] {} called with no task executor, which has no effect", binding);
        return;
      }

      // Namespaces included, so that a function of one is found again from its declaration alone.
      const std::string declaration = fn->GetDeclaration(true, true);

      // The queueing script is the one running right now, and it is also the one that owns the engine
      // the function belongs to.
      std::weak_ptr<Script> owner;
      if (const std::string name = ScriptManager::RunningScriptName(); !name.empty() && script::kAS_SCRIPT_MANAGER) {
        if (const auto found = script::kAS_SCRIPT_MANAGER->GetScript(name); !found.has_error()) {
          owner = found.value();
        }
      }

      if (owner.expired()) {
        LOG_ERROR("[AS] {} called outside of a loaded script, which has no effect", binding);
        return;
      }

      executor->QueueTask([declaration, owner, binding] {
        const std::shared_ptr<Script> script_owner = owner.lock();
        if (!script_owner) {
          // Unloaded between the queueing and this first turn. Nothing of the script is left to run.
          return;
        }

        auto* fn = script_owner->GetFunctionByDecl(declaration);
        if (!fn) {
          // The module this was queued from is still loaded, so the function it named is gone from it;
          // there is nothing to run and nothing to report beyond that.
          LOG_ERROR("[AS] A task queued by {} cannot find '{}' in the script it was queued from", binding, declaration);
          return;
        }

        // Reached only while the script is alive, so this is the engine that keeps fn valid as well.
        ScriptContext context(fn->GetEngine());
        context.Run(fn, true);
        // So the task starts in the pass it was queued in rather than the one after.
        context.Tick();

        while (context.IsRunning() && !script_owner->IsUnloadRequested()) {
          minicoropp::this_coro::yield();
          context.Tick();
        }
      });
    }

    void QueueGameTask(AngelScript::asIScriptFunction* fn) {
      QueueTaskOnto(fn, menu::script::kGAME_TASK_EXECUTOR, "thread::queue_game_task");
    }

    void QueueGeneralTask(AngelScript::asIScriptFunction* fn) {
      QueueTaskOnto(fn, menu::script::kGENERAL_TASK_EXECUTOR, "thread::queue_general_task");
    }
  }

  void RegisterGameTask(AngelScript::asIScriptEngine* engine) {
    engine->SetDefaultNamespace("thread");

    util::RegisterFuncdef(engine, "void TaskFunc()")
      .Desc("The shape of a function thread::queue_game_task() or thread::queue_general_task() can "
            "run: no arguments, no return value. Any global function of that shape can be passed to "
            "one of them, as thread::queue_game_task(@myFunction).");

    // Registered as an auto handle (`@+`) so that the engine releases the reference the script handed
    // over when this call returns: what is kept of the function is its declaration, not a reference to
    // it, and an ordinary handle parameter would leave one outstanding for the life of the module.
    util::RegisterGlobalFunction(engine, "void queue_game_task(TaskFunc@+ fn)",
                                 AngelScript::asFUNCTION(QueueGameTask), AngelScript::asCALL_CDECL)
      .Desc("Queues `fn` to run as a game task, independently of the script's own GameTick. The call "
            "itself only hands the function over: the task starts on the game task executor's next "
            "tick, on the game thread, which is what makes this safe to call from anywhere a script "
            "runs. From then on it is ticked once per frame, and a thread::yield, thread::sleep or "
            "thread::suspend inside it costs the game nothing while it waits - the same as it would in "
            "GameTick. A task cannot outlive the script that queued it: unloading the script stops it "
            "at the next tick.")
      .Param("fn", "The function to run, passed as thread::queue_game_task(@myFunction).")
      .Returns("Nothing. The task has no handle to wait on; a script that needs to know when one "
               "finished can set a global from it.");

    util::RegisterGlobalFunction(engine, "void queue_general_task(TaskFunc@+ fn)",
                                 AngelScript::asFUNCTION(QueueGeneralTask), AngelScript::asCALL_CDECL)
      .Desc("Queues `fn` to run on the general thread, which is neither the game's nor the menu's. It "
            "is a thread of its own, so a task that waits costs nothing at all while it waits, and it is "
            "where work that is not the game's business belongs: a wait, a load, a computation, anything "
            "that would cost frames on the game thread. It starts on that thread's next pass - "
            "immediately, if the thread is idle and has to be woken for it - and from then on it is "
            "ticked once per pass, which is what thread::yield, thread::sleep and thread::suspend make "
            "it wait for. What runs there is concurrent with the script's own GameTick, which is still "
            "being ticked by the game all the while, and neither the engine nor the object types "
            "registered here are safe to use from two threads at once - so a task must not touch the "
            "script's handles, its globals or the game itself, and must reach the game through "
            "thread::queue_game_task instead. The one thing that runs on this thread alongside the "
            "script's own is a script's init(), which is there precisely because nothing else of the "
            "script has started yet. A task cannot outlive the script that queued it: unloading the "
            "script stops it at the next pass.")
      .Param("fn", "The function to run, passed as thread::queue_general_task(@myFunction).")
      .Returns("Nothing. The task has no handle to wait on; a script that needs to know when one "
               "finished can set a global from it.");

    // Back to the global namespace: a later registration must not land in `thread` by accident.
    engine->SetDefaultNamespace("");
  }
}
