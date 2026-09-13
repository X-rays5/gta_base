//
// Created by X-ray on 13/09/2026.
//

// Measures whether the JIT is worth having.
//
// The method is a comparison, not an absolute: every workload is run twice on identically configured
// engines, once with the compiler attached and once without, and what gets reported is the ratio.
// The single difference between the two arms is whether `asjit::Attach` was called, so any difference
// in the numbers is the JIT's doing.
//
// This rests on the interpreter being the same interpreter in both arms, which is what the Debug
// refusal in main() is about.

#include "workloads.hpp"

#include <asjit.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace base::asjit::bench {
  namespace {

    using Clock = std::chrono::steady_clock;

    // What one run of one workload produced. Returned rather than filled in through a dozen out
    // parameters, because the fields are read together and a caller that mis-orders twelve arguments
    // fails by producing a plausible number rather than by failing to compile.
    struct ArmResult {
      double elapsed_ms = 0.0;

      // How long the module took to build, which is where the JIT's one-time cost lands: the engine
      // calls `asIJITCompilerV2::NewFunction` while it compiles each function, not when one is first
      // entered. So the difference between this column in the two arms is the whole of what attaching
      // the JIT costs before a script runs its first instruction - and it is a column rather than a
      // sentence because the two cases it separates are far apart: a script compiled once and run for
      // an hour amortises this to nothing, and a script built per use may not.
      double build_ms = 0.0;

      double coverage = 0.0;
      std::uint64_t blocks = 0;
      std::uint64_t code_size = 0;
      std::uint64_t functions_walked = 0;
      std::uint64_t functions_compiled = 0;
      std::uint64_t functions_declined = 0;
      std::uint64_t line_callbacks = 0;

      // What the engine reports about the entry point. The authoritative answer to "did the JIT
      // engage", as opposed to the counters, which could be counting a compile that was rejected.
      bool entry_has_jit = false;

      // Everything the script printed, in order. Compared between the two arms of a row rather than
      // reported - see `Measurement::arms_agree` for what that is for.
      std::string output;

      // Set when the workload did not produce a timing - a build failure or a script exception.
      std::string failure;

      [[nodiscard]] bool ok() const { return failure.empty() && elapsed_ms > 0.0; }
    };

    struct Measurement {
      ArmResult interpreted;
      ArmResult compiled;

      [[nodiscard]] double Speedup() const {
        return compiled.elapsed_ms > 0.0 ? interpreted.elapsed_ms / compiled.elapsed_ms : 0.0;
      }

      [[nodiscard]] bool ok() const { return interpreted.ok() && compiled.ok(); }

      // Whether the two arms produced the same script output.
      //
      // This is not a timing and it is not the benchmark's subject, and it is here because the
      // benchmark is the only thing that runs every workload under a configuration the correctness
      // suite does not - `fullcov` above all, whose two instructions are translated in no other run of
      // this binary. Without it a row can be fast because it is wrong, which is a failure mode a
      // ratio cannot express: an arm that skipped a return, or that ran a method whose raise was
      // dropped, would produce a smaller number and look like the win the table is looking for.
      //
      // It costs nothing, because both arms have already run and both already captured what they
      // printed. It is checked rather than printed, so the answer is in the exit status.
      [[nodiscard]] bool arms_agree() const { return interpreted.output == compiled.output; }
    };

    // Runs `source` once on a fresh engine and returns how long `main` took.
    //
    // A fresh engine per run rather than one reused engine: the counters have to be per run to be
    // reportable, and a warm engine would otherwise leak into the next measurement.
    [[nodiscard]] ArmResult TimeOnce(const std::string& source, bool use_jit, const Variant& variant) {
      ArmResult result;

      support::ClearOutput();
      support::ClearErrors();
      support::ResetLineCallbackCount();
      asjit::ResetStats();

      support::EngineConfig config;
      config.use_jit = use_jit;

      // The menu's own configuration, so that what is measured is what the menu runs - and, for the two
      // widening variants, the one or two properties by which that configuration is deliberately departed
      // from. `Variant` carries the engine-side axes and this is where they are applied; nothing below
      // this point knows which variant it is in, so the two arms of a row cannot disagree about it.
      //
      // The line callback is not one of them: it is installed per context at execution time rather than
      // on the engine, so it is passed to `Execute` below rather than through the configuration. That
      // is also how `RunScriptWithConfig` carries it, so this and the test suite agree about what the
      // flag means even though they set it in different places.
      config.disable_integer_division = variant.disable_integer_division;
      config.optimize_bytecode = variant.optimize_bytecode;
      config.translate_returns = variant.translate_returns;
      config.translate_object_method_calls = variant.translate_object_method_calls;

      auto* engine = support::CreateEngine(config);
      if (engine == nullptr) {
        result.failure = "asCreateScriptEngine failed";
        return result;
      }

      RegisterStubs(engine);

      {
        const auto build_start = Clock::now();
        const auto script = support::Build(engine, "asjit_bench", source, "void main()");
        const auto build_end = Clock::now();
        result.build_ms = std::chrono::duration<double, std::milli>(build_end - build_start).count();

        if (!script.built || script.entry == nullptr) {
          result.failure = support::Errors();
          if (result.failure.empty())
            result.failure = "no main() in the workload";
          engine->ShutDownAndRelease();
          return result;
        }

        const auto start = Clock::now();
        const auto execution = support::Execute(script.entry, variant.install_line_callback);
        const auto end = Clock::now();

        result.elapsed_ms = std::chrono::duration<double, std::milli>(end - start).count();
        result.line_callbacks = support::LineCallbackCount();
        result.entry_has_jit = script.entry->GetJITFunction() != nullptr;
        result.output = support::Output();

        if (execution.result == AngelScript::asEXECUTION_EXCEPTION)
          result.failure = "script exception: " + execution.exception;

        const auto stats = asjit::GetStats();
        result.blocks = stats.blocks_emitted;
        result.code_size = stats.code_size;
        result.functions_walked = stats.functions_seen;
        result.functions_compiled = stats.functions_compiled;
        result.functions_declined = stats.functions_declined;
        result.coverage = stats.instructions_total == 0
                            ? 0.0
                            : static_cast<double>(stats.instructions_translated) / static_cast<double>(stats.instructions_total);
      }

      engine->ShutDownAndRelease();
      return result;
    }

    // The fewest, which is the estimate of the work itself rather than of the work plus whatever else
    // the machine was doing. For a CPU-bound measurement the fastest run is the one least disturbed by
    // the scheduler and by other processes on the box, so it is the closest to the number being
    // asked for; an average would carry the noise in.
    [[nodiscard]] double Fastest(const std::vector<double>& samples) {
      return *std::min_element(samples.begin(), samples.end());
    }

    [[nodiscard]] Measurement Measure(const Workload& workload, const Variant& variant, int repetitions) {
      Measurement measurement;

      const std::string source = workload.source;

      // The timed runs, with two untimed ones in front of them. The first execution of compiled code
      // takes page faults on memory the allocator has only just handed out, and the first execution of
      // anything at all pays for cold caches and branch predictors. Timing either would measure the
      // setup rather than the work.
      const auto run_arm = [&](bool use_jit) {
        std::vector<double> samples;
        std::vector<double> builds;
        ArmResult last;

        for (int i = 0; i < repetitions + 2; ++i) {
          last = TimeOnce(source, use_jit, variant);
          if (!last.ok())
            return last;
          if (i >= 2) {
            samples.push_back(last.elapsed_ms);
            builds.push_back(last.build_ms);
          }
        }

        // The same estimator as the elapsed column, and for the same reason: the build is measured on
        // every repetition rather than once, so the run least disturbed by the machine is the closest
        // to the cost itself. What is *not* done is keeping the build time of whichever run happened to
        // be fastest at executing - that would pair two numbers from one run in the table while having
        // come from two, and the two are not otherwise related.
        last.elapsed_ms = Fastest(samples);
        last.build_ms = Fastest(builds);
        return last;
      };

      measurement.interpreted = run_arm(false);
      measurement.compiled = run_arm(true);
      return measurement;
    }

    // -----------------------------------------------------------------------
    // Reporting
    // -----------------------------------------------------------------------

    void PrintTableHeader() {
      std::printf("%-22s %-7s %12s %12s %11s %9s %10s %8s %9s\n", "workload", "cfg", "interp ms", "jit ms",
                  "speedup", "instr cov", "build +/-", "blocks", "code B");
      std::printf("%s\n", std::string(118, '-').c_str());
    }

    void PrintTableRow(const Measurement& m, const char* name, const char* config, bool csv) {
      if (csv) {
        std::printf("%s,%s,%.3f,%.3f,%.4f,%.4f,%.3f,%.3f,%llu,%llu\n", name, config, m.interpreted.elapsed_ms,
                    m.compiled.elapsed_ms, m.Speedup(), m.compiled.coverage, m.interpreted.build_ms,
                    m.compiled.build_ms, static_cast<unsigned long long>(m.compiled.blocks),
                    static_cast<unsigned long long>(m.compiled.code_size));
        return;
      }

      // A workload the JIT declined entirely prints as such rather than as a timing, so that a
      // regression to "the JIT compiled nothing" cannot masquerade as a result.
      char speedup_text[32];
      if (m.compiled.functions_compiled == 0)
        std::snprintf(speedup_text, sizeof(speedup_text), "NOT COMPILED");
      else
        std::snprintf(speedup_text, sizeof(speedup_text), "%.2fx", m.Speedup());

      // Signed, because the sign is the answer: a negative column is the JIT paying for itself before
      // the script runs, and a zero or near-zero one is a module whose functions were declined.
      const double build_delta = m.compiled.build_ms - m.interpreted.build_ms;
      char build_text[32];
      std::snprintf(build_text, sizeof(build_text), "%+.3f", build_delta);

      std::printf("%-22s %-7s %12.3f %12.3f %11s %8.1f%% %10s %8llu %9llu\n", name, config,
                  m.interpreted.elapsed_ms, m.compiled.elapsed_ms, speedup_text, m.compiled.coverage * 100.0,
                  build_text, static_cast<unsigned long long>(m.compiled.blocks),
                  static_cast<unsigned long long>(m.compiled.code_size));
    }

    // What the whole suite reached, accumulated by the per-workload diagnostic below so that the two
    // cannot drift: they are the same numbers, read once per workload and once for everything.
    //
    // The per-workload lines explain a single ratio. This is the other question a suite of workloads
    // has to answer about itself - how much of the translator it exercises at all - and it is the
    // only one that can be answered by looking at all of the workloads together. A translator case
    // that no workload produces is a translation with no measurement behind it, which is not the same
    // as an untested one (the differential tests are what say a translation is correct) but is what
    // decides whether a ratio in the table means anything for that instruction.
    struct SuiteCoverage {
      std::array<std::uint64_t, 256> translated{};
      std::array<std::uint64_t, 256> refused{};
    };

    SuiteCoverage g_suite;

    // Names for the opcodes that came up, in descending order of how often they were *not* translated.
    //
    // This is the diagnostic that turns a disappointing ratio into an explanation. A coverage figure
    // says how much was translated; this says which instructions were not, and the answer is usually
    // one opcode - a call - which is where the effort would have to go next.
    //
    // It prints two lines because one would be misleading about half the time. An opcode is missing
    // from the translated count either because the translator has no case for it, which is a gap in
    // the translator, or because it was never handed over - it sits past the point where translation
    // of its block stopped, with no entry point in between. In the second case the translator already
    // handles the opcode and translating it harder would change nothing. Both read as "untranslated"
    // and they are the difference between a next step and a dead end, so the second line lists only
    // the first kind.
    void PrintUntranslatedOpcodes(const Workload& workload, const Variant& variant, int limit) {
      support::EngineConfig config;
      config.use_jit = true;

      // The variant's own configuration, not the menu's. The point of the two lines below is to name
      // what the translator was handed, and what it is handed depends on what the engine emitted - so a
      // row printed under `intdiv` has to be compiled by an engine that permits integer division, or it
      // would name the opcodes of a different program than the one in the row above it.
      config.disable_integer_division = variant.disable_integer_division;
      config.optimize_bytecode = variant.optimize_bytecode;
      config.translate_returns = variant.translate_returns;
      config.translate_object_method_calls = variant.translate_object_method_calls;

      support::ClearErrors();
      asjit::ResetStats();

      auto* engine = support::CreateEngine(config);
      if (engine == nullptr)
        return;

      RegisterStubs(engine);

      {
        const auto script = support::Build(engine, "asjit_bench", workload.source, "void main()");
        if (!script.built) {
          engine->ShutDownAndRelease();
          return;
        }

        const auto stats = asjit::GetStats();

        // Folded in before the early return below, so that a workload with nothing left untranslated
        // still counts towards the suite: it is the workloads that translate everything which say
        // most about how wide the coverage is.
        for (std::size_t opcode = 0; opcode < stats.opcodes_translated.size(); ++opcode) {
          g_suite.translated[opcode] += stats.opcodes_translated[opcode];
          g_suite.refused[opcode] += stats.opcodes_unimplemented[opcode];
        }

        // `count` is whatever the list the entry belongs to is counting, so that one sort and one
        // printer serve both: the first list counts instructions left untranslated, the second counts
        // the ones the translator was asked for and refused. The same opcode can be in both with
        // different numbers, and the difference is the point.
        struct Entry {
          std::uint64_t count;
          std::uint64_t walked;
          std::uint8_t opcode;
        };
        std::vector<Entry> entries;
        std::vector<Entry> gaps;

        for (std::size_t opcode = 0; opcode < stats.opcodes_walked.size(); ++opcode) {
          // asBC_JitEntry is skipped, and it would otherwise be at the top of every list here. It is
          // not work that went untranslated - it is the boundary marker the compiler puts in front of
          // each loop, it has nothing to do, and the JIT never needs to execute one. Counting it as a
          // cost would bury the opcode that actually is one.
          if (opcode == static_cast<std::size_t>(AngelScript::asBC_JitEntry))
            continue;

          const auto walked = stats.opcodes_walked[opcode];
          const auto translated = stats.opcodes_translated[opcode];
          if (walked <= translated)
            continue;

          const auto as_byte = static_cast<std::uint8_t>(opcode);
          entries.push_back({walked - translated, walked, as_byte});

          if (const auto declined = stats.opcodes_unimplemented[opcode]; declined > 0)
            gaps.push_back({declined, walked, as_byte});
        }

        if (entries.empty()) {
          engine->ShutDownAndRelease();
          return;
        }

        const auto by_count = [](const Entry& a, const Entry& b) { return a.count > b.count; };
        std::sort(entries.begin(), entries.end(), by_count);
        std::sort(gaps.begin(), gaps.end(), by_count);

        // The names come from the engine's own table rather than from a list here, so a name that
        // disagrees with the opcode is impossible.
        // The list is capped per line so that a row stays one line wide, but the cap is a line break
        // and not a truncation: what does not fit is named on a continuation line rather than
        // replaced by a count. The names are the explanation for the coverage column, and "and five
        // more" is not an explanation - the opcode a reader is looking for is likely among them,
        // because the list is ordered by how often each occurred and the interesting ones are rarely
        // the most common.
        const auto print_list = [limit](const char* label, const std::vector<Entry>& list) {
          std::printf("    %s", label);
          if (list.empty()) {
            std::printf("(none)\n");
            return;
          }

          // The names come from the engine's own table rather than from a list here, so a name that
          // disagrees with the opcode is impossible.
          const auto width = static_cast<std::size_t>(limit);
          for (std::size_t i = 0; i < list.size(); ++i) {
            if (i != 0 && i % width == 0)
              std::printf("\n    %*s", static_cast<int>(std::strlen(label)), "");

            const char* name = AngelScript::asBCInfo[list[i].opcode].name;
            std::printf("%s x%llu  ", name != nullptr ? name : "?",
                        static_cast<unsigned long long>(list[i].count));
          }
          std::printf("\n");
        };

        // Two lists rather than one, because the row above reports a coverage fraction and this is
        // what explains it - and a single list cannot say which of its entries are the explanation.
        // An opcode in the first list but not the second is one the translator has a case for, sitting
        // in a block that stopped before reaching it; the fix there is not to translate the opcode,
        // because it already is translated everywhere the emitter is asked for it. An opcode in the
        // second list is the one worth the work, and is usually a call.
        print_list("untranslated:   ", entries);
        print_list("no translation: ", gaps);
      }

      engine->ShutDownAndRelease();
    }

    // The suite's own coverage, printed once at the end rather than under every row.
    //
    // Two numbers and one list. The first number is how many distinct opcodes some workload compiled
    // at least once - a measure of how much of the translator the table is sampling, which a reader
    // has no other way to judge. The second is how many distinct opcodes the translator was handed
    // and had no case for, anywhere in the suite, and it is the one that says where the remaining
    // work is: unlike the per-workload lines, an opcode here is one that no amount of widening a
    // script could avoid, because some workload already produced it.
    //
    // The two are counted over every row, so once a variant exists that translates something the
    // default configuration hands back, the same opcode can be in both - and the pair of numbers is
    // then a statement about the *suite* rather than about one configuration. The `fullcov` variant is
    // what does that here: `RET` and `Thiscall1` are named on both lines, which is the whole point of
    // it - both are translated by some row in this table, and both are handed back by the rows that
    // run the configuration the menu runs.
    //
    // It is the union of the two lines printed under each row, which is why it is accumulated there
    // rather than recomputed here.
    void PrintSuiteCoverage() {
      std::vector<std::uint8_t> exercised;
      std::vector<std::uint8_t> gaps;

      for (std::size_t opcode = 0; opcode < g_suite.translated.size(); ++opcode) {
        if (g_suite.translated[opcode] > 0)
          exercised.push_back(static_cast<std::uint8_t>(opcode));
        if (g_suite.refused[opcode] > 0)
          gaps.push_back(static_cast<std::uint8_t>(opcode));
      }

      // Named as a *pair* rather than as a total, because an opcode in both lists is a real answer and
      // not an inconsistency: some row translated it and some row handed it back, and which rows they
      // are is the `fullcov` variant. A reader who wants "which are never translated anywhere" is
      // asking a question this table can answer, but not with one number.
      std::printf("\nthe suite translated %zu distinct opcodes; %zu were handed over and refused\n",
                  exercised.size(), gaps.size());

      // Named rather than counted, because the useful question about a suite of workloads is not how
      // broad it is but *what* it covers: an instruction missing from this list is one whose ratio in
      // the table above is really the ratio of whatever ran instead. Sorted by opcode, so it is a
      // list to read against the emitter's switch rather than a ranking of anything.
      std::printf("  translated:     ");
      for (std::size_t i = 0; i < exercised.size(); ++i) {
        if (i != 0 && i % 10 == 0)
          std::printf("\n                  ");

        std::printf("%s ", AngelScript::asBCInfo[exercised[i]].name);
      }
      std::printf("\n");

      std::sort(gaps.begin(), gaps.end(), [](std::uint8_t a, std::uint8_t b) {
        return g_suite.refused[a] > g_suite.refused[b];
      });

      std::printf("  no translation: ");
      if (gaps.empty()) {
        std::printf("(none)\n");
        return;
      }

      for (std::size_t i = 0; i < gaps.size(); ++i) {
        if (i != 0 && i % 6 == 0)
          std::printf("\n                  ");
        const char* name = AngelScript::asBCInfo[gaps[i]].name;
        std::printf("%s x%llu  ", name != nullptr ? name : "?",
                    static_cast<unsigned long long>(g_suite.refused[gaps[i]]));
      }
      std::printf("\n");
    }

  } // namespace
} // namespace base::asjit::bench

