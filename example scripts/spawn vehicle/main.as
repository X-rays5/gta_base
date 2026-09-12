// The spawn vehicle example: ask the game for the "adder" model, wait for it a frame at a time, put
// the player in the car that comes out, and unload.

// All of it is one tick function, which the menu runs as a coroutine: thread::yield() suspends it and
// the next frame resumes it where it left off, so the whole thing reads top to bottom. Its locals
// survive the wait, so nothing has to be a global.

void GameTick() {
  const int ped = natives::player::PlayerPedId();
  log::info("Ped ID: {}", ped);

  // A string converts to a hash on its own, so the model can be named rather than hashed by hand.
  // Hash is an opaque value type rather than a number, so it is the name that gets logged: log::info
  // would write "<Hash>" for the value.
  const natives::Hash model = "adder";
  log::info("Model: adder");

  natives::streaming::RequestModel(model);
  log::info("Requested model");

  // Loading is the game's work, not the script's, so this waits a frame at a time rather than
  // blocking the thread the script runs on.
  while (!natives::streaming::HasModelLoaded(model)) {
    thread::yield();
  }
  log::info("Model loaded!");

  // AngelScript has no default arguments here, so the optional flags are passed explicitly: the
  // entity is alive, and the last parameter of CreateVehicle is unused by the game.
  const natives::Vector3 coords = natives::entity::GetEntityCoords(ped, true);
  log::info("Coords: x={}, y={}, z={}", coords.x, coords.y, coords.z);

  const float heading = natives::entity::GetEntityHeading(ped);
  log::info("Heading: {}", heading);

  const int vehicle = natives::vehicle::CreateVehicle(model, coords.x, coords.y, coords.z, heading, true, false, false);
  log::info("Vehicle ID: {}", vehicle);

  if (vehicle > 0) {
    log::info("Vehicle created successfully!");
    natives::ped::SetPedIntoVehicle(ped, vehicle, -1);
    log::info("Ped set into vehicle");
  } else {
    log::info("Vehicle creation failed!");
  }

  natives::streaming::SetModelAsNoLongerNeeded(model);
  log::info("done");

  // The script was loaded to spawn one car and it has done that, so it ends itself rather than being
  // ticked every frame from here on. It does not return: unload parks the coroutine for good, which
  // is also why nothing may follow it. Without it, falling off the end of GameTick would start the
  // whole function again on the next frame.
  script::unload();
}
