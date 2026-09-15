// The GUI a script draws: the pages it owns, the rows on those pages, the strings behind the rows, and the
// options it puts there - its own, and the menu's.
//
// Everything is in the `gui` namespace, and everything is made in init(). The menu runs that once, on the
// general thread, when the script is loaded - before anything else of the script runs, and with nothing
// else of it running, which is why the pages and the options are registered there and nowhere else. A
// script has one page of its own on the root page of the menu - the one it registers - and the row it gets
// carries the script's name. Every other page it makes hangs below that row, reached by a link.
//
// A page is drawn by the callback it was made with. That callback runs on the render thread, once for every
// frame the page is shown, and whatever it adds during that call is that frame - nothing of a page is kept
// between frames. So the state behind the rows below lives in the script's own handles, and the page hands
// those handles over again every frame.
//
// There is no GUI tick and no GUI init to go with them: the menu calls into a script when it needs a frame
// drawn and at no other time, so a script that wants something to happen per frame does it in the page's
// own draw callback - which is what the counter at the bottom of this file does.
//
// None of it may call a native: a native is the game thread's, and a UI callback is the render thread's.
// The rows are the meeting point - game.as reads what a player set on one and does it to the game, and
// publishes back what it read from the game for the two rows that only report.

// The rows, held by the script so that it can read them back.
gui::Toggle@           g_enabled;      // whether any of the rows below are applied at all
gui::List@             g_nearby;       // readout: where the player is
gui::FloatRange@       g_speed;        // readout: how fast they are going
gui::IntRange@         g_wanted;       // the wanted level the script gives
gui::ToggleIntRange@   g_armour;       // the armour it keeps on the player
gui::ToggleList@       g_weapon;       // the weapon it gives, and whether it gives one
gui::Toggle@           g_jump;         // whether the player may jump like a super hero
gui::ToggleFloatRange@ g_timescale;    // how fast the game itself runs
gui::IntRange@         g_hour;         // what the clock is set to
gui::List@             g_weather;      // the weather it sets
gui::Execute@          g_teleport;     // a row that asks the game thread to move the player
gui::Execute@          g_notify;       // a row that raises a notification and nothing else
gui::Execute@          g_drop;         // a row that takes the extra page away
gui::Toggle@           g_apply_flag;   // the state this script's own option reads back

// The pages, and the one option this script registers for itself. The extra page is held as a handle for as
// long as it is there: it is the page the drop row below takes away, and a handle the script let go of is a
// page it can no longer take away.
gui::Submenu@ g_root;
gui::Submenu@ g_advanced;
gui::Submenu@ g_scratch;
Option@       g_apply;

// How many frames the root page has been drawn in. The page counts it in its own draw callback, which is
// the per-frame hook a script has: a callback that runs once for every frame the page is shown, and the
// only thing that runs that often.
int g_frames = 0;

// ---------------------------------------------------------------------------------------------------
// What the rows do
// ---------------------------------------------------------------------------------------------------

// The teleport row. Its handler runs on the thread pool, where no native may be called, so it does not move
// the player itself: it asks for the game thread, and what it queues - TeleportToWaypoint, in game.as - runs
// on that thread's next tick and does the work there. That is the one way from a callback to a native.
void OnTeleport() {
  thread::queue_game_task(@TeleportToWaypoint);
}

// A notification, of any of the three kinds, from any thread a script may run on. The title and the
// message are localized as they are drawn, so a key this script registered shows its string and anything
// else shows as written - which is what lets a script raise one with no translation at all.
void OnNotify() {
  notify::info(gui::translations::Key("notify/title"), gui::translations::Key("notify/message"));
}

// The row that takes the extra page away. Removing a page is what drops its link from the root page and puts
// a player standing on it back on the page underneath. It is not something a script has to do - every page
// it made goes when it unloads - so a script only calls Remove to drop one while it is still loaded.
//
// The handle goes with the page, which is the other half of the link that reaches it: a page that is gone has
// nothing to link to, so a script that kept the handle would have to keep saying so, and this one says it by
// letting go.
void OnDropExtra() {
  if (g_scratch !is null && g_scratch.Remove()) {
    @g_scratch = null;
    notify::info(gui::translations::Key("drop/title"), gui::translations::Key("drop/message"));
  } else {
    notify::info(gui::translations::Key("drop/title"), gui::translations::Key("drop/again"));
  }
}

