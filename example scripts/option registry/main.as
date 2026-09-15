// An option of a script's own, and what running one looks like. Once the option below is registered it
// is an option of the menu like any other: found by name, run with a command line, and handed to this
// callback the way the command line reads.
//
// The option is registered as `optreg.set_wanted_level` rather than as `set_wanted_level`: a script's
// options carry the prefix its manifest gives it, so that this script and any other could each have an
// option called `set_wanted_level` without colliding. `short_name = "optreg"` is where this script says
// what that prefix is; without it, the prefix would be its name, `example_option_registry`.

// The callback is what runs when the option is run. Its argument is the option itself, and the
// arguments of that one run are read out of it here - a callback runs once per run, and each run
// carries its own command line.
void OnSetLevel(Option@ &in self) {
  if (self.WasUsed("--force")) {
    log::info("Setting the wanted level to {} by force", self.GetInt("--level"));
  } else {
    log::info("Setting the wanted level to {}", self.GetInt("--level"));
  }
}

void GameInit() {
  // An option is declared, not just named: its arguments are what a command line is read against, and
  // they are what its help text is written from. The name given here is the script's own half of it,
  // and GetName answers with the whole name the menu knows the option by.
  Option@ level = Option("set_wanted_level", "Sets the wanted level.");
  level.AddArg("--level", "i", "The level to set.");
  level.AddArg("--force", "b", "Whether to force the level.");
  level.SetCallback(@OnSetLevel);

  // False when something is already registered under this name, in which case the option that is there
  // stays and this one is not registered at all. A script's options are unregistered when it unloads.
  Options::Register(level);
  log::info("Registered '{}'", level.GetName());

  // Running one is the same call whether it is this script's own or the menu's: the command line is
  // read against the option's own declaration, a line that does not parse runs nothing, and the
  // callback runs on the game thread rather than inside this call.
  //
  // The name is the one it was declared with, which is a name the script is allowed to write about its
  // own options - the same call could have named it `optreg.set_wanted_level`. A name the menu has
  // registered is not read that way: `vehicle_repair` below is the menu's, with no prefix in front of
  // it, and is found as written.
  Options::Run("set_wanted_level", "--level 3 --force");

  // A script that wants to run an option it did not write finds it by name - here one of the menu's
  // own - and reads what it takes out of its help: one line per argument, with the type a command line
  // has to give it.
  Option@ repair = Options::Find("vehicle_repair");
  if (repair !is null) {
    log::info(repair.GetHelp());
  }

  // Every option the menu has, one by one, which is what a script lists when it does not already know
  // the names it wants. Each is listed under the name it is run by, this script's own included.
  std::array<Option@>@ all = Options::All();
  log::info("{} options are registered", int(all.length()));
}

void GameTick() {
}
