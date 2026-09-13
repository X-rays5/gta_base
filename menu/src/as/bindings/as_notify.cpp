//
// Created by X-ray on 12/09/2026.
//

#include "as_notify.hpp"

#include "../util/as_bind.hpp"
#include "../../ui/localization/manager.hpp"
#include "../../ui/notification/manager.hpp"

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

// Notifications, as a script asks for them.
//
// There is no state here and nothing to hold: a notification is a message the menu shows for a while and
// then forgets, and the manager is what remembers it until it has been shown. So the binding is a set of
// thin calls, and the only thing worth knowing about it is what the two strings are.
//
// Both are keys, on the same terms as every other text in the bindings: a literal shows as written and a
// key registered under the script's prefix resolves, which is what makes `notify::info("saved/title",
// "saved/message")` work off two strings the script registered. A message that wants formatting is
// formatted by the script - with `log::format` or the like - and passed as the finished text, which then
// resolves to itself: the manager does read its message as a format string, but only when it is given
// more than one message argument, and nothing here ever gives it more than one.
namespace base::menu::as::bindings::notify {
  namespace {
    namespace localization = ui::localization;
    namespace notification = ui::notification;

    /// What ADD_NOTIFICATION uses, and so what a notification without a duration gets: long enough to
    /// read a short line, short enough not to sit in the corner.
    constexpr std::uint32_t kDEFAULT_DURATION_MS = 5000;

    /**
     * The one call every function below goes through. True when the notification was queued.
     *
     * False only when there is nothing to queue it with - no notification manager, which a harness has
     * none of - so a script, or a test, can tell "the menu was not there to show it" from "it was shown".
     * The alternative, a silent no-op, reads exactly like a notification that was shown and gone again by
     * the time anything looked.
     *
     * `duration_ms` is a 32-bit count of milliseconds - about forty-nine days at the top of it, longer
     * than the menu stays open - and the manager takes the count as a `std::size_t`, hence the widening
     * here rather than in the declaration a script reads.
     */
    bool Add(const notification::Type type, const std::string& title, const std::string& message,
             const std::uint32_t duration_ms) {
      if (!notification::kNOTIFICATION_MANAGER || !localization::kMANAGER) {
        return false;
      }

      // Called from wherever the script is, on any thread: the manager takes its own lock, and is
      // already called from the WndProc thread and the thread pool, so nothing here needs marshalling.
      //
      // A message a script left out is passed as no argument at all rather than as an empty key: an empty
      // message and a message that resolved to nothing look the same on screen, and this way the manager
      // does not go looking for one.
      if (message.empty()) {
        notification::kNOTIFICATION_MANAGER->AddNotification(type, duration_ms, title);
      } else {
        notification::kNOTIFICATION_MANAGER->AddNotification(type, duration_ms, title, message);
      }

      return true;
    }

    /// `notify::show` - any of the three kinds, with a duration of the script's choosing.
    bool Show(const notification::Type type, const std::string& title, const std::string& message,
              const std::uint32_t duration_ms) {
      return Add(type, title, message, duration_ms);
    }

    // One thunk per shape and level rather than one thunk per shape with the level passed in: a
    // registered function's signature has to match its declaration exactly, and the level is the one
    // thing that differs between the three sets - a distinctly named thunk is also what lets each be
    // registered by name, where an overload set would have to be cast apart first.

    bool InfoWithDuration(const std::string& title, const std::string& message, const std::uint32_t duration_ms) {
      return Add(notification::Type::Info, title, message, duration_ms);
    }

    bool InfoWithMessage(const std::string& title, const std::string& message) {
      return Add(notification::Type::Info, title, message, kDEFAULT_DURATION_MS);
    }

    bool InfoTitle(const std::string& title) {
      return Add(notification::Type::Info, title, {}, kDEFAULT_DURATION_MS);
    }

    bool WarningWithDuration(const std::string& title, const std::string& message, const std::uint32_t duration_ms) {
      return Add(notification::Type::Warning, title, message, duration_ms);
    }

    bool WarningWithMessage(const std::string& title, const std::string& message) {
      return Add(notification::Type::Warning, title, message, kDEFAULT_DURATION_MS);
    }

    bool WarningTitle(const std::string& title) {
      return Add(notification::Type::Warning, title, {}, kDEFAULT_DURATION_MS);
    }

    bool ErrorWithDuration(const std::string& title, const std::string& message, const std::uint32_t duration_ms) {
      return Add(notification::Type::Error, title, message, duration_ms);
    }

    bool ErrorWithMessage(const std::string& title, const std::string& message) {
      return Add(notification::Type::Error, title, message, kDEFAULT_DURATION_MS);
    }

    bool ErrorTitle(const std::string& title) {
      return Add(notification::Type::Error, title, {}, kDEFAULT_DURATION_MS);
    }

