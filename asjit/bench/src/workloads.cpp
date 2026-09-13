//
// Created by X-ray on 13/09/2026.
//

#include "workloads.hpp"

#include <cstdint>
#include <string>

namespace base::asjit::bench {
  namespace {
    // ---------------------------------------------------------------------
    // Stubs standing in for the menu's native API
    // ---------------------------------------------------------------------

    std::int64_t g_stub_state = 0;

    int StubPlayerPedId() { return 1; }
    int StubHash(const std::string& text) { return static_cast<int>(text.size()); }
    void StubRequestModel(int /*model*/) {}
    int StubHasModelLoaded(int /*model*/) { return 1; }
    void StubThreadYield() { ++g_stub_state; }
    void StubLog(const std::string& /*message*/, int value) { g_stub_state += value; }

    // The menu's loggers, which are generic and variadic and which not one of the stubs beside it was.
    // Every other registration below is `asCALL_CDECL` with a fixed argument list, so the block comment
    // in `workloads.hpp` claiming the stubs stand in for natives registered "`asCALL_CDECL` or
    // `asCALL_GENERIC`" was true of one half of the disjunction and had never been true of the other.
    //
    // The shape cannot be registered the other way: a variadic declaration is refused outright outside
    // the generic convention (as_scriptengine.cpp:2986), so generic is the only convention under which a
    // `log::info(fmt, ...)` exists - and with it the engine's variadic marshalling branch, which builds
    // an `asCGenericVariadic` rather than an `asCGeneric` and takes the pop size from the argument count
    // the call actually carried (as_context.cpp:5865).
    //
    // `...` replaces the *name* of the last parameter rather than following it (as_parser.cpp:783). The
    // extra arguments arrive with nothing that says how many there are, so the count comes from
    // `GetArgCount()`, which for a variadic call reports what the call carried rather than what the
    // declaration lists.
    void StubLogFormat(AngelScript::asIScriptGeneric* generic) {
      const int count = generic->GetArgCount();

      int total = 0;
      for (int i = 1; i < count; i++)
        total += static_cast<int>(generic->GetArgDWord(static_cast<unsigned int>(i)));

      g_stub_state += total;
    }
    float StubGetX(int /*entity*/) { return 1.0f; }
    float StubGetY(int /*entity*/) { return 2.0f; }
    float StubGetZ(int /*entity*/) { return 3.0f; }
    float StubGetHeading(int /*entity*/) { return 90.0f; }
    int StubCreateVehicle(int model, float x, float y, float z, float heading) {
      return model + static_cast<int>(x + y + z + heading);
    }
    void StubSetModelAsNoLongerNeeded(int /*model*/) {}

    // One plain call per iteration, which is the shape a block ends on.
    void StubTouch(int /*value*/) {}

    // ---------------------------------------------------------------------
    // Synthetic - the ceiling for each opcode class
    // ---------------------------------------------------------------------

