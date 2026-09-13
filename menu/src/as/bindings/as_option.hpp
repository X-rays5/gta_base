//
// Created by X-ray on 12/09/2026.
//

#ifndef AS_OPTION_HPP_5D2A8F31C74B6E90
#define AS_OPTION_HPP_5D2A8F31C74B6E90
#include <angelscript.h>

#include <atomic>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "../../options/base_option.hpp"
#include "../../options/command_line.hpp"
#include "../script/as_script.hpp"

namespace base::menu::as::bindings::option {
  /**
   * An option a script registered: the arguments it declares, and the callback that runs them.
   *
   * It is an ordinary BaseOption, so the registry, the hotkey manager and everything else that already
   * works on an option works on it unchanged - the only thing that differs is where its `execute` gets
   * its work from, which is a function of the script rather than a C++ body. Declaring the arguments in
   * GetArgs() rather than by hand inside a parser is what gives it a command line and help text both,
   * since command_line.cpp writes the two from that one declaration.
   *
   * The owner is held weakly and the callback as its declaration. Both follow from the script going
   * away: the registry holds the option strongly and releases it only when the script unloads, but a
   * handle a script kept - or the array a script got from Options::All() - outlives that, and an option
   * whose engine has been shut down must do nothing at all rather than reach into it. A declaration is
   * resolved again in the module the script still owns, which can only answer while it is loaded, and
   * holds no reference to a function the engine has already let go of.
   */
  class ScriptOption final : public std::enable_shared_from_this<ScriptOption>, public menu::options::BaseOption {
  public:
    ScriptOption(std::string name, std::string description, std::string owner_script, std::weak_ptr<script::Script> owner);
    ~ScriptOption() override = default;

    ScriptOption(const ScriptOption&) = delete;
    ScriptOption(ScriptOption&&) = delete;
    ScriptOption& operator=(const ScriptOption&) = delete;
    ScriptOption& operator=(ScriptOption&&) = delete;

    /// The script that registered this, which is what the registry asks before letting go of it when
    /// that script unloads.
    std::string GetOwnerScript() const override {
      return owner_script_;
    }

    std::vector<menu::options::CommandArg> GetArgs() const override {
      return args_;
    }

    /// Runs the script's callback for the command line `args` was parsed from, on the game task
    /// executor's thread - which is where the caller queued this. The callback is driven to its end
    /// here, so a callback that waits is waited for by the executor rather than by this call.
    void execute(std::shared_ptr<argparse::ArgumentParser> args) override;

    /**
     * Draws this option's own row, by running the script's UI callback on the page being built right
     * now.
     *
     * This is the same hook a C++ option implements to draw its own components, reached the same way:
     * the option's row on a page is whatever this adds, and the components it adds are this option's -
     * they get its hotkey and are saved with it - because the callback runs with this option as the
     * page's current one.
     *
     * Nothing is drawn, and a report is made, when there is no page being built: an option's UI is
     * drawn by a page, and this has no frame to draw into otherwise.
     */
    void CreateOptionUi(const std::string& label, ui::Submenu* sub) override;

    /// Names the function that draws this option's own row, which is what gives an option a script
    /// registered a face of its own rather than the nothing the menu draws for one it does not know.
    /// The declaration is held rather than the function, for the same reason SetCallback holds one.
    /// False when `fn` is null or is not the shape a UI callback is called with.
    bool SetUiCallback(AngelScript::asIScriptFunction* fn);

    [[nodiscard]] bool HasUiCallback() const {
      return !ui_decl_.empty();
    }

    /// Declares one more argument, which is what gives the option a command line to be run with and a
    /// line in its help text. False when the name is empty, the type is not one of the type chars of
    /// command_line.hpp, or an argument of that name is already declared.
    bool AddArg(const std::string& name, const std::string& type, const std::string& help);

    /// Names the function to run when the option is run, which is `fn`'s declaration: the function
    /// itself is resolved again when the option runs, since the engine lends it to this call and no
    /// longer. False when `fn` is null or is not of the shape a callback is called with, which is
    /// `void fn(Option@ &in self)` - see ScriptOptionHandle for why the parameter is a reference.
    bool SetCallback(AngelScript::asIScriptFunction* fn);

    [[nodiscard]] bool HasCallback() const {
      return !callback_decl_.empty();
    }

  private:
    std::vector<menu::options::CommandArg> args_;
    std::string owner_script_;
    std::weak_ptr<script::Script> owner_;
    std::string callback_decl_;
    /// The declaration of the function that draws this option's row, or empty when it has none - which
    /// is every option until SetUiCallback is called, the menu's own included.
    std::string ui_decl_;
  };

