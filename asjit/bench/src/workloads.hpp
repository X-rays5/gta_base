//
// Created by X-ray on 13/09/2026.
//

#pragma once

#include "engine_harness.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace base::asjit::bench {

  // Which engine configuration a case runs under. The workload is the same script either way; these
  // change what the engine does around it, and they are the axes along which the answer moves most.
  //
  // The last two fields default to the configuration the *menu* runs, so `plain` is the menu and every
  // other variant is a deliberate departure from it. That is the useful direction for the defaults to
  // point: a row is evidence about the shipped engine unless its own name says otherwise, and the two
  // departures below exist because the menu's configuration makes a handful of translated instructions
  // unreachable - so without them those translations would be shipped, unmeasured and unmeasurable.
  struct Variant {
    const char* name;
    bool install_line_callback;

    // asEP_DISABLE_INTEGER_DIVISION, which the menu sets and which rewrites `int / int` and `int % int`
    // to double before the bytecode exists. Off here, the four integer *division* handlers
    // (DIVi DIVu DIVi64 DIVu64) become reachable - they are translated, and until this variant existed
    // no row in the suite had ever produced one. The four remainder handlers are not the variant's
    // doing: `%` is exempt from the flag, so `modulo` reaches MODi MODu MODi64 MODu64 under `plain`,
    // and the rows that run here reach them again.
    //
    // The unsigned two of the four need the *script* to offer an unsigned dividend as well as the
    // engine to allow the operator, which is a second condition on the same claim and was missed when
    // this comment was first written: `int_divide` divided only signed values, so DIVu and DIVu64 were
    // named here as reachable while no row produced them.
    bool disable_integer_division = true;

    // asEP_OPTIMIZE_BYTECODE. Off, the peephole passes that fold `ClrHi`/`ClrVPtr`/`PshNull`/`SwapPtr`
    // away do not run, so the translator is handed shapes the menu's engine never emits. All four are
    // translated, and all four are reached by the `unoptimized` row under this variant - which is the
    // check that the list is the right list, since a name in it that no row produces would be a claim
    // about the engine rather than an observation of one.
    bool optimize_bytecode = true;

    // The two fields of `asjit::Options`, which is the translator's configuration rather than the
    // engine's. They are on a `Variant` for the same reason the two above are: they change what the
    // *translator* does with the same bytecode, so a row under them is only readable next to its
    // `plain` twin, and the pair is what a default gets set from.
    bool translate_returns = false;
    bool translate_object_method_calls = false;
  };

  inline constexpr Variant kVariants[] = {
      // The menu's own configuration.
      {"plain", false},

      // The watchdog configuration: a line callback makes every statement a hand-off point. Same engine
      // otherwise, so the pair of rows is the per-block entry cost and nothing else.
      {"linecb", true},

      // The two widening axes. They are configurations rather than workloads because what they change is
      // what the engine *emits*, not what the script does - the same script under `intdiv` is a different
      // program, and the row is only readable next to its `plain` twin.
      {"intdiv", false, /*disable_integer_division=*/false, /*optimize_bytecode=*/true},
      {"noopt", false, /*disable_integer_division=*/true, /*optimize_bytecode=*/false},

      // Full opcode coverage: the two instructions the translator hands back by default - the return
      // and the pointer-returning method call - are translated instead. This is the row that decides
      // whether either should be on, and it is the only configuration in which those two translations
      // run at all, so it is also their evidence. See `Workload::run_full_coverage_variant` for why
      // workloads opt in rather than the whole suite running under it.
      {"fullcov", false, /*disable_integer_division=*/true, /*optimize_bytecode=*/true,
       /*translate_returns=*/true, /*translate_object_method_calls=*/true},
  };

  struct Workload {
    // Short identifier, used as the table's first column and as the CSV key.
    const char* name;

    // What this case is for. Printed in the long listing, not the table.
    const char* purpose;

    // The script. Its `main` is what gets timed.
    const char* source;

    // Which variants it runs under. Cases that isolate the translator's opcode coverage do not need
    // the line-callback variant - it would measure the callback, not the coverage - so they opt out
    // rather than paying for a row nobody reads.
    bool run_line_callback_variant;

    // The same opt-in for the two widening axes above. Default off for the same reason: a row that
    // cannot produce the instruction the variant exists for is a row nobody reads, and the two
    // configurations differ from the menu's in ways that make their *timings* incomparable with a
    // `plain` row. Only cases whose scripts actually reach those instructions ask for them.
    bool run_integer_division_variant = false;
    bool run_bytecode_optimizer_variant = false;

    // The same opt-in for `fullcov`, and it is opt-in for a stronger reason than the two above: the
    // two instructions it turns on are ones every workload with a function call reaches, so running the
    // whole suite under it would double every row to answer a question about two of them. The rows that
    // opt in are the ones whose scripts actually contain the instructions - a workload that reaches
    // neither would report the same numbers twice.
    bool run_full_coverage_variant = false;
  };

  // The cases, in the order they are run. Grouped: synthetic cases that establish the ceiling for
  // each opcode class, then cases where the JIT is expected to be neutral or worse, then scripts
  // shaped like the ones the menu actually runs.
  [[nodiscard]] const std::vector<Workload>& Workloads();

  // Registers the stubs the realistic cases call: a handful of no-op natives, a thread::yield-like
  // binding, and one generic variadic log call - the two conventions the menu's 6,692 registrations
  // are split between.
  //
  // These stand in for those natives. They are a faithful substitute rather than a compromise: the real
  // ones go through `RegisterGlobalFunction` with `asCALL_CDECL` or `asCALL_GENERIC`, and a stub
  // registered the same way compiles to the same call instruction with the same argument marshalling.
  // What the benchmark measures is the hand-off around the call, so what the native does can be nothing
  // at all - and nothing here needs the game to be present.
  //
  // Both conventions are represented because the disjunction is not a detail: the generic convention
  // marshals through an `asCGeneric` object the engine builds, and for a variadic call through an
  // `asCGenericVariadic` whose pop size comes from the argument count the call carried. Until
  // `StubLogFormat` existed, every stub here was `asCALL_CDECL`, so half of this sentence described
  // something no row in the suite had ever run.
  void RegisterStubs(AngelScript::asIScriptEngine* engine);

} // namespace base::asjit::bench
