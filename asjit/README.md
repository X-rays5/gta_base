# asjit

An AngelScript bytecode JIT. Translates script functions to x86-64 through [AsmJit](https://github.com/asmjit/asmjit) and hands back to the interpreter anything it cannot translate.

**Status: experimental.** See [Benchmarks](#benchmarks) for what it is and is not worth.

## How it works

asjit registers itself as an `asIJITCompilerV2`. The engine calls `NewFunction` once per script function, at the end of the module build that produced it; asjit compiles what it can and publishes an entry point through `SetJITFunction`. Everything declined stays interpreted.

The unit of translation is the basic block: a run beginning at an `asBC_JitEntry` and ending at the first instruction that either cannot be translated or ends the block. Blocks become native code; the code between them does not. That is what makes partial coverage correct rather than merely incomplete - an instruction the translator has no case for ends the block *before* itself, and the interpreter resumes somewhere it can resume from.

Bytecode is only modified once the whole function has compiled, so a failure part-way leaves the function running in the interpreter exactly as it was.

The prologue keeps three values in callee-saved registers:

| Register | Holds |
| --- | --- |
| `rbx` | the live `asSVMRegisters` |
| `r12` | `stackPointer`, as a byte address |
| `r13` | `stackFramePointer`, likewise |

`valueRegister`, `objectRegister` and `objectType` deliberately stay in memory: any instruction that falls back has to observe them there, so caching them natively would mean writing them back before every exit. `programPointer` is not held at all, and that is the win - inside a block the next instruction is a compile-time constant, so the interpreter's per-instruction dispatch disappears.

Calls are not reimplemented. Reaching a registered native means the platform's calling convention and a per-architecture argument shuffler, which is the one part of the engine that is not portable. The translation removes the dispatch *around* the call, so a run of native code no longer ends at every call.

One `JitCompiler` serves the whole process. That is what lets the menu's per-script engines share one pool of executable memory instead of churning pages per script.

## Opcode coverage

Of the 201 opcodes the VM's dispatch table can reach, **199 have a translation and 197 are on by default.** Every term in that sentence is counted against the engine's own `asBCInfo` table rather than asserted:

- 208 `asBCInfo` records, one of which is the `asBCINFO` macro's parameter list, leaving 207.
- 6 of those are metadata the engine consumes while finalizing a module and never dispatches - `Block`, `LABEL`, `LINE`, `ObjInfo`, `TryBlock`, `VarDecl` - leaving 201.
- 179 are cases in `emitter.cpp`, and 20 more are reached before it in `function_compiler.cpp`, which owns control flow, the calls and the two options below.
- The remaining 2 have no translation at all: `STR`, whose interpreter handler is `asASSERT(false)`, and `CALLBND`, which needs an `import` the menu never writes.

The two that are implemented but **off by default** are `asBC_RET` and `asBC_Thiscall1`. Both were written, run against the differential suite and found to agree with the interpreter; both cost more translated than handed back to the interpreter on measurement. They sit behind `Options::translate_returns` and `Options::translate_object_method_calls`, and nothing in the menu sets either. The test suite and the benchmark run under both configurations, so what is being claimed about them is "measured and turned off", not "never written".

## Using it

```cpp
engine->SetEngineProperty(asEP_INCLUDE_JIT_INSTRUCTIONS, true);  // the host's codegen policy
asjit::Attach(engine);                                           // before any module is built
```

`Attach` sets `asEP_JIT_INTERFACE_VERSION` itself, because that property and the registration are one decision: the engine picks which interface it calls from the property. `asEP_INCLUDE_JIT_INSTRUCTIONS` is *not* set by asjit - whether the optimizer emits `asBC_JitEntry` at all is the host engine's policy. Attaching to an engine built without it is not an error; the JIT simply never runs, since a function with no entry points is declined.

`Attach` has to come before the first module is built. The engine compiles a function the moment its module finishes building, so a script loaded before the attach is a script that misses out.

```cpp
asjit::Detach(engine);   // only if the engine outlives the decision to unregister
```

`Detach` hands back everything it compiled for that engine *before* taking the compiler away. A script function's destructor calls back into the engine's compiler whenever it still holds compiled code, without checking the pointer is there, so clearing the compiler first would turn a later module teardown into a null dereference. Not calling `Detach` leaks nothing: an engine shut down while still registered runs the same callbacks from its own destructor.

### Stats

`asjit::GetStats()` returns counters since process start:

| Field | Meaning |
| --- | --- |
| `functions_seen` / `compiled` / `declined` | Every function the engine offered, and what happened to it |
| `blocks_emitted` | Basic blocks turned into native code |
| `functions_cleaned` | Compiled functions the engine has thrown away |
| `instructions_translated` / `instructions_total` | Coverage over every function offered, including declined ones |
| `code_size` | Executable bytes emitted |
| `opcodes_walked` / `translated` / `unimplemented` | Per opcode byte, 256 entries |

The per-opcode arrays are what turn "this script is not faster" into a reason. The opcodes with the highest `walked - translated` are where the time is going, and `unimplemented` separates the two cases that look identical in that difference: an opcode the emitter has no case for is a gap to close, while one that was never handed over sits past the point where its block stopped and needs no work. Reported in descending order, this is usually one entry, and usually a call.

Zero `functions_compiled` means the JIT is doing nothing, which is otherwise indistinguishable from a JIT working perfectly.

## Building

asjit depends on nothing else in this repository, so it can be lifted into its own and consumed with `add_subdirectory` through CPM. It owns the AngelScript build (vendored at v2.38.0) and re-exports the engine headers, the add-ons and the engine library - which is how `menu_core` goes on seeing them.

Requires MSVC and C++23.

```bash
cmake -S . -B cmake-build-release -G Ninja -DCMAKE_BUILD_TYPE=Release -DBASE_BUILD_TOOLS=OFF
cmake --build cmake-build-release --target asjit
```

AsmJit stays private to the library, so `asjit.hpp` includes no AsmJit header. `menu_core` already carries PolyHook's AsmJit 1.17 on its include path, and a second `asmjit/...` root reaching it would make every such include in the project ambiguous.

## Tests

```bash
ctest --test-dir cmake-build-release -R asjit --output-on-failure
```

93 tests in three suites, none of which need the game:

| Suite | Tests | What it does |
| --- | --- | --- |
| `differential_test` | 75 | Runs the same script twice, interpreted and compiled, and compares the output |
| `compiler_test` | 16 | Counters, entry points, options, attach/detach lifecycle |
| `emitter_test` | 2 | Assembles individual instructions |

The differential suite is the one that matters. The expected output is never written by hand - the interpreter is the specification - because the risk with a JIT is a translation that is *nearly* right, and a hand-written expectation is only as trustworthy as the reading of the interpreter that produced it. Scripts are grouped by the instruction handlers they drive, so a failure names the part of the translator to look at.

The suite runs once under the menu's configuration and once with both `Options` flags on, so the two gated translations are covered even though nothing ships using them.

## Benchmarks

`asjit_bench` measures whether the JIT is worth having, per workload class. It is built by default but is not registered with ctest: a timing run belongs to a human deciding something, not to CI, where the numbers would be noise from whatever else the runner was doing.

### Method

Every workload runs twice, on identically configured engines that differ only in whether `asjit::Attach` was called, and what is reported is the ratio. Each arm gets a fresh engine, so the counters are per run.

- **Fastest of N runs** (default 7), with two untimed runs in front. For a CPU-bound measurement the least-disturbed run is the closest to the work itself; an average would carry the scheduler's noise in. Build time uses the same estimator over the same runs.
- **Two checks make a row trustworthy**, both reported through the exit status rather than printed. The two arms have to print identical script output - otherwise the row is a timing of two different programs - and the compiled arm's entry point has to actually carry compiled code, or the "speedup" is a ratio between two interpreted runs.
- **Debug builds are refused at run time.** The interpreter is compiled from source in this project and inherits the build configuration's optimisation flags, while the emitted machine code is unaffected by them, so a Debug run would measure unoptimised interpreter against optimised native code.

Each workload's script is run under one or more engine configurations. `plain` is the menu's; the others are deliberate departures, and a row is only readable next to its `plain` twin.

| Config | What changes |
| --- | --- |
| `plain` | Nothing - the menu's configuration |
| `linecb` | A line callback, so every statement is a hand-off point. The watchdog configuration, and the worst case |
| `intdiv` | Integer division is permitted, which makes the four integer division handlers reachable |
| `noopt` | The bytecode optimizer is off, so the translator is handed shapes the menu's engine never emits |
| `fullcov` | Both `Options` flags on - the two instructions handed back by default are translated instead |

### Running it

```bash
cmake --build cmake-build-release --target asjit_bench
cmake-build-release/asjit/bench/asjit_bench.exe                    # full suite
cmake-build-release/asjit/bench/asjit_bench.exe --filter tick      # by substring
cmake-build-release/asjit/bench/asjit_bench.exe --reps 15 --csv    # more reps, machine-readable
```

### Results

Release, MSVC 19.51, AMD Ryzen 7 9800X3D, Windows 11. Default of 7 repetitions, fastest reported. 82 rows over 63 workloads and 5 configurations; exit status 0, so no row failed either sanity check.

| Workload | Cfg | Interp ms | JIT ms | Speedup | Instr cov | Build ±ms | Blocks | Code B |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| arith_int | plain | 480.860 | 41.667 | 11.54x | 89.1% | +0.035 | 17 | 775 |
| arith_int | linecb | 693.153 | 715.984 | 0.97x | 89.1% | +0.041 | 17 | 775 |
| arith_float | plain | 382.847 | 46.110 | 8.30x | 89.8% | +0.032 | 18 | 854 |
| arith_double | plain | 328.274 | 53.100 | 6.18x | 90.6% | +0.032 | 18 | 889 |
| int64_arith | plain | 210.374 | 58.400 | 3.60x | 91.5% | +0.025 | 19 | 932 |
| uint_arith | plain | 192.989 | 36.182 | 5.33x | 91.1% | +0.033 | 22 | 928 |
| compare_branch | plain | 429.947 | 45.517 | 9.45x | 91.5% | +0.034 | 24 | 1040 |
| float_compare | plain | 341.665 | 36.618 | 9.33x | 93.2% | +0.039 | 31 | 1285 |
| bitwise_shift | plain | 600.823 | 48.733 | 12.33x | 91.8% | +0.022 | 20 | 958 |
| convert | plain | 445.504 | 113.442 | 3.93x | 93.7% | +0.047 | 24 | 1170 |
| nested_loop | plain | 397.102 | 22.488 | 17.66x | 91.5% | +0.019 | 26 | 974 |
| globals | plain | 398.810 | 24.368 | 16.37x | 90.0% | +0.020 | 19 | 971 |
| many_locals | plain | 211.664 | 13.754 | 15.39x | 94.4% | +0.025 | 33 | 1407 |
| member_access | plain | 348.077 | 187.717 | 1.85x | 83.2% | +0.056 | 38 | 2714 |
| member_access | linecb | 436.912 | 457.236 | 0.96x | 83.2% | +0.046 | 38 | 2714 |
| handle_guards | plain | 177.745 | 83.772 | 2.12x | 81.9% | +0.056 | 33 | 2392 |
| native_calls | plain | 49.550 | 26.153 | 1.89x | 85.7% | +0.007 | 15 | 675 |
| native_calls | linecb | 59.932 | 77.094 | 0.78x | 85.7% | +0.035 | 15 | 675 |
| native_calls | fullcov | 57.325 | 29.442 | 1.95x | 91.4% | +0.033 | 17 | 713 |
| generic_calls | plain | 107.846 | 63.900 | 1.69x | 92.5% | +0.032 | 23 | 1182 |
| generic_calls | linecb | 127.243 | 136.828 | 0.93x | 92.5% | +0.031 | 23 | 1182 |
| script_calls | plain | 70.846 | 52.057 | 1.36x | 82.3% | +0.027 | 23 | 1076 |
| script_calls | linecb | 98.501 | 121.799 | 0.81x | 82.3% | +0.034 | 23 | 1076 |
| script_calls | fullcov | 71.095 | 80.396 | 0.88x | 88.7% | +0.034 | 24 | 1221 |
| array_methods | plain | 13.998 | 9.925 | 1.41x | 81.3% | +0.025 | 28 | 1808 |
| array_index | plain | 69.247 | 37.423 | 1.85x | 81.7% | +0.042 | 39 | 2180 |
| array_index | fullcov | 67.996 | 91.696 | 0.74x | 92.7% | +0.064 | 40 | 2586 |
| string_build | plain | 3.207 | 2.250 | 1.43x | 93.4% | -0.022 | 29 | 1605 |
| handles | plain | 226.000 | 168.155 | 1.34x | 83.8% | +0.065 | 45 | 4865 |
| handles_nocount | plain | 144.328 | 9.540 | 15.13x | 94.4% | +0.008 | 24 | 1942 |
| value_objects | plain | 209.787 | 54.092 | 3.88x | 95.6% | +0.037 | 28 | 2010 |
| division | plain | 107.723 | 17.588 | 6.12x | 90.0% | +0.002 | 18 | 884 |
| pow_double | plain | 46.088 | 23.864 | 1.93x | 93.3% | +0.011 | 24 | 1404 |
| pow_int | plain | 80.169 | 14.875 | 5.39x | 95.3% | +0.010 | 34 | 2043 |
| switch_heavy | plain | 99.906 | 57.334 | 1.74x | 89.9% | +0.044 | 30 | 1324 |
| bool_values | plain | 376.609 | 37.203 | 10.12x | 91.0% | +0.058 | 65 | 3233 |
| short_circuit | plain | 329.926 | 24.789 | 13.31x | 96.5% | +0.013 | 63 | 1850 |
| expression_depth | plain | 225.359 | 19.180 | 11.75x | 92.6% | +0.010 | 21 | 1025 |
| recursion | plain | 64.538 | 42.651 | 1.51x | 87.8% | +0.029 | 26 | 1236 |
| recursion | fullcov | 68.606 | 60.578 | 1.13x | 91.9% | +0.031 | 28 | 1316 |
| funcdef_callback | plain | 52.583 | 34.169 | 1.54x | 89.8% | +0.043 | 31 | 2401 |
| array_literal | plain | 16.217 | 14.077 | 1.15x | 84.9% | +0.036 | 24 | 2128 |
| dictionary_literal | plain | 74.366 | 78.433 | 0.95x | 95.8% | +0.065 | 31 | 2688 |
| try_catch | plain | 9.461 | 0.555 | 17.04x | 87.0% | +0.037 | 22 | 862 |
| try_catch_throwing | plain | 87.604 | 100.664 | 0.87x | 81.2% | +0.042 | 29 | 2013 |
| string_compare | plain | 23.848 | 14.570 | 1.64x | 95.0% | +0.036 | 40 | 2026 |
| dictionary_access | plain | 134.645 | 110.179 | 1.22x | 96.8% | +0.047 | 52 | 3191 |
| do_while_break | plain | 108.672 | 8.560 | 12.70x | 94.0% | +0.016 | 34 | 1296 |
| modulo | plain | 76.350 | 9.969 | 7.66x | 94.0% | -0.006 | 25 | 1456 |
| negation | plain | 112.135 | 11.514 | 9.74x | 94.1% | +0.001 | 25 | 1291 |
| float_division | plain | 131.523 | 11.212 | 11.73x | 93.4% | +0.004 | 29 | 1293 |
| int64_bitwise | plain | 183.962 | 37.282 | 4.93x | 95.0% | +0.025 | 33 | 1466 |
| convert_64 | plain | 259.754 | 39.975 | 6.50x | 96.2% | +0.034 | 31 | 1893 |
| narrow_ints | plain | 232.443 | 27.407 | 8.48x | 96.3% | +0.031 | 34 | 1846 |
| handle_parameter | plain | 89.666 | 51.864 | 1.73x | 83.3% | +0.046 | 36 | 2701 |
| handle_parameter | linecb | 107.888 | 127.746 | 0.84x | 83.3% | +0.049 | 36 | 2701 |
| increments | plain | 202.394 | 16.064 | 12.60x | 95.9% | +0.016 | 39 | 1801 |
| mixed_numeric | plain | 212.629 | 35.117 | 6.05x | 95.2% | +0.031 | 34 | 1804 |
| wide_globals | plain | 89.123 | 6.810 | 13.09x | 93.6% | +0.008 | 24 | 1236 |
| by_reference | plain | 66.875 | 5.065 | 13.20x | 91.4% | +0.005 | 31 | 1486 |
| by_reference | linecb | 112.886 | 96.160 | 1.17x | 91.4% | +0.040 | 31 | 1486 |
| handle_conversion | plain | 45.926 | 56.519 | 0.81x | 83.1% | +0.074 | 56 | 4819 |
| handle_conversion | linecb | 71.075 | 75.362 | 0.94x | 83.1% | +0.056 | 56 | 4819 |
| globals_compound | plain | 69.796 | 16.076 | 4.34x | 93.5% | +0.017 | 26 | 1297 |
| reference_return | plain | 69.974 | 46.217 | 1.51x | 89.8% | +0.032 | 19 | 926 |
| cast_heavy | plain | 1536.085 | 1504.517 | 1.02x | 75.9% | +0.075 | 57 | 4759 |
| tick_spawn_vehicle | plain | 90.507 | 69.660 | 1.30x | 96.5% | +0.046 | 52 | 2891 |
| tick_spawn_vehicle | linecb | 101.365 | 118.313 | 0.86x | 96.5% | +0.046 | 52 | 2891 |
| tick_with_arithmetic | plain | 656.629 | 70.788 | 9.28x | 91.1% | +0.036 | 36 | 1647 |
| tick_with_switch | plain | 21.793 | 16.574 | 1.31x | 90.4% | +0.029 | 31 | 1582 |
| tick_with_state | plain | 25.450 | 14.210 | 1.79x | 89.3% | +0.040 | 39 | 3268 |
| many_short_ticks | plain | 32.660 | 24.286 | 1.34x | 84.1% | +0.023 | 25 | 1276 |
| many_short_ticks | fullcov | 32.689 | 29.008 | 1.13x | 89.9% | +0.044 | 26 | 1421 |
| script_object_loop | plain | 58.402 | 70.280 | 0.83x | 85.3% | +0.051 | 28 | 1891 |
| script_object_loop | linecb | 69.213 | 115.775 | 0.60x | 85.3% | +0.056 | 28 | 1891 |
| delegate_call | plain | 10.170 | 6.334 | 1.61x | 83.7% | +0.029 | 28 | 1938 |
| delegate_call | linecb | 13.226 | 17.091 | 0.77x | 83.7% | +0.044 | 28 | 1938 |
| delegate_call | fullcov | 10.607 | 8.116 | 1.31x | 89.5% | +0.035 | 30 | 2102 |
| int_divide | plain | 280.007 | 43.970 | 6.37x | 96.7% | +0.046 | 31 | 2569 |
| int_divide | intdiv | 223.757 | 33.011 | 6.78x | 96.0% | +0.025 | 31 | 2190 |
| unoptimized | plain | 21.318 | 3.188 | 6.69x | 93.5% | +0.058 | 71 | 5259 |
| unoptimized | noopt | 33.583 | 4.448 | 7.55x | 95.4% | +0.069 | 74 | 6872 |

### What the numbers say

Across the 63 default-configuration rows the median is **4.3x** and the range is **0.81x to 17.7x**. The spread is the finding, not the median.

- **Compute-heavy loops that never hand off** are where the JIT wins outright: 13-18x on `nested_loop`, `try_catch`, `globals`, `many_locals`, `handles_nocount`, `by_reference`, `wide_globals`. None of these resemble a menu script.
- **Call-heavy scripts are 1.3-1.9x.** This is the shape the menu's scripts actually have. The win is bounded by the fact that the calls themselves still go through the engine.
- **Object lifecycle loses.** `handle_conversion` (0.81x), `script_object_loop` (0.83x), `cast_heavy` (1.02x), `dictionary_literal` (0.95x) construct, destroy, refcount or cast - all of which fall back to the interpreter, so the compiled path pays the transition without removing the work.
- **The line-callback configuration is a loss on 10 of 11 rows** (median 0.86x, worst 0.60x). A callback on every statement makes every statement a hand-off point, which is the per-block entry cost with almost none of the block. The menu installs no line callback, so this is the watchdog case rather than the shipped one.
- **The `fullcov` rows confirm leaving both `Options` off.** Of the six rows with them on, only `native_calls` improves (1.89x to 1.95x); `script_calls` goes 1.36x to 0.88x and `array_index` 1.85x to 0.74x. These rows are the measurement the two defaults were set from.
- **Instruction coverage is 76-97%, median 91.5%.** On the high-coverage rows the residual is `asBC_RET` - one instruction, but the one a call-heavy script is mostly made of. The rows down in the low 80s are the object and array ones, where a larger share of the function sits past a fall-back. High coverage next to a low ratio is a normal result: a translated multiply and an untranslated call are one instruction each and do not cost anything like the same.
- **Build cost is +0.034 ms median, +0.075 ms worst.** That is the entire one-time cost, and it lands outside both timing columns - the engine calls the translator while it compiles each function, not when one is first entered. A script built once and run for a minute amortises it to nothing; one rebuilt per tick may not.

### Reading a row

Both timing columns come from the same process, so the *ratio* is insulated from whatever else the machine was doing. The two columns are not equally insulated from where the linker put things: rebuilding the benchmark with a one-line change to the library has moved the interpreted column of one row by 44% while the compiled column moved 3%. Within one binary the run-to-run spread is about 0.2-0.5% at the median and up to 6-8% on individual rows.

So: compare rows within a single run, and runs of one binary against each other. Do not compare a run against a run of a different build. A single row moving by a few percent is not a result.

The per-row `untranslated:` and `no translation:` lines name the opcodes behind a coverage figure. An opcode on the first line but not the second was never handed over - it sits past where its block stopped - so it is not a gap to close, and the second line is where the work is. Across the whole suite the benchmark reports **197 distinct opcodes translated and 2 handed over and refused** (`RET`, `Thiscall1` - both translated by their `fullcov` rows, both refused by the rows running the menu's configuration).

## Layout

| Path | |
| --- | --- |
| `src/asjit.hpp` | The only header a consumer needs. Free of AsmJit by design |
| `src/abi.hpp` | The register plan and the offsets into `asSVMRegisters`, asserted against the vendored SDK's own layout |
| `src/bytecode.hpp` | An instruction as the interpreter sees it, over the engine's own argument macros |
| `src/emitter.cpp` | The instruction translations - one large switch, one case per opcode |
| `src/function_compiler.cpp` | Control flow, block boundaries, calls, and the two `Options` |
| `src/runtime.cpp` | The parts of the VM that emitted code calls back into |
| `src/jit_compiler.cpp` | The `asIJITCompilerV2` the engine calls, and the code it owns |
| `support/` | The one definition of a correctly configured engine, shared by the tests and the benchmark |
| `test/` | The differential, compiler and emitter suites |
| `bench/` | The workloads and the comparison harness |
