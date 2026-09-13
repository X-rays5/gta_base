// The half of the script that talks to the game.
//
// A native is the game thread's, so every one of them in this script is called from this file: from GameInit
// and GameTick, which the menu ticks on the game thread, and from the task a row queues when it needs the
// game from somewhere else. Nothing else in the script may call one: a page's UI callback draws on the
// render thread, which is not the game thread. What a page shows of the game is what this half read and
// published to the rows.
//
// The other half is the pages, and the two meet in the rows: a page hands a player the rows, and this half
// reads back what they set and does it to the game once per tick. Nothing here is applied by the script
// merely having been loaded - the first tick mirrors every row onto the value it already has, and a row
// only acts once it moves away from that.
//
// Both of these start only once the script's init() in gui.as has returned, so the rows this half reads are
// there by the time either is called - and neither is ever in flight twice at once.

// What this half has already done about a row. A row's value does not say whether the game was told: a
// wanted level of two is the same number whether it was set last tick or not at all, and re-setting it
// every tick would have the game's own logic fighting a script that only meant to set it once.
//
// They are filled in on the first tick rather than in GameInit, which is the file's own choice rather than
// one the menu forces: the rows they mirror are made in init(), which has returned by then, so both are
// free to read them. Taking them in the tick keeps the mirroring next to everything that consults it, and
// it is the same point either way - a row that was created with a value on it is not a row a player moved.
int g_set_wanted = -1;
int g_set_hour = -1;
int g_set_weather = -1;
int g_given_weapon = -1;             // the row's index, or -1 while the player carries nothing of ours
bool g_set_timescale = false;
bool g_mirrored = false;             // whether the first tick has taken the mirrors above

// The weapon a row's index names. A list shows what a player reads - "Pistol" - and the game wants the name
// it hashes, so the two are kept in step here rather than in the row: this is the list init() builds, in
// the same order. Adding an item there means adding a case here.
natives::Hash WeaponName(const int index) {
  switch (index) {
    case 0: return natives::Hash("WEAPON_PISTOL");
    case 1: return natives::Hash("WEAPON_CARBINERIFLE");
    case 2: return natives::Hash("WEAPON_SNIPERRIFLE");
    default: return natives::Hash("WEAPON_PUMPSHOTGUN");
  }
}

// Called once, on the game thread, after init() in gui.as has returned and before GameTick's first turn. It
// is a coroutine like the other two, so a wait here costs the game nothing while it waits.
void GameInit() {
  log::info("The game half of the example is up");
}