// What this script's own option does when it is run - from its command line, which is
// `example_gui_options.apply --reason "..."`. A run is handed to the game thread and runs as a coroutine,
// so it may wait on the game as long as it likes - which is the reason init() and the two game entry
// points are coroutines, and the reason a page's draw callback is not.
void OnApply(Option@ &in self) {
  if (self.WasUsed("--reason")) {
    log::info("'{}' was applied: {}", self.GetName(), self.GetString("--reason"));
  } else {
    log::info("'{}' was applied", self.GetName());
  }

  // The checkbox the player switches is the script's own state, which is read back here rather than the
  // option holding any of it.
  if (g_apply_flag.Get()) {
    notify::info(gui::translations::Key("apply/title"), gui::translations::Key("apply/on"));
  } else {
    notify::info(gui::translations::Key("apply/title"), gui::translations::Key("apply/off"));
  }
}

// ---------------------------------------------------------------------------------------------------
// The pages
// ---------------------------------------------------------------------------------------------------

// The page the script is found by: the row of its own on the root page of the menu opens this one. The
// parameter is a reference to a handle - `gui::Submenu@ &in sub` - and everything added to it in this call
// is what the player sees.
//
// The rows are in two halves, and they are labelled as such: the two at the top only show what the game half
// last read, and every row below them does something to the game on its next tick. Nothing here reads the
// game itself - a page is drawn on the render thread, and a native belongs to the game thread.
void OnRootUi(gui::Submenu@ &in sub) {
  // Counted here, which is once for every frame this page is drawn in: the callback is the frame, so
  // this is the per-frame hook the script has. A page that wrote nothing back would be counting the same
  // way - what is added below is the frame, and a frame is built from scratch every time.
  ++g_frames;

  // A label is content between the rows rather than a row of its own: it cannot be selected, and left and
  // right do nothing to it.
  sub.Add(gui::Label(gui::translations::Key("heading")));

  sub.Add(g_enabled);

  // What game.as read from the game on its own tick, drawn here from what it published.
  sub.Add(gui::Label(gui::translations::Key("status/heading")));
  sub.Add(g_nearby);
  sub.Add(g_speed);

  // What this page does to the game: each of these is read back by game.as every tick and acted on.
  sub.Add(gui::Label(gui::translations::Key("controls/heading")));
  sub.Add(g_wanted);
  sub.Add(g_armour);
  sub.Add(g_weapon);
  sub.Add(g_jump);
  sub.Add(g_timescale);
  sub.Add(g_hour);
  sub.Add(g_weather);
  sub.Add(g_teleport);

  // One of the menu's own options, drawn here exactly as the menu draws it on its own pages: the same row,
  // the same hotkey, saved and loaded as always. It is found by the name the menu knows it by, which has
  // no script prefix in front of it; the label is optional, and passing one puts this script's own text on
  // the row instead of the menu's.
  sub.AddOption(Options::Find("god_mode"), gui::translations::Key("godmode"));

  // This script's own option, whose row is drawn by the callback it was given. An option with no UI
  // callback of its own draws nothing, which is why SetUiCallback below is what puts it on a page.
  sub.AddOption(g_apply, gui::translations::Key("apply"));

  // A row that runs something, and a row that goes somewhere. Neither waits on what it set going: an
  // Execute's handler and a link's on-open both run on the thread pool, and the page is not held up by
  // either of them.
  sub.Add(g_notify);
  sub.Add(gui::Link(gui::translations::Key("advanced"), "", g_advanced, @OnAdvancedOpened));

  // The extra page, which is only reachable from here: the script has one row of its own on the root page
  // of the menu, so a second page is a link from the first rather than a second row beside it. The link is
  // added only while there is a page to link to - the row below takes that page away, and the handle with
  // it, which is what makes this row go as well.
  if (g_scratch !is null) {
    sub.Add(gui::Link(gui::translations::Key("scratch"), "", g_scratch, null));
  }

  // The row that takes the extra page away again.
  sub.Add(g_drop);

  // How many frames this page has been drawn in, including this one - the count just written, read back
  // in the frame it was written for.
  sub.Add(gui::Label(log::format("Frame {}", g_frames)));
}

