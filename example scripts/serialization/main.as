// A script's own data, kept between runs. Nothing a script holds in a local survives a reload, and a
// local is one value rather than a document, so what a script wants to remember lives in a
// serialization::Value: a tree of nodes that reads and writes as JSON or TOML.
//
// Every name below is relative to a directory of this script's own - `script_data/<script name>/`, made
// when the document is - and that directory is the only place this script can write. A path that leads
// out of it is refused rather than followed, so a script cannot reach a file that is not its.

void GameInit() {
  serialization::Value settings;

  // Loading a file that is not there yet is not a failure to handle: it reads false and leaves the
  // document empty, so one piece of code covers the first run and every run after it.
  if (!settings.Load("settings.json")) {
    log::info("No settings file yet, so this run is what writes them");
  }

  // A member that was never set reads as the zero of whatever type is asked for, which is why a value
  // that is meant to be zero cannot be told apart from one that is missing. Where that matters, the
  // member is asked about first.
  if (!settings.Contains("volume")) {
    settings["volume"] = 0.8;
  }

  // A string is only a string: reading one out of a member that holds a number does not convert it, it
  // gives the empty string, so the shape of what is there is what says whether there is anything to
  // read. A node is exactly one of null, object, array, number, bool or string - a member that is not
  // there at all being null - and the six Is... methods are how a script asks which one it is holding.
  if (!settings.Contains("reason")) {
    settings["reason"] = "written by the serialization example";
  }

  if (settings["volume"].IsNumber() && settings["reason"].IsString()) {
    log::info("volume is a number, reason is a string");
  }

  // Writing a member makes it, and makes every member on the way down to it, so a document is built one
  // leaf at a time and no shape has to be declared up front: `last_run` is made by a write that goes a
  // member deeper than it, and holds an object for that reason rather than because anything said so.
  settings["last_run"]["volume"] = settings["volume"].GetDouble();

  // The document holds one number type, so a member written as a fraction reads back through whichever
  // of the getters the script wants it as - the int getter rounds to the nearest whole number rather
  // than towards zero. A whole number comes back exact until it is past 2^53, which is the one thing a
  // very large integer loses here.
  log::info("volume {} - {} as an int, {} as a float", settings["volume"].GetDouble(),
            settings["volume"].GetInt(), settings["volume"].GetFloat());

  // A list is an array of one of the primitive types, so it goes in and comes out whole rather than one
  // element at a time: SetIntArray, SetFloatArray and the rest are the same call for other elements.
  std::array<std::string> spawn_points = {"garage", "airport", "docks"};
  settings["spawn_points"].SetStringArray(spawn_points);

  std::array<std::string>@ read_spawns = settings["spawn_points"].GetStringArray();
  if (read_spawns !is null) {
    log::info("{} spawn points, the first is '{}'", int(read_spawns.length()), read_spawns[0]);
  }

  // A dictionary is the same idea for members whose names are only decided at runtime, and it becomes an
  // object rather than an array - so what is written below is a document with three members named after
  // the cars, not a list of three entries.
  std::dictionary tuning;
  tuning["adder"] = 0.85;
  tuning["zentorno"] = 1.0;
  tuning["banshee"] = 0.6;
  settings["tuning"].SetDictionary(tuning);

  // Read back as a dictionary, which is what a script does when the names are data rather than code, or
  // through the document itself when they are not: `settings["tuning"]["adder"]` is the same member.
  std::dictionary@ read_tuning = settings["tuning"].GetDictionary();
  if (read_tuning !is null && read_tuning.exists("adder")) {
    log::info("the adder is tuned to {}", double(read_tuning["adder"]));
  }

  // A member that is no longer wanted is removed rather than left behind at a zero, since a member that
  // is not in the document is what a later run reads as missing. An older version of this script wrote
  // its level as a number and no longer does; the false below is that member not being there to remove,
  // which leaves the document as the one it would be either way.
  settings.Remove("legacy_level");

  // The members in the order they were written, which is the order the file they are saved to will list
  // them in: a document keeps the shape its author gave it, not just the values in it.
  std::array<std::string>@ keys = settings.GetKeys();
  log::info("{} members:", int(keys.length()));
  for (uint i = 0; i < keys.length(); i++) {
    log::info("  {}", keys[i]);
  }

  // The same document as text, which is the half of this that is for handing data to something that
  // takes a string rather than a file. Parse is the other direction, and a text that does not parse
  // leaves the document it was read into exactly as it was.
  const std::string text = settings.Dump(serialization::ValueFormat::JSON);

  serialization::Value copy;
  if (copy.Parse(text)) {
    log::info("round tripped {} members through {} characters of text", int(copy.GetSize()),
              int(text.length()));
  }

  // Saving writes the whole document, naming a file inside this script's own directory and making any
  // directory a name asks for on the way. The format is the argument rather than the file's extension -
  // these two names are a convention, and the second call without its argument would write JSON into a
  // file called settings.toml - and the two formats differ only in how the one document is spelled.
  //
  // A TOML file is a table, so a document that is not an object has no TOML form at all and saving one
  // is refused rather than written as something a reader would reject. This document is an object, so
  // both calls below write.
  log::info("json: {}", settings.Save("settings.json"));
  log::info("toml: {}", settings.Save("settings.toml", serialization::ValueFormat::TOML));
}

void GameTick() {
  script::unload();
}