    // The control. Deliberately the same shape as the number this benchmark was written to check:
    // a tight integer loop with an arithmetic body and nothing that falls back.
    constexpr const char* kArithInt = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += i * 3 - (i >> 2) + 7;
        }
        return total;
      }
      void main() { show(Work(30000000)); }
    )AS";

    constexpr const char* kArithFloat = R"AS(
      float Work(int iterations) {
        float total = 0.0f;
        for (int i = 0; i < iterations; i++) {
          float f = float(i);
          total += f * 1.5f - f * 0.25f;
        }
        return total;
      }
      void main() { show(Work(20000000)); }
    )AS";

    constexpr const char* kArithDouble = R"AS(
      double Work(int iterations) {
        double total = 0.0;
        for (int i = 0; i < iterations; i++) {
          double d = double(i);
          total += d * 1.5 - d * 0.25;
        }
        return total;
      }
      void main() { show(Work(15000000)); }
    )AS";

    // Branches both ways, so that both the taken and the not-taken edge are exercised rather than
    // the predictor learning one of them.
    constexpr const char* kCompareBranch = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          if (i < iterations / 2) {
            total += i;
          } else {
            total -= i;
          }
        }
        return total;
      }
      void main() { show(Work(20000000)); }
    )AS";

    constexpr const char* kBitwiseShift = R"AS(
      uint Work(int iterations) {
        uint acc = 0x9E3779B9;
        for (int i = 0; i < iterations; i++) {
          uint v = uint(i);
          acc = (acc ^ (v << 3)) & 0xFFFFFFFF;
          acc = (acc | (v >> 2)) ^ (~v);
          acc = (acc + v) & 0x7FFFFFFF;
        }
        return acc;
      }
      void main() { show(Work(20000000)); }
    )AS";

    // Every conversion instruction the translator covers, in a loop so the cost dominates the setup.
    constexpr const char* kConvert = R"AS(
      int Work(int iterations) {
        int acc = 0;
        for (int i = 0; i < iterations; i++) {
          float f = float(i);
          double d = double(f);
          int back = int(d);
          int8 sb = int8(back);
          uint16 uw = uint16(back);
          uint u = uint(sb) + uint(uw);
          acc += int(u);
          acc += int(float(back));
        }
        return acc;
      }
      void main() { show(Work(10000000)); }
    )AS";

    constexpr const char* kNestedLoop = R"AS(
      int Work(int outer) {
        int total = 0;
        for (int i = 0; i < outer; i++) {
          for (int j = 0; j < 8; j++) {
            total += i * j;
          }
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    constexpr const char* kGlobals = R"AS(
      int g_counter = 0;
      int g_accumulator = 0;

      void Work(int iterations) {
        for (int i = 0; i < iterations; i++) {
          g_counter = g_counter + i;
          g_accumulator = g_accumulator - 1;
        }
      }
      void main() {
        Work(20000000);
        show(g_counter);
        show(g_accumulator);
      }
    )AS";

    // The 64-bit opcodes are a separate translation from the 32-bit ones - a different instruction
    // for every operation, and a different set of conversions around them - so a fast 32-bit loop
    // says nothing about this one.
    constexpr const char* kInt64Arith = R"AS(
      int64 Work(int iterations) {
        int64 acc = 123456789;
        for (int i = 0; i < iterations; i++) {
          int64 v = int64(i);
          acc = acc * 3 - (acc >> 7) + v;
          acc = acc ^ (v << 9);
        }
        return acc;
      }
      void main() { show(Work(8000000)); }
    )AS";

    // Unsigned arithmetic and comparison, which are the same instructions for addition and different
    // ones for everything that has to know about the sign - division, shifts, and the comparisons.
    constexpr const char* kUintArith = R"AS(
      uint Work(int iterations) {
        uint acc = 33554432;
        for (uint i = 0; i < uint(iterations); i++) {
          acc = acc * 1103515245 + 12345;
          if (acc > 2147483648) {
            acc = acc ^ (acc >> 3);
          }
        }
        return acc;
      }
      void main() { show(Work(8000000)); }
    )AS";

    // Member access through an object, which is what asBC_LoadThisR and asBC_LoadRObjR are for and
    // what every method body in a real script is mostly made of. Writes as well as reads, because the
    // address is computed the same way for both and only the write proves it was computed right.
    constexpr const char* kMemberAccess = R"AS(
      class Body {
        int value = 1;
        int weight = 3;

        int Bump() {
          value = value * 3 + weight;
          weight = weight + 1;
          return value;
        }

        int Get() const { return value + weight; }
      }

      int Work(int iterations) {
        Body b;
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += b.Bump();
          total = total ^ b.Get();
        }
        return total;
      }
      void main() { show(Work(6000000)); }
    )AS";

    // Float comparisons rather than float arithmetic. The interpreter's comparison produces -1/0/1 by
    // branching on `<`, and the translation has to reproduce that rather than use the flags a
    // subtract would set, because the two disagree about NaN and about infinities.
    constexpr const char* kFloatCompare = R"AS(
      int Work(int iterations) {
        int hits = 0;
        float a = 1.0f;
        float b = 2.0f;
        for (int i = 0; i < iterations; i++) {
          float f = float(i) * 0.5f;
          if (f < a) {
            hits += 1;
          } else if (f > b) {
            hits -= 1;
          }
          a = a + 0.5f;
          b = b + 0.25f;
        }
        return hits;
      }
      void main() { show(Work(12000000)); }
    )AS";

    // Sixteen live variables, so that the frame is wide enough for the variable offsets to matter.
    // A translation that scaled a frame offset wrongly would read a neighbouring local here and
    // nowhere else.
    constexpr const char* kManyLocals = R"AS(
      int Work(int iterations) {
        int a = 1;
        int b = 2;
        int c = 3;
        int d = 4;
        int e = 5;
        int f = 6;
        int g = 7;
        int h = 8;
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          a = a + i;  b = b ^ a;  c = c + b;  d = d - c;
          e = e + d;  f = f ^ e;  g = g + f;  h = h - g;
          total = total + h;
        }
        return total;
      }
      void main() { show(Work(6000000)); }
    )AS";

    // ---------------------------------------------------------------------
    // Where the JIT may not pay
    // ---------------------------------------------------------------------

    // The shape the recon says real scripts actually have: a loop whose body is a call. The block
    // ends at the call every iteration, so this measures the round-trip rather than the arithmetic.
    constexpr const char* kNativeCalls = R"AS(
      void Work(int iterations) {
        for (int i = 0; i < iterations; i++) {
          StubTouch(i);
        }
      }
      void main() { Work(2000000); }
    )AS";

    // Array indexing, length and insertLast are all method calls, so the body of this loop is
    // almost entirely interpreter work.
    constexpr const char* kArrayMethods = R"AS(
      void Work(int iterations) {
        array<int> values;
        for (int i = 0; i < iterations; i++) {
          values.insertLast(i);
          if (values.length() > 16) {
            values.removeAt(0);
          }
        }
      }
      void main() { Work(200000); }
    )AS";

    // Floating-point division, which *is* translated now. The guard is the zero-divisor case, and it
    // is emitted out of line, so the working path pays a not-taken branch and nothing else - which is
    // exactly what this case exists to measure, since the interesting number is the cost of the guard
    // on a loop that never takes it.
    //
    // A divisor of exactly zero would be the other half of that, and cannot be measured here: the
    // interpreted arm raises on it, so the row would be a comparison of two exceptions.
    constexpr const char* kDivision = R"AS(
      double Work(int iterations) {
        double total = 0.0;
        for (int i = 1; i < iterations; i++) {
          double d = double(i);
          total += 1000.0 / d;
        }
        return total;
      }
      void main() { show(Work(5000000)); }
    )AS";

    // POD value objects, assigned to and passed by value. These are a shape the menu has in quantity -
    // every `Vector3` a native hands back is one - and they are the only case here that produces
    // asBC_COPY.
    //
    // Twelve bytes is deliberate: it is under the limit below which the translator unrolls a copy
    // into straight-line moves instead of handing it to `rep movsb`, so this row measures the case
    // that actually occurs in a script. The cost it does *not* measure is the prologue parking rdi
    // and rsi, which it does for any function containing a COPY whether or not the copy reaches the
    // string instructions - that happens once per hand-off rather than once per copy, and a loop this
    // long buries it.
    //
    // `Vec3` is a registered type, not one declared here. A class declared in the script would be
    // given an auto-generated copy operator by the compiler, the copy would become a call, and this
    // workload would silently measure a different instruction than the one it names.
    //
    // The loop copies a whole value from another every iteration - which is the copy the instruction
    // exists for - and reads and writes the components either side of it, so the compiled block has
    // something to do with the result.
    //
    // There is deliberately no call inside the loop. The shapes that would put one there are the
    // obvious ones - a by-value parameter, an operator overload, both of which are how a script
    // normally does vector arithmetic - and every one of them is a call the translator does not
    // handle, so the block would end at the call and the row would report the cost of the hand-off
    // rather than the cost of the copy it names. Those shapes are covered by native_calls and
    // script_calls, where they belong; the thing that is unique to this row is the copy.
    constexpr const char* kValueObjects = R"AS(
      float Work(int iterations) {
        Vec3 a(1.0f, 2.0f, 3.0f);
        Vec3 b(0.5f, 0.25f, 0.125f);

        float total = 0.0f;
        for (int i = 0; i < iterations; i++) {
          Vec3 sum = a;
          sum.x = sum.x + b.x;
          sum.y = sum.y + b.y;
          sum.z = sum.z + b.z;
          total += sum.x * sum.y + sum.z;

          a = sum;
          b.x = b.x + 0.0001f;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // Handles: assigning one object reference to another, which is `asBC_REFCPY`.
    //
    // This is the most common shape in a menu script that no other row here covers - a `Player@` or a
    // `Vehicle@` passed around, stored, and compared - and it was the row that used to price a gap:
    // the instruction carries the object type, but the release and addref it must call are named by
    // *index into the engine's private function table*, not by address, so the first version of this
    // component could not reach them and the row's coverage was capped by exactly that.
    //
    // It is translated now, and not by reaching the table: the engine exports `AddRefScriptObject` and
    // `ReleaseScriptObject`, which take the type and the pointer and do the same two things the
    // handler does - its own two lines, in its own order. So the ratio this row prints is no longer a
    // price for a gap, which is why the number moved and why this comment had to.
    //
    // Two assignment sites rather than one, because they are not the same instruction. Storing into a
    // *local* handle is a plain pointer move the compiler emits inline, and it does not appear here or
    // in the diagnostics; storing into a global or a member is what goes through the refcounting
    // instruction, so the loop writes through both. The method call is the third hand-off and the
    // reason the entity is a class rather than a bare handle: it is what a script does with one.
    //
    // Every assignment is written with the `@` operator, and that is not decoration. Without it the
    // compiler does not assign the handle at all - it reads the source handle, checks it for null
    // (`asBC_ChkNullV`) and then *copies the object* into whatever the destination handle points at,
    // through the engine's default copy operator for script classes. A destination that is still null
    // makes that copy raise `Null pointer access` on the first iteration, in the interpreter as much
    // as in the JIT, so the script would not run at all; and even where it did run it would compile to
    // `asBC_CALLSYS` rather than to `asBC_REFCPY`, so the row would not be measuring the instruction it
    // names. `@` is the handle assignment, and this row is about the handle assignment.
    //
    // `JitCompiler.HandleAssignmentIsRefusedForALackOfReach` pins the shape this claims to exercise,
    // so a change to this script that made it stop producing the instruction fails there rather than
    // quietly turning this row into a duplicate of `script_calls`.
    constexpr const char* kHandles = R"AS(
      class Entity {
        int id = 0;
        float value = 0.0f;

        void Bump(int delta) {
          id += delta;
          value += float(delta) * 0.5f;
        }
      }

      class Holder {
        Entity@ entity;
      }

      Entity@ g_current;

      float Work(int iterations) {
        Holder holder;
        @holder.entity = Entity();
        @g_current = holder.entity;

        float total = 0.0f;
        for (int i = 0; i < iterations; i++) {
          holder.entity.Bump(i);
          @g_current = holder.entity;
          total += g_current.value;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // The same assignment over a type the interpreter does *not* reference-count, which is the half of
    // the instruction that is a plain store rather than two engine calls.
    //
    // `asBC_REFCPY` and `asBC_RefCpyV` branch on the object type, and the two halves are translated
    // differently rather than one being translated and the other refused. A counted type needs
    // `addref` and `release`, which are reached by handing the type and the pointer to the engine's own
    // `AddRefScriptObject`/`ReleaseScriptObject`; a type carrying `asOBJ_NOCOUNT` needs neither, and
    // the handler is then one pointer store, which is emitted directly with no call at all. `Handle` in
    // the harness is that registration; the row above is its counted counterpart, and reading the two
    // side by side is the point of having both.
    //
    // The loop reads the handle's `value` *property* rather than calling `Value()`, and that is a
    // deliberate choice about what gets measured rather than a matter of taste. A method call is
    // `asBC_CALLSYS`, which is translated but still ends the block - the engine builds the frame and
    // the VM resumes the callee - so a body built around one would be measuring the call on every
    // iteration and would say little about the assignment this row was added for. A property is a load
    // and a displacement, so the block survives the whole body and the four assignment forms are what
    // the ratio is about.
    //
    // Four assignments rather than one because they are not one instruction: `@local = other` is the
    // named-destination form, and storing into the global is what puts a destination pointer on the
    // stack for the other. Both are in the body so that neither translation is measured by itself.
    //
    // A ratio above 1.0 here is the expected result, and was not before this translation existed - so
    // the row is also the regression detector for it. If the counted-type condition were ever misread
    // and the no-count half stopped being translated, this is the row that would move, and
    // `Differential.NoCountHandleAssignmentIsTranslated` is the one that would fail.
    constexpr const char* kHandlesNoCount = R"AS(
      Handle@ g_slot;

      int Work(int iterations) {
        Handle@ a = Handle(1);
        Handle@ b = Handle(2);
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          @a = b;
          @g_slot = a;
          total += a.value;
          @b = a;
        }
        return total + g_slot.value;
      }
      void main() { show(Work(5000000)); }
    )AS";

    // The floating point exponents. Unlike every other arithmetic case here, the operation is a call
    // to the CRT's `pow` on both sides, so what the JIT can win is the dispatch around it rather than
    // the arithmetic - and the guard on the result is out of line, so a loop whose results are all
    // finite pays a not-taken branch for it.
    //
    // All three translated forms are in the body because they read their operands at three different
    // widths, and a workload that covered one of them would leave the other two unmeasured. The base
    // is nudged upward every iteration so the compiler cannot treat either operand as a constant, and
    // it stays near one so that every result is finite: an overflow raises in the interpreted arm, and
    // the row would be a comparison of two exceptions.
    constexpr const char* kPowDouble = R"AS(
      double Work(int iterations) {
        double total = 0.0;
        double base = 1.0001;
        double exponent = 3.0;
        int intExponent = 3;
        float small = 1.5f;
        for (int i = 0; i < iterations; i++) {
          total += base ** exponent;
          total += base ** intExponent;
          total += small ** 2.0f;
          base += 0.0000001;
        }
        return total;
      }
      void main() { show(Work(1000000)); }
    )AS";

    // The integer exponent, in all four widths. This row used to price a refusal; it now prices the
    // translation that replaced it, and the distinction matters for reading its ratio.
    //
    // The four integer forms are translated by *calling the interpreter's own routine* rather than by
    // reimplementing it, and that is the whole design rather than a shortcut around it. An integer
    // power is not `pow` narrowed to an integer: it carries its own overflow rule, and the rule is a
    // table of maximum bases per exponent (`pow_info`, as_context.cpp:6479) whose entries are data
    // rather than arithmetic. `MaxBaseu64` for exponent 2 is 2147483647, which is not
    // `floor(sqrt(2^64-1))`, and the 32-bit pair reads transposed against the values their names
    // suggest. A translation that derived the bound instead of asking would raise where the
    // interpreter returns a number, which is the one failure mode a JIT cannot have.
    //
    // So the emitted code loads its operands, calls `as_powi` and its three siblings, and branches to
    // the fault exit when the routine reports an overflow - at which point the interpreter re-runs the
    // instruction, reaches the same verdict and raises. `Differential.IntegerExponentMatchesTheInterpreter`
    // and `Differential.IntegerExponentOverflowRaisesLikeTheInterpreter` are the two halves of that:
    // agreement on values, and agreement on the raise.
    //
    // What this row therefore measures is a real function call per iteration on *both* sides - the
    // interpreter's handler does exactly the same thing, which is why it hands the operands to the same
    // routine. The ratio here is the honest price of an integer power under each, not a translation
    // that outthought the interpreter, and a ratio near 1.0 is the correct result rather than a
    // disappointment. The row earns its place by being the only one that puts all four of these
    // opcodes under the translator at once - before it did, the suite produced `POWi` alone and the
    // other three were present but never timed.
    //
    // The bases cycle through single digits and the exponents are three, so every result fits and every
    // running total stays well inside its type. The magnitudes are small on purpose: this measures the
    // instruction, not the numbers.
    //
    // The four accumulators are separate rather than folded into one so that each width's result has to
    // be produced and printed, which is what stops the compiler from simplifying away a form whose
    // result nothing read.
    constexpr const char* kPowInt = R"AS(
      void Work(int iterations) {
        int itotal = 0;
        uint utotal = 0;
        int64 i64total = 0;
        uint64 u64total = 0;
        int exponent = 3;
        int64 exponent64 = 3;
        uint64 exponentu64 = 3;
        for (int i = 0; i < iterations; i++) {
          int base = (i & 7) + 3;
          uint ubase = uint(i & 7) + 3;
          itotal += base ** exponent;
          utotal += ubase ** uint(exponent);
          i64total += int64(base) ** exponent64;
          u64total += uint64(ubase) ** exponentu64;
        }
        show(itotal);
        show(utotal);
        show(i64total);
        show(u64total);
      }
      void main() { Work(2000000); }
    )AS";

    // A switch compiles to asBC_JMPP followed by a run of asBC_JMP, which the translation turns into
    // a compare chain. The selector is masked so that both the case arms and the default arm are
    // reached, rather than one arm being predicted perfectly.
    constexpr const char* kSwitchHeavy = R"AS(
      int Classify(int value) {
        switch (value % 4) {
          case 0: return 10;
          case 1: return 20;
          case 2: return 30;
          default: return 40;
        }
      }
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += Classify(i);
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // A call from one script function to another. asBC_CALL is the one instruction a translated block
    // cannot swallow - the callee may execute script, so control has to leave - and this is the case
    // that isolates where a real script's time goes when it is not calling natives.
    //
    // Two levels deep rather than one, so the callee is itself entered through compiled code and the
    // measurement includes the hand-off on both sides rather than only the caller's.
    constexpr const char* kScriptCalls = R"AS(
      int Leaf(int v) { return v * 3 + 1; }
      int Middle(int v) { return Leaf(v) - 2; }

      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += Middle(i);
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";

    // Strings. Every operation on one is a system call - the buffer is allocated, the operator is
    // dispatched through the engine, and the result is reference counted - so this is the case where
    // the least of the work is translated and the arithmetic that is compiled is a rounding error.
    constexpr const char* kStringBuild = R"AS(
      int Work(int iterations) {
        string text = "";
        for (int i = 0; i < iterations; i++) {
          text = "abc";
          text += "defgh";
          if (text.length() > 100) {
            text = "";
          }
        }
        return int(text.length());
      }
      void main() { show(Work(50000)); }
    )AS";

    // Array element access. The indexing operator is a registered method rather than an instruction,
    // so a loop over an array is a loop of calls - but the addressing arithmetic around each call is
    // compiled, which is the part this case measures.
    constexpr const char* kArrayIndex = R"AS(
      int Work(int iterations) {
        array<int> values;
        for (int i = 0; i < 64; i++) {
          values.insertLast(i);
        }

        int total = 0;
        for (int i = 0; i < iterations; i++) {
          int index = i & 63;
          values[index] = values[index] + 1;
          total += values[index];
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";

    // Handles. Every one of these dereferences is guarded natively, and the guard passes every time -
    // so this is the cost of the null checks rather than the cost of the failure. A handle that was
    // actually null would raise in both arms and measure nothing.
    constexpr const char* kHandleGuards = R"AS(
      class Node {
        int value = 0;
        int Bump() {
          value = value + 1;
          return value;
        }
      }

      int Work(int iterations) {
        Node@ held = Node();
        Node@ alias = held;
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += alias.Bump();
          total += held.value;
        }
        return total;
      }
      void main() { show(Work(5000000)); }
    )AS";

    // ---------------------------------------------------------------------
    // Constructs no case above reaches
    // ---------------------------------------------------------------------
    //
    // These are here for a different reason than the two groups above. A case above exists because it
    // is a shape a script has and the ratio for it is the question; a case here exists because the
    // shape produces an instruction or a hand-off no other case produces, and without it that
    // instruction's translation - or its absence - is unmeasured.
    //
    // Several are expected to lose, and that is not a reason to leave them out. A slow row for a
    // construct scripts use is a price, and a price is only visible next to the thing it is charged
    // for.

    // Comparisons whose result is *kept* rather than branched on, which is the only shape that
    // produces asBC_TZ, asBC_TNZ, asBC_TS, asBC_TNS, asBC_TP and asBC_TNP.
    //
    // `if (a < b)` does not produce them. The optimizer folds a comparison and the truth test that
    // consumes it into a single conditional jump, so the six instructions above only survive when the
    // -1/0/1 the interpreter's comparison leaves in the register has to be turned into a bool and
    // *stored*: it has to be, whenever a script keeps the result of a comparison rather than acting
    // on it immediately. That is why they are written as locals here and consumed afterwards - a
    // comparison consumed straight away is folded back into a jump and the row would measure nothing.
    //
    // The handle identity pair at the bottom is the same shape for a different comparison:
    // `asBC_CmpPtr` with `asBC_TZ`/asBC_TNZ after it, which is what `is` and `!is` compile to and what
    // a script comparing two entity handles is doing.
    constexpr const char* kBoolValues = R"AS(
      class Node {
        int value = 0;
      }

      int Work(int iterations) {
        Node@ first = Node();
        Node@ second = Node();

        int hits = 0;
        for (int i = 0; i < iterations; i++) {
          int a = i;
          int b = iterations - i;

          bool lt = a < b;
          bool le = a <= b;
          bool gt = a > b;
          bool ge = a >= b;
          bool eq = a == b;
          bool ne = a != b;

          bool same = first is second;
          bool other = first !is second;

          hits += (lt ? 1 : 0) + (le ? 2 : 0) + (eq ? 3 : 0);
          hits -= (gt ? 1 : 0) + (ge ? 2 : 0) + (ne ? 4 : 0);
          hits += (same ? 5 : 0) + (other ? 6 : 0);
        }
        return hits;
      }
      void main() { show(Work(4000000)); }
    )AS";

    // Boolean operators rather than integer comparisons: `&&` and `||` compile to short-circuit
    // jumps that leave a value behind, and `!` to asBC_NOT. None of the cases above contains either,
    // because none of them builds a condition out of other conditions.
    //
    // The operands are locals rather than expressions, which is what keeps the short-circuit visible:
    // written inline the compiler would see a run of comparisons and emit one branch per comparison,
    // and the shape a script actually has - conditions combined into a named flag and used later - is
    // the one here.
    constexpr const char* kShortCircuit = R"AS(
      int Work(int iterations) {
        int hits = 0;
        for (int i = 0; i < iterations; i++) {
          bool a = (i & 1) == 0;
          bool b = (i & 2) == 0;
          bool c = (i & 4) == 0;

          if (a && b && c) hits += 1;
          if (a || b || c) hits += 2;
          if (!a && (b || !c)) hits += 3;
          if (!(a || (b && c))) hits -= 1;
          if (!a) hits -= 2;
        }
        return hits;
      }
      void main() { show(Work(4000000)); }
    )AS";

    // One long expression per iteration and nothing else, so the block being measured is a single
    // straight run of instructions with a deeper temporary stack than anything above. The other
    // arithmetic cases break their work into statements; how a translation handles a deep expression
    // - where every sub-result is a live temporary - is a different question, and the ternary at the
    // end puts a conditional jump inside the expression rather than around it.
    constexpr const char* kExpressionDepth = R"AS(
      int Work(int iterations) {
        int acc = 1;
        for (int i = 0; i < iterations; i++) {
          acc = ((((i * 3 + 11) * 5 - 7) * 9 + 13) ^ ((i << 3) + (i >> 2)))
                + ((acc * 7 - 5) >> 1);
          acc = acc + (((i & 3) == 0) ? 1 : (acc & 1));
        }
        return acc;
      }
      void main() { show(Work(6000000)); }
    )AS";

    // A function that calls itself. Every case above that involves a call does so at a fixed depth -
    // the call graph is a tree compiled once - and this is the one where the same function is
    // re-entered while it is still running, which is the shape a recursive descent over a tree or a
    // set of nested menu nodes has.
    //
    // Depth six rather than the hundreds a real script might reach: what is being measured is the
    // cost of the hand-off at each level, and at depth six the arithmetic in the body is still
    // comparable to the call overhead rather than hidden by it. The mask keeps the accumulated value
    // from overflowing, which would make the interpreted arm raise.
    constexpr const char* kRecursion = R"AS(
      int Descend(int depth, int seed) {
        if (depth <= 0) {
          return seed;
        }
        return seed + Descend(depth - 1, (seed * 3 + depth) & 1023);
      }

      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += Descend(6, i & 31);
        }
        return total;
      }
      void main() { show(Work(400000)); }
    )AS";

    // A callback held in a function pointer and called through it. `funcdef` and the address-of a
    // function are the two halves of asBC_FuncPtr, and calling through the pointer is asBC_CallPtr -
    // neither of which any other case produces, because every other case calls a function by name.
    //
    // This is how a menu script registers a handler, and the shape is deliberately one where the
    // pointer is reassigned inside the loop: a call through a *constant* pointer is the same
    // instruction, but a reader would reasonably suspect the compiler had devirtualised it, and
    // nothing here is constant.
    constexpr const char* kFuncdefCallback = R"AS(
      funcdef int Modifier(int);

      int Doubled(int v) { return v * 2; }
      int Tripled(int v) { return v * 3; }

      int Work(int iterations) {
        Modifier@ apply = @Doubled;
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          if ((i & 1) == 0) {
            @apply = @Doubled;
          } else {
            @apply = @Tripled;
          }
          total += apply(i & 63);
        }
        return total;
      }
      void main() { show(Work(1000000)); }
    )AS";

    // An array built from a list initialiser. `array<int> values = {1, 2, 3, 4, 5, 6, 7, 8};` is not
    // the same bytecode as a loop of insertLast: the compiler emits the list operations the add-on
    // exposes for the purpose - an allocation, a size, one element address per value - and all three
    // are translated, so this row now ends its block at the array's constructor call rather than at
    // the first list instruction. The `FREE` that destroys the buffer still hands back, which is what
    // caps the row's coverage rather than anything in the list family.
    //
    // The literal is inside the loop rather than hoisted, which is the honest version of this shape:
    // the point of a literal is that a script writes one where it needs one, and hoisting it out
    // would compile the initialiser once at setup and time a loop that only indexes. What the row
    // therefore prices is building and destroying a small array per iteration, which is a real cost
    // in the interpreted arm and the reason the count is modest.
    constexpr const char* kArrayLiteral = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          array<int> values = {1, 2, 3, 4, 5, 6, 7, 8};
          total += values[i & 7];
        }
        return total;
      }
      void main() { show(Work(100000)); }
    )AS";

    // A dictionary built from a list initialiser, which is the one shape that reaches
    // `asBC_SetListType`: the add-on registers its list factory with a `?` element - `{repeat {string,
    // ?}}` - so the element's type is deduced from the value at compile time and has to be *written
    // into the buffer* rather than being known from the pattern. An `array<int>` literal never emits
    // it, which is why the row above covers `SetListSize` and not this.
    //
    // Inside the loop for the same reason the array literal is: what the row prices is building and
    // destroying a small container per iteration, which is the cost the compiler's list machinery
    // exists to make cheap and the interpreter pays in full.
    constexpr const char* kDictionaryLiteral = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          dictionary counts = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}};
          total += int(counts["one"]) + int(counts["four"]);
        }
        return total;
      }
      void main() { show(Work(50000)); }
    )AS";

    // A try/catch around the loop body. The boundary is not an instruction the emitter could have a
    // case for: the compiler allocates the asBC_TryBlock marker zero-sized and Output drops anything
    // zero-sized, so the try body reaches the translator as ordinary code and the frame around it
    // reaches the VM through scriptData->tryCatchInfo. The row therefore measures a guard that costs
    // the compiled path nothing beyond the code it wraps - which is worth a row precisely because the
    // opposite was assumed here until the marker was traced.
    //
    // Nothing in the loop throws, which is the half of the question this row is for: what a script
    // pays for the guard on the path where it never fires, which is every path a working script takes.
    // The other half - what the firing costs - is a row of its own below rather than a variation here,
    // because the two rates would otherwise be averaged into one number that answers neither.
    constexpr const char* kTryCatch = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          try {
            total += (i & 7) * 3;
          } catch {
            total = 0;
          }
        }
        return total;
      }
      void main() { show(Work(500000)); }
    )AS";

    // A catch that runs, which the case above deliberately does not have. That one measures what a
    // guard costs on the path where it never fires; this one measures what the firing costs, and the
    // two differ enough to be separate rows rather than one row with a rate.
    //
    // The ratio this row produces is below 1.0x and that is the finding rather than a failure to
    // measure. Over three runs it read 0.80x, 0.82x and 0.83x - 70.1/74.6/76.4 ms interpreted against
    // 87.4/91.4/92.2 ms compiled - so the compiled arm is the stable half and the ratio moves with the
    // interpreted one, which is the drift every table in this component carries and the reason the pair
    // is what is quoted rather than the ratio alone. The prediction was that it
    // would land near 1.0x on the argument that the unwinding belongs to the interpreter in *both*
    // arms: a compiled block does not own the try frame - the frame reaches the VM through
    // `scriptData->tryCatchInfo`, and all the compiled path has to do is stop - so the raise, the walk
    // out through the caller's frames and the catch itself are the same code either way, and the JIT's
    // share of the iteration is only the arithmetic between them. That is right about the unwinding and
    // wrong about the total, by twenty percent: what the JIT adds here is *worse* than the interpreter
    // it replaces, because the iteration leaves the compiled block at the array index, is run by the
    // dispatcher through the raise and the catch, and then has to be entered again - and the entry is
    // paid on a body with almost nothing in it, so the per-block overhead is most of the work. The
    // counters say that is the shape: the row translates 81.2% of its instructions and the `no
    // translation` line names `Thiscall1`, which is the array index the iteration ends at.
    // A menu script that guards a native call on every tick is the shape where the JIT buys the least,
    // and this is the row that says so. It is a loss case in the same sense as the line-callback rows:
    // not a defect, a boundary.
    //
    // The raise is a real one from inside a registered method - `array<int>::opIndex` with an index past
    // the end - rather than anything the script asks for, because a script cannot raise on its own:
    // `throw` is a global function this engine does not register, and the menu installs no such thing.
    // So the exception starts where a menu script's exceptions start, which is inside a native.
    //
    // One iteration in four raises, which keeps the unwinding visible in the total without making the
    // row a measurement of the interpreter's exception machinery on its own.
    constexpr const char* kTryCatchThrowing = R"AS(
      int Work(int iterations) {
        array<int> values = {10, 20, 30};
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          try {
            total += values[i & 3];
          } catch {
            total -= 1;
          }
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";
    // String comparison, which is a system call on both sides and is what a script does with a string
    // far more often than it does arithmetic on one. The case above builds strings; this one only
    // compares them, so the calls it makes are the comparison operators and nothing else.
    //
    // The comparison is against a non-constant operand - `third` is a variable, not a literal -
    // because a literal operand is still a call and the row would otherwise be readable as the
    // engine folding something. Ordering is included as well as equality, since they dispatch to
    // different operators and a script that sorts or ranges over names uses both.
    constexpr const char* kStringCompare = R"AS(
      int Work(int iterations) {
        string candidate = "adder";
        string other = "adder";
        string third = "banshee";

        int hits = 0;
        for (int i = 0; i < iterations; i++) {
          if (candidate == other) hits += 1;
          if (third != candidate) hits += 2;
          if (candidate < third) hits += 3;
        }
        return hits;
      }
      void main() { show(Work(300000)); }
    )AS";

    // A dictionary lookup. The third of the container types the menu registers, and the one the
    // suite had no case for: every operation on it is a registered method, so the loop is calls, but
    // the out-parameter form is what makes it its own row - `values.get("beta", value)` passes a
    // reference to a local for the engine to marshal through, which is neither the pointer argument
    // shape of `handles` nor the by-value native call of `native_calls`.
    //
    // The two gets are the two overloads, which is not a duplication. `get(const string&, int64&out)`
    // is a dedicated signature and the call needs no type tag; `get(const string&, ?&out)` is the
    // generic one and carries an asBC_TYPEID operand saying what to read the value back as. A
    // workload with only the int form would report a coverage figure for a call shape that is
    // genuinely narrower than the one scripts use, so the string form is here to reach it.
    //
    // The store is inside the loop so both directions are measured, and the key is a literal each
    // time so what varies between iterations is the lookup rather than the hashing of a new key.
    constexpr const char* kDictionaryAccess = R"AS(
      int Work(int iterations) {
        dictionary values;
        values.set("alpha", 1);
        values.set("beta", 2);
        values.set("label", "counter");

        int total = 0;
        for (int i = 0; i < iterations; i++) {
          if (values.exists("alpha")) {
            total += 1;
          }
          int value = 0;
          if (values.get("beta", value)) {
            total += value;
          }
          string label = "";
          if (values.get("label", label)) {
            total += label.length();
          }
          values.set("gamma", i & 255);
        }
        return total;
      }
      void main() { show(Work(200000)); }
    )AS";

    // The two loop shapes the cases above do not have. Every loop written so far is a `for`, and the
    // `while` in `tick_spawn_vehicle` has its test at the top; a `do`/`while` tests at the bottom, so
    // its entry edge and its back edge are the same edge - which is a different graph for the block
    // finder to get right, and the one place a block start can be reachable only from itself.
    //
    // `continue` and `break` are in the body because they are the two jumps that leave a loop body
    // at a point that is not its end, and a translation that got the target of either wrong would
    // still terminate here rather than spin: the work per iteration is what would be skipped.
    constexpr const char* kDoWhileBreak = R"AS(
      int Work(int iterations) {
        int total = 0;
        int i = 0;
        do {
          i++;
          if ((i & 3) == 0) {
            continue;
          }
          total += i & 7;
        } while (i < iterations);

        int j = 0;
        while (true) {
          j++;
          if (j >= iterations) {
            break;
          }
          total += j & 3;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // ---------------------------------------------------------------------
    // Numeric shapes with no case above
    // ---------------------------------------------------------------------
    //
    // This group exists because of a count rather than a hunch. The translator has a case for 191
    // distinct opcodes in the emitter plus the eleven the function compiler intercepts itself, and
    // before this group existed the rows above only ever produced a fraction of them - so a large part
    // of the translator had never been timed by anything in this file, and a translation that has never
    // been timed is one whose cost is unknown. Each case below is written to put one of those families
    // under the translator at least once. Several are expected to lose on the ratio: the point of the
    // group is the instruction it produces, not the speedup it prints. A full run prints the count it
    // reached, which is the number to compare against those when this file is next extended.
    //
    // Refused, and in the refusal column of nearly every row: `RET` and `Thiscall1`. That list has
    // shrunk twice and grown once, and each time the current version reads as if it had always been
    // that size. It began as the whole call family; `CALLSYS`, `CALL` and `CALLINTF` were translated
    // first, with `FREE` and the counted half of the reference copies; `ALLOC`'s script-object branch
    // and `CallPtr` followed, which is why the construction and funcdef rows above no longer report
    // them - and the effect of all of it is visible in the coverage column of every row that calls or
    // constructs anything.
    //
    // The two that are left are both refused on this file's measurements rather than for want of a
    // case, and they are the reason the refusal column lists them on rows whose ratio is otherwise
    // good.
    //
    // `RET` grew back: it was translated, verified against the interpreter, and then refused again
    // because this file measured it - the predicate that decides whether a frame may be popped is only
    // reachable through a virtual call that serialises two pointers, and paying that on every return
    // cost the call-heavy rows of this very suite twenty to forty-five percent. The table is in the
    // `asBC_RET` case of `function_compiler.cpp`.
    //
    // `Thiscall1` is the newest entry and the closest thing here to a surprise, because the original
    // reason for refusing it read as settled and was never anything but a prediction. It was translated,
    // and it turned out that the prediction was right: the `array_index` row - the only row that
    // reaches the instruction - compiles to 81.4 ms with the translation against 36.1 ms without it,
    // which is that row's 1.79x speedup turning into 0.80x. The paragraph and the table are in the
    // default case of `emitter.cpp`. The four `Thiscall1` sites the suite reaches are all in that one
    // row, so this is a row to read as a cost rather than as a regression.
    //
    // What is left in the emitter's default case after those two is `ALLOC`'s value-type branch, which
    // is not the same instruction's other half so much as the same opcode's other opcode: its tail
    // branches on a private suspend flag. It has never appeared in the refusal column, and the reason
    // is that no script this suite compiles reaches it - the compiler only produces the value-type
    // branch where the object is on the heap, which for a value type means a module global, a reference
    // member or a generated factory stub, and none of those is a function the JIT is offered. So it is
    // a refusal written in advance of the construct that would reach it: correct to refuse, since what
    // stops it is a private field, and not a row that can be written. The reasoning for it, and for why
    // the rest moved, is in the default case of `emitter.cpp`; the engine's own documentation is where
    // the discussion starts rather than a conclusion drawn from it - `doc_adv_jit.h`, "The structure of
    // the JIT function": "Some byte code instructions are not meant to be converted into native code.
    // These are usually the ones that have a more global effect on the VM, e.g. the instructions that
    // setup a call to a new script function, or that return from a previous instruction." The calls are
    // translated anyway, because what that is protecting is the frame and a translated call still has
    // the engine build it; the return can be translated on the same footing, and is not, because the
    // translation measured slower than the dispatch it replaces.
    //
    // What the group leaves behind after that, and why - recorded rather than left to be rediscovered
    // by whoever next counts the columns.
    //
    // The list below was re-grounded against a full run rather than extended from the last one, because
    // the two are not the same thing: the suite has grown since it was written and one of its entries
    // had stopped being true. Taking the VM's own dispatch table (as_context.cpp:2219, 201 entries) and
    // subtracting every opcode the suite walks leaves exactly four - CALLBND ChkNullS PshC8 STR - and
    // that subtraction is the check, not this paragraph. Two of the four are answered below by why no
    // configuration can reach them, and two by the configuration that does.
    //
    //   DIVi DIVu DIVi64 DIVu64
    //     Not reachable in the configuration this suite's `plain` rows measure.
    //     asEP_DISABLE_INTEGER_DIVISION rewrites `/` and `/=` on every integer type to double before
    //     the bytecode is built (as_compiler.cpp:15766), so the menu's engine cannot produce them.
    //     They are translated, and they are reached two other ways: the differential tests run with
    //     the property off, and the `int_divide` row does too, under the `intdiv` variant - which is
    //     where these are measured rather than only asserted. The unsigned two need a script with an
    //     unsigned dividend as well, which is why `kIntDivide` carries one.
    //     What remains true is the narrower claim: no `plain` row can produce one.
    //   ClrHi ClrVPtr PshNull SwapPtr
    //     Reachable, and translated, but only with `asEP_OPTIMIZE_BYTECODE` off: the peephole passes
    //     fold or rewrite away every shape that produces them, so an engine configured like the menu's
    //     never hands one to the translator. Reachable *here*, which is the correction to this entry:
    //     `SwapPtr` used to be listed below, as unreachable in any configuration, and the `unoptimized`
    //     row runs under the `noopt` variant and produces all four. The reasoning for the old entry was
    //     sound about what it examined - a handle to a value type really is refused by
    //     `asCDataType::MakeHandle` - and wrong about the requirement, since what decides the branch is
    //     whether the exchange can be folded at compile time (`asCByteCode::CanBeSwapped`,
    //     as_bytecode.cpp:305, which folds it only when both addresses are statically known). The
    //     differential suite reaches the same instruction the same way, so the row is not the only
    //     check on it - but the row is what a coverage number is counted from, and the old entry was
    //     making one of those wrong. An "unreachable" finding is a finding about a *configuration*.
    //   ChkNullS PshC8
    //     Not reachable at all, in any configuration, and the two have nothing in common but that:
    //       PshC8     never emitted - the compiler has no site that produces it.
    //       ChkNullS  still emitted from as_compiler.cpp:2412, but the argument's type has already
    //                 been converted away from a handle by the time that line runs, so the guard
    //                 never fires. The guard the shape actually gets is `asBC_ChkRefS`, which is
    //                 translated and has two tests of its own.
    //     Both are translated anyway, because the instruction exists and its meaning is unambiguous.
    //     That is the same footing `SwapPtr` was defended on above, and it is the reason the two are
    //     not simply deleted: the count of implemented opcodes is a claim about the translator, not
    //     about which scripts happen to exist.
    //   CALLBND STR
    //     The two the dispatch table has and nothing can reach.
    //       STR       the handler is `asASSERT(false)` under a "TODO: NEWSTRING: Deprecate this
    //                 instruction", and the current compiler emits it from nowhere - the only
    //                 remaining reference is `as_restore.cpp`, reading modules saved by a version
    //                 that did. A translator case would mean inventing a behaviour the engine does
    //                 not have.
    //       CALLBND   produced, and dispatchable, and still out of reach: it is emitted only for a
    //                 call to a function the module declared `import` (as_compiler.cpp:17425), and
    //                 the menu builds one module per script and never calls `BindImportedFunction`.
    //                 Its translation is blocked a second time - resolving an import index to the
    //                 bound function has no published route, since `GetFunctionById` takes a
    //                 function id rather than an import index. The full argument is in
    //                 `emitter.cpp`'s refusal for it.
    //
    // Naming them is the point: four uncovered opcodes with an answer each is a coverage claim that can
    // be checked, where four uncovered opcodes without one is a number nobody can act on. The check is
    // mechanical and takes one command - every name in the dispatch table, minus every name the run
    // printed, leaves that list of four and nothing else.

    // Integer modulo, in all four widths. `%` is the one integer division the menu does not promote to
    // floating point - asEP_DISABLE_INTEGER_DIVISION covers `/` and `/=` and leaves `%` alone
    // (as_compiler.cpp:15766) - so the `division` row does not subsume this one: that measures DIVd,
    // this measures the four MOD instructions, each of which the interpreter guards against a zero
    // divisor on its own path.
    //
    // The divisors are variables so the guard is a real branch rather than a constant the compiler can
    // fold, and `+ 1` keeps them away from zero, which is what lets this run to completion.
    constexpr const char* kModulo = R"AS(
      int Work(int iterations) {
        int total = 0;
        int a = 1000003;
        uint b = 4294967291;
        int64 c = 1234567891011;
        uint64 d = 1099511628211;
        for (int i = 1; i < iterations; i++) {
          int v = i & 1023;
          total += a % (v + 1);
          total += int(b % (uint(v) + 1));
          total += int(c % (int64(v) + 1));
          total += int(d % (uint64(v) + 1));
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";

    // Unary minus, which is its own instruction per type rather than a subtraction from zero: the
    // floating-point forms flip a sign bit and share nothing with the integer ones, and the 64-bit
    // form is a separate case again. Operands are variables so none of it folds away.
    constexpr const char* kNegation = R"AS(
      int Work(int iterations) {
        int total = 0;
        int i32 = 7;
        float f32 = 1.5f;
        double f64 = 2.5;
        int64 i64 = 123456789;
        for (int i = 0; i < iterations; i++) {
          total += -i32 + int(-f32) + int(-f64) + int(-i64);
          i32 = i32 + 1;
          f32 = f32 + 0.5f;
          f64 = f64 + 0.25;
          i64 = i64 + 1;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // Single-precision division, and a comparison between a float and an integer. The `division` row is
    // double division and the `float_compare` row compares two floats; neither reaches DIVf - the one
    // arithmetic instruction where a single-precision reciprocal differs from the double one - nor
    // CMPIf, which exists because the two operands of a comparison need not be the same width.
    constexpr const char* kFloatDivision = R"AS(
      float Work(int iterations) {
        float total = 1.0f;
        int hits = 0;
        for (int i = 1; i < iterations; i++) {
          float f = float(i) * 0.5f;
          total = total + 1000.0f / f;
          if (f < i) {
            hits += 1;
          }
          if (i & 1023 == 0) {
            total = 1.0f;
          }
        }
        return total + float(hits);
      }
      void main() { show(Work(4000000)); }
    )AS";

    // The 64-bit bitwise and shift instructions the `arith_int64` row does not produce. That row uses
    // `^`, `<<` and `>>` on a value that stays positive; these are `&`, `|`, `~` and the *arithmetic*
    // right shift, which differs from the logical one on exactly the values a signed type can hold -
    // which is why the translator has a case of its own for it rather than reusing the unsigned one.
    constexpr const char* kInt64Bitwise = R"AS(
      int64 Work(int iterations) {
        int64 acc = 0x0123456789ABCDEF;
        int64 mask = 0x00FF00FF00FF00FF;
        for (int i = 0; i < iterations; i++) {
          int64 v = int64(i);
          acc = (acc & mask) | (v & ~mask);
          acc = acc ^ (~v);
          acc = acc >> 5;
          acc = acc + (v >> 3);
          acc--;
          if (acc == v) {
            acc = acc + 1;
          }
          if (acc < 0) {
            acc = -acc;
          }
        }
        return acc;
      }
      void main() { show(Work(4000000)); }
    )AS";

    // Every conversion between the 64-bit integers and the floating-point types, in both directions,
    // plus the 32-bit unsigned ones. The `convert` row covers the signed 32-bit forms only; the 64-bit
    // ones are separate instructions with their own rounding, and the unsigned ones a separate set
    // again, because the value that has no representation differs between them.
    constexpr const char* kConvert64 = R"AS(
      int Work(int iterations) {
        int total = 0;
        int64 i64 = 1234567890123;
        uint64 u64 = 98765432109876;
        for (int i = 0; i < iterations; i++) {
          double fromSigned = double(i64);
          float fromSignedF = float(i64);
          double fromUnsigned = double(u64);
          float fromUnsignedF = float(u64);
          total += int(fromSigned) + int(fromSignedF);
          total += int(fromUnsigned) + int(fromUnsignedF);

          uint u32 = uint(total);
          double fromU32 = double(u32);
          float fromU32F = float(u32);
          total += int(uint(fromU32)) + int(uint(fromU32F));
          total += int(double(fromU32F)) + int(float(fromU32));

          i64 = int64(fromSigned) + int64(fromSignedF) + 1;
          u64 = uint64(fromUnsigned) + uint64(fromUnsignedF) + 1;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // The narrow integer types, which are the only ones whose reads and writes are byte- or
    // word-sized. An `int8` local is one byte wide, so setting it, reading it back and incrementing it
    // in place are each a different instruction from their 32-bit counterpart, and `convert` only
    // ever converts *into* one - it never increments one or reads it back out.
    //
    // The module-level narrow globals are here for the same reason the locals are: a global is not a
    // frame slot, so it is reached by a different instruction again.
    constexpr const char* kNarrowInts = R"AS(
      int8 g_tiny = 3;
      uint8 g_byte = 7;
      int16 g_short = 11;
      uint16 g_word = 13;

      int Work(int iterations) {
        int8 tiny = 1;
        uint8 byte = 2;
        int16 shortish = 3;
        uint16 word = 4;
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          tiny++;
          byte++;
          shortish++;
          word++;
          tiny--;
          byte--;
          shortish--;
          word--;
          total += int(tiny) + int(byte) + int(shortish) + int(word);
          g_tiny = tiny;
          g_byte = byte;
          g_short = shortish;
          g_word = word;
          total += int(g_tiny) + int(g_byte) + int(g_short) + int(g_word);
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // A handle taken as a parameter and dereferenced, which is a different shape from the handles the
    // `handle_guards` row holds in locals: the compiler guards a parameter it cannot prove anything
    // about, so this is where the null guard instructions are produced. The `is null` test is here for
    // the null *constant* it pushes - the guard is what runs, and it never fires.
    //
    // The loop is inside the callee so that the compiled function is the one containing the guard,
    // rather than a caller whose only instruction is the call.
    constexpr const char* kHandleParameter = R"AS(
      class Node {
        int value = 3;
        int Read() const { return value; }
      }

      int Deref(Node@ node, int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          if (node is null) {
            break;
          }
          total += node.value + node.Read();
        }
        return total;
      }

      void main() {
        Node@ node = Node();
        show(Deref(node, 2000000));
      }
    )AS";

    // Increment and decrement as *expressions* rather than as statements, and on the floating-point
    // types as well as the integers. `i++` standing alone is a variable instruction the `for` loops
    // above already produce; `total += i++` has to put the old value on the stack first, which is a
    // different instruction each time, and the float and double forms are different again.
    constexpr const char* kIncrements = R"AS(
      int Work(int iterations) {
        int total = 0;
        int i = 0;
        int64 wide = 1000;
        float f = 1.0f;
        double d = 1.0;
        for (int n = 0; n < iterations; n++) {
          total += i++;
          total += --i;
          wide++;
          wide--;
          f++;
          d--;
          f--;
          d++;
          total += int(f) + int(d) + int(wide);
        }
        for (int n = iterations; n > 0; n--) {
          total += n & 1;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // The operators that reach instructions no other case does, which took reading the compiler to
    // find rather than guessing: the arithmetic right shift is a separate *operator* (`>>>`) rather
    // than a property of `>>` (as_compiler.cpp:16442), single- and double-precision remainder are
    // separate instructions from the integer ones, and a float variable combined with an *integer*
    // literal is fused by the bytecode optimizer into a form that carries the immediate
    // (as_bytecode.cpp:443) - so `f > 0` and `f + 1` are what produce CMPIf and SUBIf, and `f > 0.0f`
    // would not.
    constexpr const char* kMixedNumeric = R"AS(
      double Work(int iterations) {
        int acc = -1000000000;
        int64 wide = -1234567890123;
        float f = 4.0f;
        double d = 4.0;
        int shift = 3;
        double total = 0.0;
        for (int n = 0; n < iterations; n++) {
          acc = acc >>> shift;
          wide = wide >>> shift;
          f = f % 2.5f;
          d = d % 2.5;
          if (f > 0) {
            f = f - 1;
          }
          total = total + 0.125;
          total += d + double(acc) + double(wide);
          f = f + 1.0f;
          d = d + 1.0;
        }
        return total;
      }
      void main() { show(Work(3000000)); }
    )AS";

    // Module-level variables of every width, each assigned a constant and read back. A global is not
    // a frame slot, so storing to one is a different instruction from storing to a local, and which
    // one depends on the width being stored: a four-byte constant goes to a global through a single
    // instruction, an eight-byte one through another, and a value that arrives in a variable goes
    // through the copy forms the `globals` row already produces. The `narrow_ints` row writes only
    // the byte- and word-wide globals.
    constexpr const char* kWideGlobals = R"AS(
      int g_int = 1;
      int64 g_wide = 2;
      double g_double = 3.0;
      uint64 g_uwide = 4;

      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          g_int = 1234567;
          g_wide = 1234567890123;
          g_double = 0.5;
          g_uwide = 98765432109876;
          total += g_int;
          total += int(g_wide);
          total += int(g_double);
          total += int(g_uwide);
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";

    // Parameters taken by reference rather than by value, which is a different instruction from a
    // value parameter on both sides of the call: reading through the reference and writing through it
    // are each their own case, and the width decides which one. The `dictionary_access` row passes a
    // reference to a registered method and gets the stack forms instead, so this is the only place the
    // reference-width instructions are produced.
    constexpr const char* kByReference = R"AS(
      int64 Accumulate(const int64 &in seed, int iterations) {
        int64 acc = seed;
        for (int i = 0; i < iterations; i++) {
          acc = acc + seed;
        }
        return acc;
      }

      void Bump(int64 &out total, int iterations) {
        for (int i = 0; i < iterations; i++) {
          total = total + 1;
        }
      }

      void main() {
        int64 total = 0;
        Bump(total, 2000000);
        show(total + Accumulate(1234567890123, 2000000));
      }
    )AS";

    // A handle assigned *without* the `@` operator, which is not the same operation at all. With `@`
    // the compiler moves the pointer; without it, the compiler checks the source for null and then
    // copies the object it points at into the object the destination points at - and the check is
    // where the family of null-guard instructions comes from. `handle_guards` and `handle_parameter`
    // both stop short of this because every assignment they write uses the operator.
    //
    // The global handle and the by-reference parameter are here for the same reason: a global handle
    // is not a frame slot, so reading it is its own instruction, and a parameter taken by reference
    // is a different conversion again from an explicit-handle parameter - `Node &in` is a reference
    // and `Node@` a handle, and a script that passes one where the other is wanted is what makes the
    // compiler insert the check. The null argument needs the handle form for exactly that reason: a
    // reference cannot be null, so `Deref(null)` does not compile. It is never reached either way,
    // sitting under a condition that cannot hold, so the call is compiled and never made - which is
    // what a translation has to survive and a call does not have to.
    constexpr const char* kHandleConversion = R"AS(
      class Node {
        int value = 3;
        int Read() const { return value; }
      }

      int Deref(Node &in node) {
        return node.value;
      }

      int DerefHandle(Node@ node) {
        return node.value;
      }

      Node@ Make() {
        return Node();
      }

      Node@ g_node;

      int Work(int iterations) {
        Node@ source = Node();
        Node@ destination = Node();
        @g_node = source;

        int total = 0;
        for (int i = 0; i < iterations; i++) {
          destination = source;
          total += destination.value;
          total += Deref(source);
          total += Deref(Make());
          total += g_node.Read();
          source.value = source.value + 1;
          if (i < 0) {
            total += DerefHandle(null);
          }
        }
        return total;
      }
      void main() { show(Work(200000)); }
    )AS";

    // A read-modify-write of a module-level variable, which the bytecode optimizer turns into its own
    // instruction: `asBC_LdGRdR4` loads the global's current value straight into a register, where the
    // unoptimized form computes the address and dereferences it. The `4` is the width of the value
    // read, so the row carries a float as well as an int - a translation that read a different number
    // of bytes would be right for one of these and wrong for the other, and both would still print a
    // number.
    //
    // Unlike most of the numeric group this one does not need an unusual engine configuration: the
    // instruction is the optimizer's product and the benchmark runs with the optimizer on, as the menu
    // does.
    constexpr const char* kGlobalCompound = R"AS(
      int g_count = 1;
      float g_scale = 1.0f;

      int Work(int iterations) {
        for (int i = 0; i < iterations; i++) {
          g_count += 3;
          g_count *= 2;
          g_count -= 1;
          g_scale *= 1.5f;
          g_scale += 0.25f;

          // Both accumulators are reset rather than left to run away, because the row is about how a
          // compound assignment reads and writes a global and not about what happens at the ends of
          // the range. Left alone the int wraps and the float reaches infinity within the first few
          // thousand iterations, and the rest of the run would be measuring the same arithmetic on
          // different magnitudes.
          if (g_count > 1000000) {
            g_count = 1;
            g_scale = 1.0f;
          }
        }
        return g_count + int(g_scale);
      }
      void main() { show(Work(2000000)); }
    )AS";

    // An object handed back by *reference*, which is the third way one crosses a function boundary and
    // the only one that reaches `asBC_PshRPtr`. The other two - by value and by handle - have rows
    // above; this one exists because the instruction lives on a branch of the compiler's call code
    // that neither of them takes (as_compiler.cpp:17543).
    //
    // Expected to land near parity, and that is the honest result rather than a fault: `GetGlobalVec`
    // is a native call, so the loop breaks at it and the block after it - the one holding PshRPtr -
    // pays an entry and an exit for one instruction. What the row reports is therefore the cost of
    // reaching that instruction, which is the number a menu script would actually see.
    constexpr const char* kReferenceReturn = R"AS(
      int Work(int iterations) {
        float total = 0.0f;
        for (int i = 0; i < iterations; i++) {
          total += GetGlobalVec().GetX();
        }
        return int(total);
      }
      void main() { show(Work(2000000)); }
    )AS";

    // The reference cast - `asBC_Cast` - which is a type test followed by a reference taken on the
    // object when the test passes. Every branch of it happens a fixed number of times per iteration: an
    // interface the object's dynamic type implements, a sibling type it does not, and the handle traffic
    // the result implies in both cases. The row exists because the instruction was translated without a
    // row that reached it - it had differential tests and no timing, and a translation that has never
    // been timed is one whose cost is unknown.
    constexpr const char* kCastHeavy = R"AS(
      interface IShape { int Sides(); }
      class Shape { int kind; }
      class Square : Shape, IShape { int Sides() { return 4; } }
      class Circle : Shape, IShape { int Sides() { return 1; } }

      int Work(int iterations) {
        Square sq;
        Circle ci;
        Shape@ a = sq;
        Shape@ b = ci;

        int total = 0;
        for (int i = 0; i < iterations; i++) {
          IShape@ asInterface = cast<IShape>(a);
          if (asInterface !is null) total += 1;

          Square@ asSquare = cast<Square>(b);
          if (asSquare !is null) total += 2;
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";

    // ---------------------------------------------------------------------
    // Realistic - shaped like what the menu runs per game frame
    // ---------------------------------------------------------------------

    // A reconstruction of `example scripts/spawn vehicle/main.as`: the only shipped script, and the
    // only statement of intent about what scripts look like. Its entire GameTick is native calls,
    // logs and a wait loop, with one comparison in it.
    constexpr const char* kSpawnVehicleTick = R"AS(
      void GameTick() {
        int ped = StubPlayerPedId();
        StubLog("Ped ID", ped);

        int model = StubHash("adder");
        StubRequestModel(model);
        while (StubHasModelLoaded(model) != 1) {
          StubThreadYield();
        }

        float x = StubGetX(ped);
        float y = StubGetY(ped);
        float z = StubGetZ(ped);
        StubLog("Coords", ped);

        float heading = StubGetHeading(ped);
        int vehicle = StubCreateVehicle(model, x, y, z, heading);
        if (vehicle > 0) {
          StubLog("Created", vehicle);
        } else {
          StubLog("Failed", vehicle);
        }
        StubSetModelAsNoLongerNeeded(model);
      }

      void main() {
        for (int tick = 0; tick < 300000; tick++) {
          GameTick();
        }
      }
    )AS";

    // The same tick, with a small arithmetic loop added between the calls. This is the case the JIT
    // is supposed to win, and the one that says whether compiling the code *between* natives is worth
    // anything at all.
    constexpr const char* kTickWithArithmetic = R"AS(
      int Mix(int seed, int rounds) {
        int acc = seed;
        for (int i = 0; i < rounds; i++) {
          acc = acc * 3 - (acc >> 2) + i;
        }
        return acc;
      }

      void GameTick() {
        int ped = StubPlayerPedId();
        int mixed = Mix(ped, 200);
        StubLog("Ped ID", mixed);
        StubRequestModel(mixed);
        StubThreadYield();
      }

      void main() {
        for (int tick = 0; tick < 200000; tick++) {
          GameTick();
        }
      }
    )AS";

    // The same tick with a dispatch switch in it, which is what a script that handles several event
    // kinds looks like. The switch is now compiled to a compare chain rather than left to the
    // interpreter, so this is where that shows up in a realistic shape rather than a synthetic one.
    constexpr const char* kTickWithSwitch = R"AS(
      int Dispatch(int kind) {
        switch (kind) {
          case 0: return StubPlayerPedId();
          case 1: return StubHash("adder");
          case 2: return int(StubGetHeading(1));
          default: return 0;
        }
      }

      void GameTick(int frame) {
        StubLog("kind", Dispatch(frame % 4));
      }

      void main() {
        for (int tick = 0; tick < 300000; tick++) {
          GameTick(tick);
        }
      }
    )AS";

    // ...and with the per-script state a real script keeps between ticks. The state is an object, so
    // every field access in the tick is one of the member-address translations, and the natives are
    // still there to end the block.
    constexpr const char* kTickWithState = R"AS(
      class VehicleState {
        int model = 0;
        float x = 0.0f;
        float y = 0.0f;
        int updates = 0;

        int Update(int frame) {
          model = StubHash("adder");
          x = x + 0.5f;
          y = y + 0.25f;
          updates = updates + 1;
          return model + int(x + y) + frame;
        }
      }

      void main() {
        VehicleState state;
        for (int tick = 0; tick < 300000; tick++) {
          StubLog("state", state.Update(tick));
        }
        show(state.updates);
      }
    )AS";

    // Many small scripts rather than one long run: each tick is entered and left once, so what this
    // measures is the per-entry overhead rather than the throughput of a loop. A script that is
    // ticked once per frame is the shape the menu actually has, and the one where a prologue that is
    // a few instructions too long is paid on every frame instead of once.
    constexpr const char* kManyShortTicks = R"AS(
      int ScriptA(int frame) { return StubPlayerPedId() + frame; }
      int ScriptB(int frame) { return StubHash("adder") - frame; }
      int ScriptC(int frame) { return ScriptA(frame) + ScriptB(frame); }

      void main() {
        int total = 0;
        for (int tick = 0; tick < 400000; tick++) {
          total += ScriptC(tick);
        }
        show(total);
      }
    )AS";

    // A script object constructed and destroyed inside the loop. This is `asBC_ALLOC`'s
    // script-object branch under load, and it is the row the translation was added for: the
    // interpreter allocates, pre-initialises, rewrites the caller's argument slot and enters the
    // constructor, and every one of those steps is a dispatch it does not get back.
    //
    // The object is a *local* rather than a handle, which is the shape that puts the whole lifetime in
    // one iteration: the construction is an ALLOC at the top of the body and the scope exit is a FREE
    // at the bottom, so the loop turns both over once per iteration. A handle would move the release to
    // wherever the last reference went, which for a loop that overwrites one variable every iteration is
    // the same place - but it would also leave the object alive across the boundary under a
    // conservative reading, and this row is meant to have nothing alive across the boundary.
    //
    // The constructor takes an argument and writes a member through it, because a parameterless
    // constructor that assigned a constant would fold most of the body away and would leave the
    // argument-slot rewrite - the one step of the translation that is not a straight copy of the
    // interpreter's - unexercised.
    constexpr const char* kScriptObjectLoop = R"AS(
      class Particle {
        int id;
        float life;

        Particle(int seed) {
          id = seed;
          life = float(seed & 15) * 0.25f;
        }

        float Step(float dt) {
          life -= dt;
          return life;
        }
      }

      float Work(int iterations) {
        float total = 0.0f;
        for (int i = 0; i < iterations; i++) {
          Particle p(i);
          total += p.Step(0.5f) + float(p.id & 3);
        }
        return total;
      }
      void main() { show(Work(600000)); }
    )AS";

    // A method bound to an object and called through the funcdef it was bound to. `asBC_CallPtr`
    // dispatches on the callee's kind, and this is the *delegate* kind - the one the translation
    // reaches through `GetDelegateObject`/`GetDelegateFunction` rather than through the id table.
    // `funcdef_callback` above is the same instruction with a plain function pointer in it, so the two
    // rows are the two halves of the instruction's dispatch and only one of them was covered before.
    //
    // What makes this a loss case rather than a win, and is the reason to have it: a delegate call is
    // the only call the engine enters through the *interface* route, and it pushes its own receiver
    // before doing so. The translation removes the two dispatches and keeps everything else.
    //
    // The binding is made once, outside the loop, because constructing a delegate is itself an
    // allocation and a system call - inside the loop the row would measure that instead, and the row
    // below it in spirit is `funcdef_callback`, which rebinds per iteration and is the other question.
    constexpr const char* kDelegateCall = R"AS(
      funcdef int Modifier(int);

      class Scale {
        int factor;

        Scale(int f) { factor = f; }

        int Apply(int v) { return v * factor; }
      }

      int Work(int iterations) {
        Scale scale(3);
        Modifier@ apply = Modifier(scale.Apply);

        int total = 0;
        for (int i = 0; i < iterations; i++) {
          total += apply(i & 63);
        }
        return total;
      }
      void main() { show(Work(400000)); }
    )AS";

    // Integer division and remainder, in both widths. The only row in the suite whose *plain* form is
    // not the program it names: the menu forbids `int / int` at compile time, so under `plain` the
    // compiler rewrites every `/` and `%` below to double arithmetic with a conversion on each side,
    // and under `intdiv` they are the four integer handlers. Printing the two rows side by side is the
    // point - one is what the menu ships, the other is what the translator can do when it is allowed
    // to, and the difference between them is the cost of the menu's own restriction.
    //
    // The divisor is `(i & 31) + 1`, which is never zero: a script that divides by an expression the
    // compiler cannot prove non-zero still compiles, but the interpreted arm would raise on the first
    // iteration and the row would report a failure rather than a ratio.
    //
    // The two halves are signed and unsigned, and both are needed. The first version of this script had
    // only `int` and `int64`, which produce the four *signed* handlers - and the variant's own comment
    // claimed it made DIVu and DIVu64 reachable, which no row in the suite had ever made true. An
    // unsigned divisor is the only way to reach them: `as_compiler.cpp:15891` picks DIVu over DIVi on
    // `IsIntegerType()`, so the operand type is the whole difference.
    //
    // With the unsigned half added the body produces all eight integer division and remainder handlers
    // the engine has - DIVi DIVu DIVi64 DIVu64 and MODi MODu MODi64 MODu64 - which is every one of them
    // in both widths and both signednesses. The `modulo` row already reaches the two unsigned remainders
    // under `plain`, because `%` is exempt from the flag; the two unsigned divisions are reachable here
    // and nowhere else.
    //
    // `uwide` and `uwide64` are kept in ranges that wrap rather than trap - unsigned overflow is
    // defined, and both arms of a row run the same bytecode, so what the wrap does to the printed sum is
    // the same on both sides of the comparison.
    constexpr const char* kIntDivide = R"AS(
      int Work(int iterations) {
        int total = 0;
        int64 wide = 123456789;

        uint uwide = 4000000007;
        uint64 uwide64 = 1234567891011;

        for (int i = 1; i < iterations; i++) {
          int divisor = (i & 31) + 1;
          total += (i * 7) / divisor;
          total -= (i * 5) % divisor;
          wide = (wide * 3 + i) / divisor;
          wide = wide + ((wide % divisor) & 1023);

          uint udivisor = uint(divisor);
          total += int((uint(i * 9) + uint(3)) / udivisor);
          uwide = uwide / udivisor + uint(1);
          total += int(uwide % uint(1024));

          uint64 udivisor64 = uint64(divisor);
          uwide64 = (uwide64 + uint64(i)) / udivisor64;
          total += int((uwide64 % uint64(2048)) & uint64(1023));
        }

        return total + int(wide & 1023);
      }
      void main() { show(Work(3000000)); }
    )AS";

    // The optimizer's own axis, exercised on the shapes it folds: a comparison stored in a `bool` and
    // then tested, and a handle explicitly set to `null` and tested against it.
    //
    // The three opcodes this exists for are `ClrHi`, `PshNull` and `SwapPtr`, all of which the peephole
    // passes remove - `ClrHi` immediately before a test becomes part of the test, `PshNull` in front of
    // a handle store is folded into it. They are translated, and until this row existed nothing in the
    // suite had ever produced one, which is the difference between "translated" and "known to work".
    // `bool_values` covers the same ground with the optimizer on and produces `JLowZ` instead; the pair
    // of rows is the fold, and the reason this case exists rather than a new one written from scratch.
    //
    // `SwapPtr` comes from the third shape and is the one of the three that is not obviously a fold on
    // paper. A conversion operator producing a value type too large for the registers hands the callee
    // a caller-supplied buffer, and because the operator is also reached with the object's own address
    // on the stack, the buffer's address arrives above it and the two are exchanged
    // (as_compiler.cpp:8080). The harness registers that operator on `Handle` for exactly this row, and
    // nothing in the suite had produced the instruction before it.
    //
    // The two calls below are on a slow path - one iteration in 256 - rather than in the body proper.
    // They are here for the instruction, not for the timing, and a value-type copy per iteration would
    // dominate a row whose ratio is already the least interesting thing about it; the branch keeps them
    // in the compiled code and out of the measurement.
    //
    // The body is deliberately small for the same reason: the row is about which instructions appear,
    // not how fast they are, and a no-optimizer script is a slower program for reasons that have
    // nothing to do with the JIT, so its ratio is one to read against its `plain` twin and not against
    // the table.
    constexpr const char* kUnoptimized = R"AS(
      class Node {
        int value;
        Node@ next;
      }

      int Work(int iterations) {
        int total = 0;
        Node@ head = null;
        Node@ tail = null;
        Block block;

        for (int i = 0; i < iterations; i++) {
          bool positive = (i & 1) == 0;
          if (head is null) {
            @head = tail;
          }

          if (positive) {
            total += 1;
          } else {
            total -= 1;
          }

          if (tail is null && positive) {
            total += 2;
          }

          if ((i & 255) == 0) {
            block.head = float(i);
            Block copy = block.Copy();
            total += int(copy.head + copy.tail);

            Handle@ handle = Handle(1);
            Vec3 converted = handle;
            total += int(converted.x + converted.y);
          }
        }

        return total + (head is null || tail is null ? 1 : 0);
      }
      void main() { show(Work(400000)); }
    )AS";

    // A tick that logs, which is what the menu's own scripts do on every frame and what no row in this
    // suite could produce until `StubLogFormat` existed: every call here is the generic convention, and
    // the two sites carry different numbers of arguments, so the engine's variadic branch - the one that
    // derives its pop size from the count the call actually carried - runs at two widths rather than one.
    //
    // The second site is behind a branch so that the row is not simply twice the first, and the body
    // otherwise does almost nothing, because the question this row asks is what a per-frame log costs
    // and not what a slow script costs.
    //
    // The row was written expecting to lose and it wins: 100.7 ms interpreted against 62.5 ms compiled,
    // 1.61x, at 92.5% coverage. The reason is worth recording because it corrects a premise this whole
    // suite was built on. The plan that produced it said a realistic script "compiles into many tiny
    // blocks separated by interpreter round-trips at every native call", reading `emitter.cpp`'s default
    // case as ending the block at any call. That was true when it was written and is not true now:
    // `asBC_CALLSYS` is translated, so a native call does not end a block - it is made from inside one
    // and the block carries on, which is why this row translates 92.5% of its instructions instead of
    // stopping at every one of them. What still ends a block is `asBC_CALL` and `asBC_CALLINTF`, the
    // calls into *script* functions, and the row that prices those is `script_calls`.
    //
    // So the marshaller's extra work is not something the translation pays for. This is the shape the
    // menu logs in, on every frame, through the convention the menu's loggers are registered with, and
    // it is inside the compiled path rather than at its edge.
    constexpr const char* kGenericCalls = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++) {
          StubLogFormat("frame", i);
          if ((i & 3) == 0)
            StubLogFormat("frame", i, i + 1, i + 2);
          total += i & 15;
        }
        return total;
      }
      void main() { show(Work(2000000)); }
    )AS";
  } // namespace

  void RegisterStubs(AngelScript::asIScriptEngine* engine) {
    engine->RegisterGlobalFunction("int StubPlayerPedId()", AngelScript::asFUNCTION(StubPlayerPedId), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("int StubHash(const string &in)", AngelScript::asFUNCTION(StubHash), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("void StubRequestModel(int)", AngelScript::asFUNCTION(StubRequestModel), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("int StubHasModelLoaded(int)", AngelScript::asFUNCTION(StubHasModelLoaded), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("void StubThreadYield()", AngelScript::asFUNCTION(StubThreadYield), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("void StubLog(const string &in, int)", AngelScript::asFUNCTION(StubLog), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("float StubGetX(int)", AngelScript::asFUNCTION(StubGetX), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("float StubGetY(int)", AngelScript::asFUNCTION(StubGetY), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("float StubGetZ(int)", AngelScript::asFUNCTION(StubGetZ), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("float StubGetHeading(int)", AngelScript::asFUNCTION(StubGetHeading), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("int StubCreateVehicle(int, float, float, float, float)", AngelScript::asFUNCTION(StubCreateVehicle), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("void StubSetModelAsNoLongerNeeded(int)", AngelScript::asFUNCTION(StubSetModelAsNoLongerNeeded), AngelScript::asCALL_CDECL);
    engine->RegisterGlobalFunction("void StubTouch(int)", AngelScript::asFUNCTION(StubTouch), AngelScript::asCALL_CDECL);

    engine->RegisterGlobalFunction("void StubLogFormat(const string &in, int ...)",
                                   AngelScript::asFUNCTION(StubLogFormat), AngelScript::asCALL_GENERIC);
  }

  const std::vector<Workload>& Workloads() {
    // The line-callback variant is run for one representative of each group rather than for all of
    // them. It answers a single question - what does the per-statement hand-off cost - and asking it
    // fifteen times would triple the run time to print the same answer.
    static const std::vector<Workload> workloads = {
        {"arith_int", "tight integer loop, nothing falls back - the ceiling", kArithInt, true},
        {"arith_float", "float add/sub/mul in a loop", kArithFloat, false},
        {"arith_double", "double add/sub/mul in a loop", kArithDouble, false},
        {"int64_arith", "64-bit arithmetic and shifts", kInt64Arith, false},
        {"uint_arith", "unsigned arithmetic and comparison", kUintArith, false},
        {"compare_branch", "a branch taken and not taken, both ways", kCompareBranch, false},
        {"float_compare", "float comparisons driving branches", kFloatCompare, false},
        {"bitwise_shift", "bitwise and shift operations on a loop counter", kBitwiseShift, false},
        {"convert", "every numeric conversion the translator covers", kConvert, false},
        {"nested_loop", "two nested loops, small inner body", kNestedLoop, false},
        {"globals", "reads and writes of module-level variables", kGlobals, false},
        {"many_locals", "sixteen live locals, so frame offsets have to be right", kManyLocals, false},
        {"member_access", "field reads and writes through methods", kMemberAccess, true},
        {"handle_guards", "null guards that pass, on every iteration", kHandleGuards, false},

        {"native_calls", "loop body is a single call - the realistic shape", kNativeCalls, true, false, false,
         /*run_full_coverage_variant=*/true},
        {"generic_calls", "the same round trip through the generic variadic marshaller", kGenericCalls, true},
        {"script_calls", "script function calling script function", kScriptCalls, true, false, false,
         /*run_full_coverage_variant=*/true},
        {"array_methods", "array indexing/length/insert are all method calls", kArrayMethods, false},
        {"array_index", "indexed access to a fixed array", kArrayIndex, false, false, false,
         /*run_full_coverage_variant=*/true},
        {"string_build", "string assignment and concatenation", kStringBuild, false},
        {"handles", "counted handle assignment, still handed back", kHandles, false},
        {"handles_nocount", "no-count handle assignment, which is translated", kHandlesNoCount, false},
        {"value_objects", "POD struct copies, the only opcode needing rdi/rsi", kValueObjects, false},
        {"division", "floating-point division, guard never taken", kDivision, false},
        {"pow_double", "the floating-point exponents, all three forms", kPowDouble, false},
        {"pow_int", "the integer exponents, all four widths", kPowInt, false},
        {"switch_heavy", "a switch per iteration", kSwitchHeavy, false},

        // Constructs the cases above do not reach, in the order they were added rather than by how
        // they perform - several of them are expected to lose, and the point of the group is the
        // instruction each one puts under the translator rather than the ratio it produces.
        {"bool_values", "comparisons stored as bools, and handle identity", kBoolValues, false},
        {"short_circuit", "conditions built from conditions with && || !", kShortCircuit, false},
        {"expression_depth", "one deep expression per iteration, ternary inside it", kExpressionDepth, false},
        {"recursion", "a function that re-enters itself", kRecursion, false, false, false,
         /*run_full_coverage_variant=*/true},
        {"funcdef_callback", "a call through a function pointer that moves", kFuncdefCallback, false},
        {"array_literal", "an array built from a list initialiser", kArrayLiteral, false},
        {"dictionary_literal", "a dictionary built from a list initialiser", kDictionaryLiteral, false},
        {"try_catch", "a try guard on a path that never throws", kTryCatch, false},
        {"try_catch_throwing", "the same guard with the catch running every fourth iteration",
         kTryCatchThrowing, false},
        {"string_compare", "equality and ordering on strings, no building", kStringCompare, false},
        {"dictionary_access", "dictionary lookups through an out parameter", kDictionaryAccess, false},
        {"do_while_break", "a bottom-tested loop, with break and continue", kDoWhileBreak, false},

        // Numeric families the rows above never produce. Added after counting the emitter's cases
        // against what the suite had compiled, rather than because a shape looked missing.
        {"modulo", "integer remainder in all four widths", kModulo, false},
        {"negation", "unary minus on every numeric type", kNegation, false},
        {"float_division", "single-precision division, and float against int", kFloatDivision, false},
        {"int64_bitwise", "and/or/not and arithmetic shift on a signed 64-bit value", kInt64Bitwise, false},
        {"convert_64", "64-bit and unsigned conversions, both directions", kConvert64, false},
        {"narrow_ints", "byte- and word-wide locals and globals", kNarrowInts, false},
        {"handle_parameter", "a handle parameter, and the guard around it", kHandleParameter, true},
        {"increments", "increment as an expression, on four types", kIncrements, false},
        {"mixed_numeric", "arithmetic shift, float remainder, float against int literal", kMixedNumeric, false},
        {"wide_globals", "globals of four widths, each stored a constant", kWideGlobals, false},
        {"by_reference", "reference parameters, read through and written through", kByReference, true},
        {"handle_conversion", "a handle assigned without the @ operator", kHandleConversion, true},
        {"globals_compound", "read-modify-write of a global, in two widths", kGlobalCompound, false},
        {"reference_return", "an object handed back by reference, the PshRPtr shape", kReferenceReturn, false},
        {"cast_heavy", "the reference cast, taking and dropping a reference per iteration", kCastHeavy, false},

        {"tick_spawn_vehicle", "reconstruction of the shipped example script", kSpawnVehicleTick, true},
        {"tick_with_arithmetic", "the same tick with arithmetic between the calls", kTickWithArithmetic, false},
        {"tick_with_switch", "the tick dispatching on an event kind", kTickWithSwitch, false},
        {"tick_with_state", "the tick carrying state in an object between frames", kTickWithState, false},
        {"many_short_ticks", "small scripts entered and left once per tick", kManyShortTicks, false, false, false,
         /*run_full_coverage_variant=*/true},

        // The two instructions translated most recently, and the two engine configurations the menu
        // does not run. The first two exist because the rows above could not resolve them: `handles`
        // constructs once outside its loop and `funcdef_callback` carries a function pointer rather
        // than a delegate, so nothing in the suite had ever put either translation under a timer at the
        // rate it runs at. The second two exist because a translated instruction that no row can
        // produce is a translation nobody has watched work.
        {"script_object_loop", "a script object constructed and destroyed per iteration", kScriptObjectLoop, true},
        {"delegate_call", "a method bound to an object, called through a funcdef", kDelegateCall, true, false, false,
         /*run_full_coverage_variant=*/true},
        {"int_divide", "integer division and remainder, both widths, both signednesses", kIntDivide, false,
         /*run_integer_division_variant=*/true},
        {"unoptimized", "bools as conditions and null handles, with the optimizer off", kUnoptimized, false,
         /*run_integer_division_variant=*/false, /*run_bytecode_optimizer_variant=*/true},
    };
    return workloads;
  }

} // namespace base::asjit::bench