// The page reached from the root page by that link. It is not registered, which is what makes it a sub-page:
// a page a script reaches from one of its own is a page, and only the one it registers is listed on the
// root page of the menu.
//
// Everything on it is a component with nothing behind it, which is what it is here to show: a component a
// script never kept a handle to is still the row it made, and a string that was never registered anywhere
// shows as the text it is.
void OnAdvancedUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label(gui::translations::Key("advanced/heading")));

  // A key nothing is registered under shows as written, so a script that wants no translation anywhere can
  // pass its text as it is - here as the name of a row, and as the description of the one below it.
  sub.Add(gui::Toggle("A checkbox with no key behind it", "Its name and its description are the text they are, passed with no translation registered for either. Nothing reads it - a page may draw a row that is only there to be looked at.", false));

  // Made and added in the same statement. Nothing but the frame holds this one, and the frame keeps what it
  // was made with - so a component a script never kept a handle to is still the row it meant.
  sub.Add(gui::ToggleList("Made and dropped in one statement", "Three items, literal text, and one checkbox for the row as a whole.", {"Red", "Green", "Blue"}, 1, false));

  sub.Add(gui::Execute("Say something", "Raises a notification with a literal for both texts.",
                       @OnNotifyLiteral));
}

// The extra page, which exists to be removed.
void OnScratchUi(gui::Submenu@ &in sub) {
  sub.Add(gui::Label(gui::translations::Key("scratch/body")));
  sub.Add(gui::Toggle(gui::translations::Key("scratch/flag"), "", false));
}

// The row of this script's own option. The components added here belong to the option - they carry its
// hotkey and are saved and loaded with it - which is what a menu option's own row gets as well.
void OnApplyUi(gui::Submenu@ &in sub) {
  sub.Add(g_apply_flag);
}

// A link's on-open handler, which runs on the way to the page it names.
void OnAdvancedOpened() {
  log::info("Looking at the advanced page");
}

void OnNotifyLiteral() {
  notify::warning("No key behind this text", "It shows as written.", 3000);
}

// ---------------------------------------------------------------------------------------------------
// What the menu calls
// ---------------------------------------------------------------------------------------------------