    /**
     * The three shapes one level of notification is registered in: with a duration, without it, and
     * without a message either.
     *
     * Written as one call because the three differ in nothing else, and because there is a trap in
     * registering three overloads of one name: docs are keyed by the symbol rather than by the signature,
     * so a second and third `.Desc()` would append a second and third copy of every parameter to the one
     * doc the three of them share. The first is the one documented, and it documents all three arities.
     */
    void RegisterLevel(AngelScript::asIScriptEngine* const engine, const char* const level,
                       const std::string_view description, const AngelScript::asSFuncPtr& with_duration,
                       const AngelScript::asSFuncPtr& with_message, const AngelScript::asSFuncPtr& title_only) {
      util::RegisterGlobalFunction(
          engine,
          std::format("bool {}(const std::string &in title, const std::string &in message, uint duration_ms)", level).c_str(),
          with_duration, AngelScript::asCALL_CDECL)
        .Desc(description)
        .Param("title", "The line that stands out, as a key or a literal: a key this script registered "
                        "resolves, anything else shows as written.")
        .Param("message", "The line under it, on the same terms. Left out - see the other two overloads - "
                          "the notification is the title alone.")
        .Param("duration_ms", "How long to show it for, in milliseconds. Left out, five seconds.")
        .Returns("True when it was shown. False only when there is no menu to show it with, which is the "
                 "case outside the game and not inside it.");

      // No documentation on these two: they are the same symbol as the one above, and the registry keys
      // one doc per symbol.
      util::RegisterGlobalFunction(
        engine, std::format("bool {}(const std::string &in title, const std::string &in message)", level).c_str(),
        with_message, AngelScript::asCALL_CDECL);
      util::RegisterGlobalFunction(engine, std::format("bool {}(const std::string &in title)", level).c_str(),
                                   title_only, AngelScript::asCALL_CDECL);
    }
  }

  void RegisterNotify(AngelScript::asIScriptEngine* const engine) {
    engine->SetDefaultNamespace("notify");

    // Registered by hand rather than through the template that takes the name from enchantum: enchantum
    // names a type after its leaf, and the leaf here is `Type`, which collides with every other nested
    // `Type` in the bindings - `script::ScriptBase::Type` among them. Naming it explicitly is the whole
    // reason that overload exists, and the three values are listed beside it for the same reason.
    util::RegisterEnum(engine, "Type")
      .Desc("What a notification is: how it reads, and what colour it is drawn in. These are the three the "
            "menu raises itself, and the only three there are.");
    util::RegisterEnumValue(engine, "Type", "Info", static_cast<int>(notification::Type::Info))
      .Desc("Something that happened, which the player did not ask about.");
    util::RegisterEnumValue(engine, "Type", "Warning", static_cast<int>(notification::Type::Warning))
      .Desc("Something worth knowing about, which did not stop anything.");
    util::RegisterEnumValue(engine, "Type", "Error", static_cast<int>(notification::Type::Error))
      .Desc("Something that failed.");

    util::RegisterGlobalFunction(
        engine, "bool show(Type type, const std::string &in title, const std::string &in message, uint duration_ms)",
        AngelScript::asFUNCTION(Show), AngelScript::asCALL_CDECL)
      .Desc("Shows a notification of any of the three kinds, for as long as it is asked to. The three "
            "functions below are this one with the kind - and, usually, the duration - filled in.")
      .Param("type", "Which kind it is.")
      .Param("title", "The line that stands out, as a key or a literal.")
      .Param("message", "The line under it, on the same terms.")
      .Param("duration_ms", "How long to show it for, in milliseconds.")
      .Returns("True when it was shown. False only when there is no menu to show it with, which is the "
               "case outside the game and not inside it.");

    RegisterLevel(engine, "info",
                  "Shows an informational notification: something that happened, which the player did not ask "
                  "about and has nothing to do about.",
                  AngelScript::asFUNCTION(InfoWithDuration), AngelScript::asFUNCTION(InfoWithMessage),
                  AngelScript::asFUNCTION(InfoTitle));

    RegisterLevel(engine, "warning",
                  "Shows a warning: something a player should know about, which stopped nothing and needs "
                  "nothing done about it.",
                  AngelScript::asFUNCTION(WarningWithDuration), AngelScript::asFUNCTION(WarningWithMessage),
                  AngelScript::asFUNCTION(WarningTitle));

    RegisterLevel(engine, "error",
                  "Shows an error: something that failed, which the player will want to know failed.",
                  AngelScript::asFUNCTION(ErrorWithDuration), AngelScript::asFUNCTION(ErrorWithMessage),
                  AngelScript::asFUNCTION(ErrorTitle));

    engine->SetDefaultNamespace("");
  }
}