int main(int argc, char** argv) {
#ifndef NDEBUG
  // The refusal path takes no arguments, but it has to keep the same signature as the real one.
  static_cast<void>(argc);
  static_cast<void>(argv);

  std::printf("asjit_bench: refusing to run in a Debug build.\n\n");
  std::printf("  The AngelScript interpreter is compiled from source in this project and inherits the build\n");
  std::printf("  configuration's optimisation flags. In Debug it is built with /Od /Ob0 /RTC1, while the JIT's\n");
  std::printf("  emitted machine code is unaffected by any build flag - so a Debug run measures unoptimised\n");
  std::printf("  interpreter code against optimised native code and reports a speedup that is mostly an\n");
  std::printf("  artefact of the configuration.\n\n");
  std::printf("  Configure a separate Release directory instead. Do not add /O2 to this target as a shortcut:\n");
  std::printf("  that optimises the benchmark, not the interpreter it is measuring, and biases the ratio\n");
  std::printf("  further in the JIT's favour.\n\n");
  std::printf("    cmake -S . -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release -DBASE_BUILD_TOOLS=OFF\n");
  std::printf("    cmake --build cmake-build-release --target asjit_bench\n");
  std::printf("    cmake-build-release/asjit/bench/asjit_bench.exe\n\n");
  return 1;
#else
  using namespace base::asjit::bench;

  int repetitions = 7;
  bool csv = false;
  const char* filter = nullptr;

  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--csv") == 0) {
      csv = true;
    } else if (std::strcmp(argv[i], "--reps") == 0 && i + 1 < argc) {
      repetitions = std::max(1, std::atoi(argv[++i]));
    } else if (std::strcmp(argv[i], "--filter") == 0 && i + 1 < argc) {
      filter = argv[++i];
    } else {
      std::printf("usage: asjit_bench [--reps N] [--csv] [--filter SUBSTRING]\n");
      return 2;
    }
  }

  if (!csv) {
    std::printf("asjit_bench - AngelScript JIT, compiled against interpreted\n\n");
    std::printf("  build      Release\n");
    std::printf("  compiler   MSVC %d\n", static_cast<int>(_MSC_VER));
    std::printf("  reps       %d, fastest reported\n", repetitions);
    std::printf("\n");
    std::printf("  Every row is the same script on two engines that differ only in whether the JIT was\n");
    std::printf("  attached.\n\n");
    std::printf("  'instr cov' is the fraction of the script's *instructions* that were translated. It is an\n");
    std::printf("  instruction count, not a share of the time: a translated multiply and an untranslated call\n");
    std::printf("  are one instruction each, and they do not cost anything like the same. That is why a high\n");
    std::printf("  coverage next to a low ratio is a normal result, not a contradiction - it means the\n");
    std::printf("  instructions that were left behind are the expensive ones. The 'untranslated:' line names\n");
    std::printf("  them.\n\n");
    std::printf("  Under each row the 'no translation:' line narrows that to the opcodes the translator was\n");
    std::printf("  actually asked for and has no case for. An opcode on the first line but not the second was\n");
    std::printf("  never handed over - it sits past where its block stopped - so it is not a gap to close, and\n");
    std::printf("  the second line is where the work is.\n\n");
    std::printf("  'cfg=linecb' installs a line callback, which makes every statement a hand-off point -\n");
    std::printf("  the watchdog configuration, and the worst case for the JIT.\n\n");
    std::printf("  'build +/-' is how much longer the module took to build with the JIT attached, in ms. It is\n");
    std::printf("  the one-time cost, and it is outside both timing columns: the engine calls the translator\n");
    std::printf("  while it compiles each function, so the whole of it lands here and none of it in 'jit ms'.\n");
    std::printf("  Read it against the work a script does. A script built once and run for a minute amortises a\n");
    std::printf("  millisecond to nothing; one rebuilt per tick may not, and the ratio in the speedup column\n");
    std::printf("  would not show that at all. 'reps' above count only the timed runs - both arms build the\n");
    std::printf("  module again on every repetition, so this column is the fastest of the same runs the timing\n");
    std::printf("  columns come from.\n\n");
    std::printf("  Compare rows within one run, and compare runs of one binary against each other. Do not\n");
    std::printf("  compare a run against a run of a *different build*: both arms of a row are timed in the\n");
    std::printf("  same process, so a ratio is insulated from whatever the machine was doing, but the two\n");
    std::printf("  columns are not equally insulated from where the linker put things. Rebuilding this\n");
    std::printf("  executable with a one-line change to the library has moved the interpreted column of\n");
    std::printf("  arith_int by 44%% while the compiled column moved 3%%, on a row neither change touches.\n");
    std::printf("  Within one binary the run-to-run spread is about 0.2-0.5%% at the median and up to 6-8%%\n");
    std::printf("  on individual rows. A single row moving by a few percent is therefore not a result, and a\n");
    std::printf("  pair of runs of the same configuration will disagree about the sign of such a change: it\n");
    std::printf("  takes a median over the rows to see an effect that small, which is what makes a row-to-row\n");
    std::printf("  comparison inside a single run the only timing claim this harness supports.\n\n");
  }

  int problems = 0;
  int rows = 0;

  if (csv)
    std::printf("workload,cfg,interp_ms,jit_ms,speedup,coverage,build_interp_ms,build_jit_ms,blocks,code_bytes\n");
  else
    PrintTableHeader();

  for (const auto& workload : Workloads()) {
    if (filter != nullptr && std::strstr(workload.name, filter) == nullptr)
      continue;

    for (const auto& variant : kVariants) {
      if (variant.install_line_callback && !workload.run_line_callback_variant)
        continue;
      if (!variant.disable_integer_division && !workload.run_integer_division_variant)
        continue;
      if (!variant.optimize_bytecode && !workload.run_bytecode_optimizer_variant)
        continue;
      if ((variant.translate_returns || variant.translate_object_method_calls) && !workload.run_full_coverage_variant)
        continue;

      const auto measurement = Measure(workload, variant, repetitions);

      if (!measurement.ok()) {
        const std::string reason = measurement.interpreted.failure.empty() ? measurement.compiled.failure
                                                                          : measurement.interpreted.failure;
        std::printf("%-22s %-7s %11s %s\n", workload.name, variant.name, "FAILED:",
                    reason.empty() ? "did not run" : reason.c_str());
        ++problems;
        continue;
      }

      PrintTableRow(measurement, workload.name, variant.name, csv);
      ++rows;

      // The two arms ran the same program and have to say so. Checked for every row rather than only
      // for the configurations the correctness suite does not cover, because it is the same check and
      // a row that cannot be trusted should not be in the table at all.
      if (!measurement.arms_agree()) {
        std::printf("    ^ WARNING: the two arms printed different output - this row is a timing of two\n");
        std::printf("      different programs and means nothing.\n");
        ++problems;
      }

      // The guard against the whole suite being meaningless. If the compiled arm's entry point has no
      // JIT function then the row above is a ratio between two interpreted runs, which is a very
      // convincing way to report nothing at all.
      if (!measurement.compiled.entry_has_jit) {
        std::printf("    ^ WARNING: the compiled arm had no compiled code - this ratio compares two\n");
        std::printf("      interpreted runs and means nothing.\n");
        ++problems;
      }

      // Printed for every variant except the line-callback one. The *configuration* is what decides
      // which opcodes exist and the two diagnostic lines are how a coverage figure is read back to the
      // instructions behind it, so a row compiled by an engine in a different configuration needs its
      // own. A `linecb` row is the same program as its `plain` twin and would only repeat it.
      if (!csv && !variant.install_line_callback)
        PrintUntranslatedOpcodes(workload, variant, 6);
    }
  }

  if (!csv) {
    PrintSuiteCoverage();
    std::printf("\n%d rows reported", rows);
    if (problems > 0)
      std::printf(", %d problem(s)", problems);
    std::printf("\n");
  }

  return problems == 0 ? 0 : 1;
#endif
}