// Called once, on the general thread, when the script is loaded, and before anything else of the script
// runs. This is the only place a page may be made and the only place an option may be registered: both are
// looked up by the script that is running, and a running script is what init() is - a callback handed to
// the thread pool, an Execute or a link's on-open, is not one, so nothing in this file that runs later can
// make a page.
//
// It is a coroutine on a thread of its own, so it may wait - for a file, for the game through
// thread::queue_game_task, for anything. The two game entry points in game.as do not start until this has
// returned, so nothing here races the game half of the script.
void init() {
  // The prefix every key below goes under, and the name of the script's own command line: a manifest
  // short_name if it has one, the script's name made into one if it has not. Keys are namespaced with it
  // so that a script cannot take a key the menu's own translations use, which is why the spelling above -
  // `translations::Key("heading")` - is the only one a script writes; the full key is never written by hand.
  log::info("Setting up the gui under the prefix '{}'", gui::translations::Prefix());

  // Every string this script shows, under a name of its own. The string is held beside the menu's own
  // translations rather than among them: it is never written to disk, and it goes when the script does, so
  // a script cannot change what the menu says and cannot leave anything behind.
  // `translations::Remove` takes one back again, for a script that changes its own text; a script that
  // does nothing is cleaned up for, like the pages are.
  gui::translations::Register("title", "Script Options");
  gui::translations::Register("heading", "Every row below was put here by a script.");

  // A component's description is looked up as its name with "/desc" after it when the description it was
  // given is empty, which is what the empty strings below are.
  gui::translations::Register("enabled", "Script");
  gui::translations::Register("enabled/desc", "Whether the script does any of the below at all. The rows it only reads the game with keep reading it either way.");

  gui::translations::Register("status/heading", "What the game last said");
  gui::translations::Register("nearby", "Player position");
  gui::translations::Register("nearby/desc", "The last position the game half read, from its own tick. The game moves this row; a player cannot.");
  gui::translations::Register("speed", "Speed");
  gui::translations::Register("speed/desc", "Kilometres per hour, as the game half last read it - left where it was while the player is on foot.");

  gui::translations::Register("controls/heading", "What this page does to the game");
  gui::translations::Register("wanted", "Wanted level");
  gui::translations::Register("wanted/desc", "The stars the script gives the player, none to five. Given as the row moves, and taken away at none of them.");
  gui::translations::Register("armour", "Armour");
  gui::translations::Register("armour/desc", "The armour the script keeps on the player while the box beside it is ticked - put back every tick, so a shot takes it down and the script puts it up again.");
  gui::translations::Register("weapon", "Weapon");
  gui::translations::Register("weapon/desc", "The weapon the script gives the player while the box beside it is ticked, with 250 rounds. Untick it and the weapon goes - only ever the one the script gave.");
  gui::translations::Register("jump", "Super jump");
  gui::translations::Register("jump/desc", "Whether the player may jump like a super hero. The game allows it only on the frame of the jump, so it is asked for on every tick the box is ticked.");
  gui::translations::Register("timescale", "Time scale");
  gui::translations::Register("timescale/desc", "What the game itself runs at: 1 is normal, 0.5 is slow motion, 2 is double. Held at the value the row is on while the box beside it is ticked, and handed back to the game on the way out.");
  gui::translations::Register("hour", "Time of day");
  gui::translations::Register("hour/desc", "The hour the game's clock is set to, when the row is moved. The minutes go with it; nothing here sets them on their own.");
  gui::translations::Register("weather", "Weather");
  gui::translations::Register("weather/desc", "The weather the script sets, in the game's own names for it, held rather than drifting on. Set when the row is moved.");
  gui::translations::Register("teleport", "Teleport to waypoint");
  gui::translations::Register("teleport/desc", "Puts the player on the waypoint they set on the map. The move itself is done on the game thread, which the row asks for when it is pressed.");
  gui::translations::Register("teleport/title", "Teleport");
  gui::translations::Register("teleport/done", "The player is at the waypoint.");
  gui::translations::Register("teleport/nowhere", "There is no waypoint on the map to teleport to. Put one there first.");

  gui::translations::Register("godmode", "God mode");
  gui::translations::Register("apply", "Apply");
  gui::translations::Register("apply/desc", "Runs the script's own option.");
  gui::translations::Register("apply/title", "Script option");
  gui::translations::Register("apply/on", "Applied with the flag on.");
  gui::translations::Register("apply/off", "Applied with the flag off.");
  gui::translations::Register("apply/flag", "Dry run");
  gui::translations::Register("notify", "Notify");
  gui::translations::Register("notify/title", "Script");
  gui::translations::Register("notify/message", "Raised from a script's own callback.");
  gui::translations::Register("advanced", "Another page");
  gui::translations::Register("advanced/desc", "A page of this script's own, reached from this one by a link. It is not on the menu's root page: a script has one row there, and this hangs below it.");
  gui::translations::Register("advanced/heading", "You came here from a link.");
  gui::translations::Register("scratch", "Extra page");
  gui::translations::Register("scratch/desc", "A second page of this script's, which the row below takes away again to show what removing a page does.");
  gui::translations::Register("scratch/body", "A page that exists to be removed.");
  gui::translations::Register("scratch/flag", "Nothing behind it");
  gui::translations::Register("drop", "Remove the extra page");
  gui::translations::Register("drop/title", "Script");
  gui::translations::Register("drop/message", "The extra page is gone, and its link with it.");
  gui::translations::Register("drop/again", "There is no extra page left to remove.");

  // The rows. Each keeps its own state, so a handle is all the page needs to hand the same row over every
  // frame it is drawn, and all the script needs to read it back.
  // The `@` is what makes each of these a handle assignment. A declaration that fills a handle in as it
  // declares it - `gui::Toggle@ t = gui::Toggle(...)` - needs none, but taking the global first and filling
  // it in here does, and the engine refuses the bare form for a reference type.
  //
  // The values they start on are what game.as mirrors in its own init, so that loading the script changes
  // nothing until a player moves a row - a row created with a value on it is not a row a player moved.
  @g_enabled    = gui::Toggle(gui::translations::Key("enabled"), "", true);
  @g_nearby     = gui::List(gui::translations::Key("nearby"), "", {"not read yet"}, 0);
  @g_speed      = gui::FloatRange(gui::translations::Key("speed"), "", 0.0f, 0.0f, 300.0f, 0.25f);
  @g_wanted     = gui::IntRange(gui::translations::Key("wanted"), "", 0, 0, 5, 1);
  @g_armour     = gui::ToggleIntRange(gui::translations::Key("armour"), "", 50, 0, 100, 5, false);
  @g_weapon     = gui::ToggleList(gui::translations::Key("weapon"), "", {"Pistol", "Carbine rifle",
                                                                        "Sniper rifle", "Pump shotgun"},
                                  0, false);
  @g_jump       = gui::Toggle(gui::translations::Key("jump"), "", false);
  @g_timescale  = gui::ToggleFloatRange(gui::translations::Key("timescale"), "", 1.0f, 0.0f, 2.0f, 0.1f, false);
  @g_hour       = gui::IntRange(gui::translations::Key("hour"), "", 12, 0, 23, 1);
  @g_weather    = gui::List(gui::translations::Key("weather"), "", {"EXTRASUNNY", "CLEAR", "CLOUDS",
                                                                    "OVERCAST", "RAIN", "THUNDER"}, 0);
  @g_apply_flag = gui::Toggle(gui::translations::Key("apply/flag"), "", false);
  @g_notify     = gui::Execute(gui::translations::Key("notify"), "", @OnNotify);
  @g_teleport   = gui::Execute(gui::translations::Key("teleport"), "", @OnTeleport);
  @g_drop       = gui::Execute(gui::translations::Key("drop"), "", @OnDropExtra);

  // The pages. A page is drawn by the callback it is made with. The advanced page and the extra page are
  // reached from the page below by a link, which is the only thing that makes them reachable at all - a
  // page that is not registered has no row of its own on the root page of the menu.
  @g_scratch  = gui::Submenu(gui::translations::Key("scratch"), @OnScratchUi);
  @g_advanced = gui::Submenu(gui::translations::Key("advanced"), @OnAdvancedUi);
  @g_root     = gui::Submenu(gui::translations::Key("title"), @OnRootUi);

  // Registering a page is what puts the script on the root page of the menu, below everything the menu put
  // there itself, under the name of the script. A script has one root - the row a player finds it by - so
  // this is the only page of this script that asks, and every other page it makes hangs below this one.
  // Registering the same page twice is one row, which is what a reload of the script does.
  if (!g_root.RegisterRoot()) {
    log::error("'{}' could not be registered on the root page", g_root.GetName());
  }

  // An option of this script's own, registered like any other: from here on it has a command line -
  // `example_gui_options.apply --reason "..."` - and any script can find it with Options::Find. Its name
  // goes under the same prefix as the translations, so another script may have one called "apply" too.
  @g_apply = Option("apply", "Applies what the page is set to.");
  g_apply.AddArg("--reason", "s", "Why it was applied. Logged, and shown in the notification.");
  g_apply.SetCallback(@OnApply);
  // Without this the option is an option with no row: an option draws itself by the callback it is given
  // here, and one that has none draws nothing.
  g_apply.SetUiCallback(@OnApplyUi);

  if (!Options::Register(g_apply)) {
    log::error("'{}' was not registered", g_apply.GetName());
  }
}