void GameTick() {
  const int ped = natives::player::PlayerPedId();

  // The first tick is where the mirrors above are taken, for the reason they are declared with: init() has
  // returned and this script's rows are there to read, and what they are on is what this half has already
  // done about - nothing.
  if (!g_mirrored) {
    g_mirrored = true;
    g_set_wanted = g_wanted.Get();
    g_set_hour = g_hour.Get();
    g_set_weather = g_weather.Get();
    g_set_timescale = g_timescale.GetToggled();
  }

  // The player's position, published to the row that shows it. A row's state is atomic and a list hands its
  // items over as a whole, which is what makes the game thread the safe side to write one from: the render
  // thread reads the set it was given while this builds the next one.
  const natives::Vector3 here = natives::entity::GetEntityCoords(ped, true);
  const std::string position = log::format("{}, {}, {}", int(here.x), int(here.y), int(here.z));

  // Only when the text would differ. An identical set is still a new set as far as the row is concerned, and
  // handing one over on every one of the game's ticks would be a heap allocation to say nothing.
  if (g_nearby.GetItemCount() != 1 || g_nearby.GetItem(0) != position) {
    g_nearby.SetItems({position});
  }

  // The speed of whatever the player is in, left where it was while they are on foot - the row is the
  // script's own state, and this is the script moving it the same way a player with the row selected can.
  const int vehicle = natives::ped::GetVehiclePedIsIn(ped, false);
  if (vehicle != 0) {
    g_speed.Set(natives::entity::GetEntitySpeed(vehicle) * 3.6f);
  }

  // Everything below writes to the game, and the row above them all says whether it may. A script that has
  // been switched off keeps reading the game and stops writing to it, and leaves what it already did in
  // place - a wanted level it set is the player's now, not the script's.
  if (!g_enabled.Get()) {
    return;
  }

  // The wanted level, when the player moves the row: given now rather than left to the game's own schedule,
  // which is what makes the stars appear as the row is moved instead of drifting in over a few seconds.
  // Level 0 is the one of them the game will not take as a level, so it is the clear.
  const int wanted = g_wanted.Get();
  if (wanted != g_set_wanted) {
    g_set_wanted = wanted;

    if (wanted == 0) {
      natives::player::ClearPlayerWantedLevel(0);
    } else {
      natives::player::SetPlayerWantedLevel(0, wanted, false);
      natives::player::SetPlayerWantedLevelNow(0, false);
    }
  }

  // Armour, while the row's box is ticked: held at the amount the player put it on. Every tick rather than
  // on a change, because the armour is the game's as much as the script's - a shot takes it down, and the
  // row says it should be there. Putting back the number it already has costs nothing.
  if (g_armour.GetToggled() && natives::ped::GetPedArmour(ped) != g_armour.Get()) {
    natives::ped::SetPedArmour(ped, g_armour.Get());
  }

  // The weapon, while the row's box is ticked. On a change and not every tick: a weapon handed over again
  // every tick is a weapon that never runs out of ammo, which is not what the row says.
  const int wantedWeapon = g_weapon.GetToggled() ? g_weapon.Get() : -1;
  if (wantedWeapon != g_given_weapon) {
    if (g_given_weapon >= 0) {
      // Only ever the weapon this script gave: one the player was carrying before it touched is theirs.
      natives::weapon::RemoveWeaponFromPed(ped, WeaponName(g_given_weapon));
    }

    if (wantedWeapon >= 0) {
      natives::weapon::GiveWeaponToPed(ped, WeaponName(wantedWeapon), 250, false, true);
    }

    g_given_weapon = wantedWeapon;
  }

  // The super jump, while the row's box is ticked. The native is "this frame" - the jump is allowed on the
  // frame the player jumps and on no other - so it is asked for on every tick the box is on.
  if (g_jump.Get()) {
    natives::misc::SetSuperJumpThisFrame(0);
  }

  // What the game runs at, on the value the row is on while the box is ticked - and handed back to the game
  // once on the way out, rather than every tick after it.
  if (g_timescale.GetToggled()) {
    natives::misc::SetTimeScale(g_timescale.Get());
  } else if (g_set_timescale) {
    natives::misc::SetTimeScale(1.0f);
  }
  g_set_timescale = g_timescale.GetToggled();

  // The clock, on the hour the player moves it to. The minutes go with it because the native takes all
  // three; nothing here sets them on their own.
  const int hour = g_hour.Get();
  if (hour != g_set_hour) {
    g_set_hour = hour;
    natives::clock::SetClockTime(hour, 0, 0);
  }

  // The weather, on the item the row is showing. Those items are the game's own names for it - "RAIN" - and
  // not text this script made up, which is what lets the row hand what it shows straight to the native.
  const int weather = g_weather.Get();
  if (weather != g_set_weather) {
    g_set_weather = weather;
    natives::misc::SetWeatherTypeNowPersist(g_weather.GetItem(weather));
  }
}

// Where the teleport row lands: on the waypoint the player put on the map. It runs on the game thread
// because it was asked for there - the row's handler is called on the thread pool, where a native may not
// be called, and thread::queue_game_task is how a callback reaches the one thread that may.
void TeleportToWaypoint() {
  // There is nothing to land on without one, and the row says so rather than doing nothing at all.
  if (!natives::hud::IsWaypointActive()) {
    notify::warning(gui::translations::Key("teleport/title"), gui::translations::Key("teleport/nowhere"));
    return;
  }

  // A waypoint is a blip like any other, and this is the one the player set: the game makes it sprite 8.
  const int blip = natives::hud::GetFirstBlipInfoId(8);
  if (blip == 0) {
    notify::warning(gui::translations::Key("teleport/title"), gui::translations::Key("teleport/nowhere"));
    return;
  }

  const natives::Vector3 there = natives::hud::GetBlipInfoIdCoord(blip);

  // A metre above the blip rather than at it: a blip sits on the ground, and landing exactly on it puts the
  // player's feet in it. The three axis flags are left alone, and the last one takes the traffic around the
  // landing away - the difference between arriving and arriving inside a car that was already there.
  natives::entity::SetEntityCoords(natives::player::PlayerPedId(), there.x, there.y, there.z + 1.0f,
                                   false, false, false, true);

  notify::info(gui::translations::Key("teleport/title"), gui::translations::Key("teleport/done"));
}