  /**
   * The script-facing `Option`: what a script holds while it builds one, and what its callback is
   * given as `self`.
   *
   * The values a callback reads have to come from somewhere, and they are carried here rather than
   * parked on the option. A handle handed to a callback holds the parse of that run, so a callback
   * that keeps `self` keeps its own arguments, a callback that runs another option does not disturb
   * it, and nothing has to be unwound if one of them throws. A handle a script got from Find or All
   * holds no parse, and its getters say so rather than answering a zero that looks like a value the
   * script never gave.
   *
   * The option is held strongly, so a handle can never name a freed option - the option itself is
   * what goes inert when its script unloads, since that is what its callback needed.
   */
  class ScriptOptionHandle {
  public:
    ScriptOptionHandle(std::shared_ptr<menu::options::BaseOption> option, std::shared_ptr<const std::vector<menu::options::ParsedArg>> args);

    /// AngelScript's reference count: a handle is a reference type, and the engine counts it while a
    /// script holds it or passes it around.
    void AddRef();
    void Release();

    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] std::string GetDescription() const;
    [[nodiscard]] std::string GetHelp() const;

    /// The argument declaration and the callback setting, which only an option a script registered has
    /// anything to do with - either is refused with a report when this handle names one of the menu's
    /// own.
    bool AddArg(const std::string& name, const std::string& type, const std::string& help);
    bool SetCallback(AngelScript::asIScriptFunction* fn);

    /// The same for the function that draws the option's row, refused for one of the menu's own for the
    /// same reason: the menu draws its own rows, and an option that has its own row drawn for it is a
    /// script's option.
    bool SetUiCallback(AngelScript::asIScriptFunction* fn);
    [[nodiscard]] bool HasUiCallback() const;

    /**
     * The option this handle names, held strongly.
     *
     * C++ only - it is not registered with the engine and no script can reach it. It is what the GUI
     * binding draws when a page places this option: a page holds the *option* rather than the handle,
     * since an option outlives every handle a script may let go of.
     */
    [[nodiscard]] const std::shared_ptr<menu::options::BaseOption>& GetOption() const {
      return option_;
    }

    /// Puts the option this handle names into the registry under its own name, so that it can be found
    /// by name and run. Refused, with a report, for an option that is already registered under that
    /// name, for one of the menu's own, and for one that has no callback to run.
    bool Register();

    /// Whether the run that called this gave the argument `name` a value.
    [[nodiscard]] bool WasUsed(const std::string& name) const;

    // The value of the argument `name` in the shape it is asked for. Every declared argument answers in
    // every shape: one declared as an integer reads as a number, one declared as a flag reads as the 1
    // or 0 it was set to, and anything reads as the text it was typed as. An argument the run left out
    // reads as the zero of its type, and reading an argument the option never declared is reported
    // rather than answered.
    [[nodiscard]] bool GetBool(const std::string& name) const;
    [[nodiscard]] std::int32_t GetInt(const std::string& name) const;
    [[nodiscard]] std::int64_t GetInt64(const std::string& name) const;
    [[nodiscard]] float GetFloat(const std::string& name) const;
    [[nodiscard]] double GetDouble(const std::string& name) const;
    [[nodiscard]] std::string GetString(const std::string& name) const;

    /// How many handles exist right now, which is how a test holds the engine to releasing the ones it
    /// made - a handle per Find, per All element and per run.
    [[nodiscard]] static std::int64_t LiveInstances();

  private:
    /// The argument `name` names, or null when it is not one this option declares or there is no run
    /// to read from - both reported, since a callback reading an argument it never declared is a bug
    /// that would otherwise be answered with a zero.
    [[nodiscard]] const menu::options::ParsedArg* Find(const std::string& name) const;

    /// The option as the thing a script registered, or null when it names one of the menu's own - for
    /// the two calls that only make sense on a script's option.
    [[nodiscard]] std::shared_ptr<ScriptOption> AsScriptOption() const;

    std::atomic<std::uint32_t> refs_{1};
    std::shared_ptr<menu::options::BaseOption> option_;
    std::shared_ptr<const std::vector<menu::options::ParsedArg>> args_;
  };

  /**
   * `Option::SetUiCallback` and `Option::HasUiCallback`, the two calls that give an option a row of its
   * own for a page to draw.
   *
   * Declared and defined beside the rest of the option binding, but *registered* by the GUI binding,
   * which is the only place the funcdef they take exists: `SetUiCallback` takes a `gui::UiCallback`, and
   * that type is declared once RegisterGui has run. That is the one thing about the GUI the option
   * binding would have to be ordered around, and this is what keeps it out: it names the functions, it
   * does not know what a page is.
   */
  bool SetOptionUiCallback(ScriptOptionHandle* self, AngelScript::asIScriptFunction* fn);
  bool HasOptionUiCallback(const ScriptOptionHandle* self);

  /// Binds `Option`, the `OptionCallback` shape its callback is declared with, and the `Options`
  /// namespace of things to do with them.
  void RegisterOption(AngelScript::asIScriptEngine* engine);
}

#endif //AS_OPTION_HPP_5D2A8F31C74B6E90
