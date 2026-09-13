//
// Created by X-ray on 13/09/2026.
//

// Every test here runs the same script twice - once interpreted, once compiled - and compares the
// output. The expectation is never written out by hand, because a hand-written expectation would
// only be as trustworthy as the reading of the interpreter that produced it, and the whole risk with
// a JIT is that a translation is *nearly* right. The interpreter is the specification.
//
// The scripts are grouped by which group of instruction handlers they drive, so that a failure names
// the part of the translator to look at.

#include "test_support.hpp"

#include <gtest/gtest.h>

#include <string>

namespace base::asjit::test {
  namespace {
    [[nodiscard]] std::string Source(const char* text) { return text; }
  } // namespace

  TEST(Differential, IntegerArithmetic) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        int a = 7;
        int b = 3;
        show(a + b);
        show(a - b);
        show(a * b);
        show(a / b);
        show(a % b);
        show(-a);
        show(a + 100000);
        show(a - 100000);
        show(a * 100000);

        int big = 2147483647;
        show(big + 1);
        show(big * 2);
        int small = -2147483647 - 1;
        show(small);
        show(small / 1);
        show(small % 1);
      }
    )AS"), "integer arithmetic");
  }

  TEST(Differential, IntegerComparisons) {
    // The comparisons live in their own function rather than in a loop over a table, so that each one
    // is a compile-time pair of operands. The pairs are the ones where a signed comparison written
    // with the wrong condition code would disagree: the two ends of the range, and zero.
    ExpectJitMatchesInterpreter(Source(R"AS(
      void Compare(int a, int b) {
        show(a < b);
        show(a > b);
        show(a <= b);
        show(a >= b);
        show(a == b);
        show(a != b);
      }

      void main() {
        Compare(-2147483647, -1);
        Compare(-1, 0);
        Compare(0, 1);
        Compare(1, 2147483647);
        Compare(-2147483647, 2147483647);
        Compare(2147483647, -2147483647);
        Compare(0, 0);
        Compare(5, 5);

        // The most negative value has no positive counterpart, which is where a comparison that
        // negated one of its operands to reach a subtraction would go wrong.
        int lowest = -2147483647 - 1;
        Compare(lowest, 0);
        Compare(lowest, -1);
        Compare(lowest, 1);
      }
    )AS"), "integer comparisons");
  }

  TEST(Differential, UnsignedArithmeticAndBitwise) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void Compare(uint a, uint b) {
        show(a < b);
        show(a > b);
        show(a <= b);
        show(a >= b);
        show(a == b);
      }

      void main() {
        uint a = 0x80000000;
        uint b = 3;
        show(a >> 1);
        show(a << 1);
        show(a | b);
        show(a & b);
        show(a ^ b);
        show(~a);

        uint c = 0xFFFFFFFF;
        show(c);
        show(c >> 4);
        show(c >> 31);
        show(c / b);
        show(c % b);
        show(c + 1);
        show(c * 2);

        // The unsigned comparisons, whose whole point is that the high bit is not a sign.
        Compare(0, 1);
        Compare(1, 2147483647);
        Compare(2147483647, 2147483648);
        Compare(2147483648, 4294967295);
        Compare(4294967295, 0);
        Compare(4294967295, 4294967294);
        Compare(0x80000000, 0x7FFFFFFF);
      }
    )AS"), "unsigned arithmetic and bitwise");
  }

  TEST(Differential, ShiftAmounts) {
    // The interpreter shifts by a count read out of a variable, and the C++ it compiles to masks the
    // count to five bits on x86 - so a shift of 32 is a shift of 0 rather than an undefined result.
    // The translation has to reproduce that, which it does by using the same instruction, and this is
    // what says so.
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        int value = 0x0F0F0F0F;
        for (int shift = 0; shift < 40; shift++) {
          show(value << shift);
          show(value >> shift);
        }
        uint uvalue = 0xF0F0F0F0;
        for (uint shift = 0; shift < 40; shift++) {
          show(uvalue << shift);
          show(uvalue >> shift);
        }
      }
    )AS"), "shift amounts past the width");
  }

  TEST(Differential, Loops) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        int sum = 0;
        for (int i = 0; i < 100; i++) {
          sum += i * 2 - 1;
        }
        show(sum);

        int j = 0;
        while (j < 50) {
          j += 3;
        }
        show(j);

        int k = 10;
        do {
          k--;
        } while (k > 0);
        show(k);

        int total = 0;
        for (int i = 0; i < 20; i++) {
          for (int m = 0; m < 20; m++) {
            if ((i + m) % 3 == 0) {
              total += i;
            } else {
              total -= m;
            }
          }
        }
        show(total);

        // A loop that never runs, and a loop that runs exactly once - the two cases an off-by-one in
        // a compiled back edge would show up in first.
        int never = 0;
        for (int i = 10; i < 10; i++) { never++; }
        show(never);
        int once = 0;
        for (int i = 0; i < 1; i++) { once++; }
        show(once);
        int downToZero = 0;
        for (int i = 5; i > 0; i--) { downToZero++; }
        show(downToZero);
      }
    )AS"), "loops");
  }

  TEST(Differential, FloatArithmetic) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        float a = 1.5f;
        float b = 0.25f;
        show(a + b);
        show(a - b);
        show(a * b);
        show(a / b);
        show(-a);
        show(a + 0.5f);
        show(a - 0.5f);
        show(a * 0.5f);

        show(a < b);
        show(a > b);
        show(a <= b);
        show(a >= b);
        show(a == b);
        show(a != b);

        float c = 0.0f;
        show(-c);
        show(c == 0.0f);
      }
    )AS"), "float arithmetic");
  }

  TEST(Differential, DoubleArithmetic) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        double a = 1.0 / 3.0;
        double b = 0.25;
        show(a);
        show(a + b);
        show(a - b);
        show(a * b);
        show(a / b);
        show(-a);
        show(a < b);
        show(a > b);
        show(a == a);
      }
    )AS"), "double arithmetic");
  }

  TEST(Differential, InfinitiesAndNaN) {
    // The interpreter compares rather than subtracting in order to get infinity right, and it does
    // not special-case NaN - so an unordered comparison falls through to "+1". Both of those are
    // things a translation that reached for the obvious instruction would get wrong, and both show
    // up here.
    ExpectJitMatchesInterpreter(Source(R"AS(
      float MakeInfinity() {
        float big = 3.0e38f;
        return big * big;
      }

      void main() {
        float inf = MakeInfinity();
        show(inf);
        show(inf > 1.0f);
        show(inf < 1.0f);
        show(inf == inf);

        float nan = inf - inf;
        show(nan);
        show(nan > 0.0f);
        show(nan < 0.0f);
        show(nan == nan);
        show(nan != nan);
      }
    )AS"), "infinities and NaN");
  }

  TEST(Differential, Conversions) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        int i = 130;
        int8 sb = int8(i);
        show(int(sb));
        uint8 ub = uint8(i);
        show(uint(ub));

        int16 sw = int16(-500);
        show(int(sw));
        uint16 uw = uint16(60000);
        show(uint(uw));

        int negative = -300;
        show(int(int8(negative)));
        show(uint(uint8(negative)));
        show(int(int16(negative)));
        show(uint(uint16(negative)));

        show(int(3.75f));
        show(uint(3.75f));
        show(int(-3.75f));
        show(int(3.75));
        show(uint(3.75));
        show(double(7));
        show(double(4000000000));
        show(float(7));
        show(float(4000000000));
        show(double(float(1.5f)));
        show(float(double(1.5)));
        show(double(2000000000));
        show(float(-123456789));
      }
    )AS"), "conversions");
  }

  TEST(Differential, FloatToUnsignedIntGoesThroughSigned) {
    // `asBC_fTOu` and `asBC_dTOu` are the two conversions the interpreter does not perform as such:
    // both cast to `int` first and reinterpret the bits as unsigned, and the comment there says why -
    // "on some compilers the cast of a negative float value to uint result in 0". So the translation
    // is the instruction the *first* cast compiles to, which is what makes the two agree.
    //
    // Every value below is printed through both casts, so the relationship is visible in the output as
    // well as asserted: the unsigned reading is the signed one reinterpreted. The interesting values
    // are the ones above 2^31, where the intermediate `int` is out of range and what happens is the
    // platform's own answer rather than the language's - the interpreter's C cast and the
    // translation's `cvttss2si`/`cvttsd2si` are the same instruction, so a translation that had read
    // this as a conversion to the full unsigned range, or as a saturating one, would disagree here and
    // agree on everything a test with small values would have checked. The last pair is far enough out
    // that only the pattern is left.
    //
    // The parameters are what keeps any of it out of the optimizer's hands: a value that is a literal
    // all the way to the cast is a value the compiler can fold, and a folded conversion is not a
    // translation of anything.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void Report(float f, double d) {
        show(int(f));
        show(uint(f));
        show(int(d));
        show(uint(d));
      }

      void main() {
        Report(3.75f, 3.75);
        Report(-3.75f, -3.75);
        Report(2147483648.0f, 2147483648.0);      // exactly 2^31, the first value past the signed range
        Report(4000000000.0f, 4000000000.0);      // inside the unsigned range, outside the signed one
        Report(-2147483904.0f, -2147483904.0);    // below the signed range
        Report(1e30f, 1e30);
      }
    )AS"), "float and double to unsigned int",
        {AngelScript::asBC_fTOi, AngelScript::asBC_fTOu, AngelScript::asBC_dTOi, AngelScript::asBC_dTOu});
  }

  TEST(Differential, Globals) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      int g_counter = 5;
      double g_value = 0.0;
      float g_ratio = 1.0f;
      string g_text = "start";

      void Bump() {
        g_counter += 1;
      }

      void main() {
        show(g_counter);
        g_counter = 42;
        show(g_counter);
        Bump();
        show(g_counter);

        g_value = 1.0 / 8.0;
        show(g_value);
        g_ratio = 3.0f / 4.0f;
        show(g_ratio);

        g_text = "changed";
        show(g_text);

        for (int i = 0; i < 10; i++) {
          g_counter += i;
        }
        show(g_counter);
      }
    )AS"), "globals");
  }

  TEST(Differential, FunctionCalls) {
    // Calls are not translated - the block ends at one and the interpreter runs it - so this is
    // really a test of the hand-off being seamless in both directions, including across the
    // recursion in the middle of it.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Add(int a, int b) {
        return a + b;
      }

      int Fibonacci(int n) {
        if (n < 2) {
          return n;
        }
        return Fibonacci(n - 1) + Fibonacci(n - 2);
      }

      int SumTo(int n) {
        int total = 0;
        for (int i = 0; i <= n; i++) {
          total = Add(total, i);
        }
        return total;
      }

      void main() {
        show(Add(3, 4));
        show(Fibonacci(15));
        show(SumTo(100));
      }
    )AS"), "function calls");
  }

  TEST(Differential, SwitchStatement) {
    // A switch compiles to asBC_JMPP followed by a run of asBC_JMP - the "table" is not a table at
    // all, it is the jumps themselves and the selector indexes into them. The translation turns that
    // into a compare chain, so this is the test that the chain picks the same entry the interpreter's
    // own program-counter arithmetic picks.
    //
    // The loop deliberately runs past both ends of the case range. The compiler emits a range check
    // ahead of the JMPP, so an out-of-range value reaches the default arm rather than the instruction,
    // and the two arms have to agree about which arm that is.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int Classify(int value) {
        int result = 0;
        switch (value) {
          case 0: result = 100; break;
          case 1: result = 200; break;
          case 2: result = 300; break;
          case 3: result = 400; break;
          default: result = -1; break;
        }
        return result;
      }

      void main() {
        for (int i = -2; i < 8; i++) {
          show(Classify(i));
        }
      }
    )AS"), "switch statements", {AngelScript::asBC_JMPP, AngelScript::asBC_JMP});
  }

  TEST(Differential, SwitchWithoutDefaultAndWithFallthrough) {
    // The two shapes the first switch test does not contain. A switch with no `default` still gets a
    // table entry for the out-of-range case - it lands past the switch - so the number of entries is
    // not the number of `case` labels, and a translation that assumed it was would run the body after
    // the switch instead of skipping it.
    //
    // The fallthrough cases matter for the same reason from the other side: `case 1` has no `break`,
    // so its table entry and `case 2`'s lead to the same code. Both entries must reach it.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int NoDefault(int value) {
        int result = 7;
        switch (value) {
          case 0: result = 10; break;
          case 1: result = 11; break;
        }
        return result;
      }

      int Fallthrough(int value) {
        int result = 0;
        switch (value) {
          case 0: result += 1;
          case 1: result += 2;
          case 2: result += 4; break;
          case 3: result += 8;
          default: result += 16; break;
        }
        return result;
      }

      void main() {
        for (int i = -1; i < 5; i++) {
          show(NoDefault(i));
          show(Fallthrough(i));
        }
      }
    )AS"), "switch without default, and with fallthrough",
                                {AngelScript::asBC_JMPP, AngelScript::asBC_JMP});
  }

  TEST(Differential, MemberAddressesThroughThisAndReferences) {
    // asBC_LoadThisR and asBC_LoadRObjR compute the address of a member and put it in the value
    // register; neither touches the stack. Both are guards - they dereference a variable to find
    // their base - so they are also the two that have a fault route, and a null `this` or a null
    // reference has to leave through it rather than crash.
    //
    //   LoadThisR  - `this.field`, from the frame's own `this` pointer
    //   LoadRObjR  - a reference variable's member, so the variable is dereferenced first
    //
    // asBC_LoadVObjR, the value form of LoadRObjR, is deliberately absent and cannot be added here:
    // it has no producer in this SDK. The bytecode optimizer forms it from `PSF x, ADDSi, PopRPtr`
    // (as_bytecode.cpp:929), but every member-access site in the compiler emits `PSF 0, RDSPtr,
    // ADDSi` instead, which that rule does not match. The translation is kept because the opcode is
    // part of the format and a restored module could carry one, but no script compiles to it.
    //
    // The members are read and written in both directions, because a base address that is off by a
    // constant reads plausibly and only shows up when the same field is written back.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      class Counter {
        int value = 0;
        int step = 3;
        int total = 0;

        void Bump() {
          value += step;
          total += value;
        }

        int Value() const {
          return value;
        }
      }

      class Wrapper {
        Counter inner;
        int BumpThroughReference(Counter &in target) {
          target.Bump();
          return target.Value();
        }
      }

      void main() {
        Counter local;
        for (int i = 0; i < 6; i++) {
          local.Bump();
          show(local.Value());
          show(local.total);
        }

        Wrapper wrapper;
        show(wrapper.BumpThroughReference(local));
        show(local.total);
      }
    )AS"), "member addresses through this and through references",
                                {AngelScript::asBC_LoadThisR, AngelScript::asBC_LoadRObjR});
  }

  TEST(Differential, NullThisRaisesLikeTheInterpreter) {
    // The fault route of asBC_LoadThisR. `this` is null only when a method is called on a null
    // handle, and the interpreter catches it at the first member access rather than at the call -
    // which is the point, since the call itself is not translated and the access is.
    //
    // The cast is what makes the handle null while still typed, so the method call resolves at
    // compile time and the failure happens at run time, inside the compiled body.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      class Counter {
        int value = 4;
        int Value() const { return value; }
      }

      void main() {
        Counter@ held = Counter();
        show(held.Value());
        @held = null;
        show(held.Value());
      }
    )AS"), "a method called through a null handle",
                                               {AngelScript::asBC_LoadThisR});
  }

  TEST(Differential, NullMemberReferenceRaisesLikeTheInterpreter) {
    // The fault route of asBC_LoadRObjR, whose base is a pointer *read out of a variable* rather
    // than a constant offset from the frame. Reaching it with a null base takes a handle member that
    // was never assigned: the member is a reference once it is followed, and following it is the
    // LoadRObjR. The method body is where the access has to be, because that is where the compiler
    // can inline the member access instead of going through a property call.
    //
    // A null `&in` parameter would be the more direct route and is not available - the compiler
    // rejects a null handle where a reference is expected, so the null has to come from a member
    // that the class itself never initialised.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      class Inner {
        int value = 4;
        int Value() const { return value; }
      }

      class Outer {
        Inner@ inner;
        int InnerValue() const { return inner.Value(); }
      }

      void main() {
        Outer@ outer = Outer();
        show(outer.InnerValue());
      }
    )AS"), "a member reference left null",
                                               {AngelScript::asBC_LoadRObjR});
  }

  TEST(Differential, ObjectsAndStrings) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        array<int> values = { 1, 2, 3, 4, 5 };
        int sum = 0;
        for (uint i = 0; i < values.length(); i++) {
          sum += values[i];
        }
        show(sum);

        values.insertLast(6);
        show(values.length());

        string text = "abc";
        text += "def";
        show(text);
        show(text.length());
      }
    )AS"), "objects and strings");
  }

  TEST(Differential, NestedConditions) {
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Sign(int value) {
        if (value < 0) {
          return -1;
        } else if (value > 0) {
          return 1;
        }
        return 0;
      }

      void main() {
        for (int i = -5; i <= 5; i++) {
          show(Sign(i));
        }

        // Conditions written so that both the taken and the not-taken side of each is a block of its
        // own, which is where a mis-bound label would show up as a wrong branch rather than a crash.
        for (int i = 0; i < 20; i++) {
          if (i % 2 == 0) {
            if (i % 3 == 0) {
              show(i);
            }
          }
          if (i > 10 && i < 15) {
            show(-i);
          }
        }
      }
    )AS"), "nested conditions");
  }

  TEST(Differential, Int64Arithmetic) {
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int64 MakeBig() { return 5000000000; }
      uint64 MakeHuge() { return 18000000000; }

      void main() {
        int64 a = MakeBig();
        int64 b = 7;
        show(a + b);
        show(a - b);
        show(a * b);
        show(a & b);
        show(a | b);
        show(a ^ b);
        show(~a);
        show(-a);
        show(a << 3);
        show(a >> 2);
        // `>>` is a logical shift in AngelScript even for a signed type; the arithmetic one is its
        // own operator, and asBC_BSRA64 is only ever emitted for it.
        show(a >>> 2);
        show(b << 40);
        show(a < b);
        show(a > b);
        show(a <= b);
        show(a >= b);
        show(a == b);
        show(a != b);

        // The ends of the range, where a signed comparison written with the wrong condition code
        // disagrees, and where a 64-bit operation implemented at 32 bits silently truncates.
        int64 highest = 9223372036854775807;
        show(highest);
        show(highest + 1);
        show(highest < 0);
        int64 lowest = -9223372036854775807 - 1;
        show(lowest);
        show(-lowest);
        show(lowest - 1);

        uint64 u = MakeHuge();
        show(u);
        show(u >> 1);
        show(u << 1);
        show(u & 0xFFFFFFFF);
        show(u | 1);
        show(u ^ 0xFFFF);

        // The unsigned comparisons, which are what asBC_CMPu64 exists for - a signed comparison here
        // would call every value above 2^63 smaller than every value below it.
        uint64 small = 1;
        show(u < small);
        show(u > small);
        show(u == small);
        show(u != small);
        show(small <= u);
        show(small >= u);

        uint64 umax = 18446744073709551615;
        show(umax);
        show(umax + 1);
        show(umax / 3);
        show(umax % 3);
        show(umax >> 40);
        show(-a >> 3);
      }
    )AS"), "64-bit arithmetic",
        {AngelScript::asBC_ADDi64, AngelScript::asBC_SUBi64, AngelScript::asBC_MULi64,
         AngelScript::asBC_BAND64, AngelScript::asBC_BOR64, AngelScript::asBC_BXOR64,
         AngelScript::asBC_BNOT64, AngelScript::asBC_NEGi64, AngelScript::asBC_BSLL64,
         AngelScript::asBC_BSRA64, AngelScript::asBC_CMPi64, AngelScript::asBC_CMPu64});
  }

  TEST(Differential, Int64Conversions) {
    // The two shapes matter here. The float conversions take a second operand and the double ones
    // work in place, and reading the source from the wrong slot is the kind of mistake that produces
    // a plausible wrong number rather than a failure.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int64 MakeBig() { return 5000000000; }
      uint64 MakeHuge() { return 18000000000; }
      int MakeInt() { return 123456789; }
      uint MakeUInt() { return 4000000000; }
      float MakeFloat() { return 1.5f; }
      double MakeDouble() { return 2.75; }

      void main() {
        int64 a = MakeBig();
        uint64 u = MakeHuge();

        show(double(a));
        show(float(a));
        show(int(a));
        show(double(u));
        show(float(u));

        show(int64(MakeInt()));
        show(uint64(MakeUInt()));
        show(int64(MakeFloat()));
        show(uint64(MakeFloat()));
        show(int64(MakeDouble()));
        show(uint64(MakeDouble()));

        show(int64(-1));
        show(uint64(-1));
        show(int64(0.5));
        show(uint64(0.5));
        show(int64(-0.5));
      }
    )AS"), "64-bit conversions",
        {AngelScript::asBC_iTOi64, AngelScript::asBC_uTOi64, AngelScript::asBC_i64TOi,
         AngelScript::asBC_i64TOd, AngelScript::asBC_i64TOf, AngelScript::asBC_u64TOd,
         AngelScript::asBC_u64TOf, AngelScript::asBC_dTOi64, AngelScript::asBC_dTOu64,
         AngelScript::asBC_fTOi64, AngelScript::asBC_fTOu64});
  }

  TEST(Differential, UnsignedInt64ToFloatingPointRounding) {
    // The one conversion with no instruction behind it. Values at and above 2^53 have to round
    // correctly, which is what the sticky bit in the translation is for - without it half of these
    // would be one unit in the last place low.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      uint64 Build(uint64 seed) { return seed; }

      void main() {
        uint64 a = 9007199254740993;   // 2^53 + 1
        show(double(a));
        show(double(a + 1));
        show(double(a + 2));
        show(double(a + 3));
        show(float(a));

        uint64 big = 18446744073709551615;  // 2^64 - 1
        show(double(big));
        show(float(big));
        show(double(big - 1));
        show(double(9223372036854775808));  // 2^63, which is negative as a signed value
        show(float(9223372036854775808));
        show(double(Build(4611686018427387905)));
        show(float(Build(4611686018427387905)));
      }
    )AS"), "unsigned 64-bit to floating point",
        {AngelScript::asBC_u64TOd, AngelScript::asBC_u64TOf});
  }

  TEST(Differential, BooleanTestsFromComparisons) {
    // TS/TNS/TP/TNP exist so that a comparison feeding a bool does not need a second instruction.
    // They test the low dword of the value register and clear all eight bytes afterwards.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int Sign(int value) { return value; }

      void main() {
        for (int i = -3; i <= 3; i++) {
          int v = Sign(i);
          bool negative = v < 0;
          show(negative);
          bool notNegative = v >= 0;
          show(notNegative);
          bool positive = v > 0;
          show(positive);
          bool notPositive = v <= 0;
          show(notPositive);
        }
      }
    )AS"), "boolean tests from comparisons",
        {AngelScript::asBC_TS, AngelScript::asBC_TNS, AngelScript::asBC_TP, AngelScript::asBC_TNP});
  }

  TEST(Differential, HandleIdentity) {
    // asBC_CmpPtr, which is how `is` and `!is` are compiled.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      class Thing {
        int value = 0;
      }

      void main() {
        Thing@ first = Thing();
        Thing@ second = Thing();
        Thing@ alias = first;

        show(first is second);
        show(first is first);
        show(first is alias);
        show(first !is second);
        show(first is null);
        show(alias !is null);

        @alias = null;
        show(alias is null);
        show(first is null);
      }
    )AS"), "handle identity", {AngelScript::asBC_CmpPtr});
  }

  TEST(Differential, HandlesThroughCalls) {
    // Passing and returning handles is where the object register and the stack-slot handle
    // instructions appear: the argument has to be moved out of the variable and into the call frame.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int Sum(array<int> @values) {
        int total = 0;
        for (uint i = 0; i < values.length(); i++) {
          total += values[i];
        }
        return total;
      }

      array<int> Build(int seed) {
        array<int> a = {seed, seed + 1, seed + 2};
        return a;
      }

      void main() {
        array<int> values = Build(10);
        show(Sum(values));
        show(values.length());
        show(Sum(Build(100)));
      }
    )AS"), "handles through calls",
        {AngelScript::asBC_STOREOBJ, AngelScript::asBC_GETOBJ, AngelScript::asBC_GETOBJREF});
  }

  TEST(Differential, ValuesThroughReferences) {
    // The RDR and WRTV families, which are how a primitive is read and written through a reference:
    // the address goes into the value register and the datum is moved to or from it at the width the
    // reference names.
    //
    // `&out` and `&in` rather than `&inout`: AngelScript refuses `&inout` on anything that does not
    // support object handles, and these are all primitives. The two directions are what the opcodes
    // are split by anyway, so nothing about the coverage is lost.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void AssignInt(int &out target, int value) { target = value; }
      int ReadInt(int &in source) { return source; }
      void AssignByte(int8 &out target, int8 value) { target = value; }
      void AssignShort(int16 &out target, int16 value) { target = value; }
      void AssignLong(int64 &out target, int64 value) { target = value; }
      int16 ReadShort(int16 &in source) { return source; }
      int64 ReadLong(int64 &in source) { return source; }

      void main() {
        int a = 0;
        AssignInt(a, 42);
        show(a);
        show(ReadInt(a));

        for (int i = 0; i < 10; i++) {
          AssignInt(a, i * 3);
        }
        show(a);

        int8 b = 0;
        AssignByte(b, int8(-7));
        show(int(b));

        int16 c = 0;
        AssignShort(c, int16(-30000));
        show(int(c));
        show(int(ReadShort(c)));

        int64 d = 0;
        AssignLong(d, 5000000000);
        show(d);
        show(ReadLong(d));
      }
    )AS"), "values through references",
        {AngelScript::asBC_RDR2, AngelScript::asBC_RDR4, AngelScript::asBC_RDR8,
         AngelScript::asBC_WRTV1, AngelScript::asBC_WRTV2, AngelScript::asBC_WRTV4,
         AngelScript::asBC_WRTV8});
  }

  TEST(Differential, PropertyAccessThroughValueObjects) {
    // The same class held by value and by handle. Both have to work; they compile to entirely
    // different instructions, and only one of them goes through the object register.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      class Counter {
        int value = 0;
        float ratio = 1.0f;
        double precise = 0.0;
      }

      void main() {
        Counter value;
        Counter@ handle = Counter();

        for (int i = 0; i < 20; i++) {
          value.value += i;
          value.ratio *= 1.5f;
          value.precise += 0.5;

          handle.value += i;
          handle.ratio *= 1.5f;
          handle.precise += 0.5;
        }

        show(value.value);
        show(handle.value);
        show(value.ratio);
        show(handle.ratio);
        show(value.precise);
        show(handle.precise);
      }
    )AS"), "property access through value objects and handles",
        {AngelScript::asBC_LOADOBJ});
  }

  TEST(Differential, PointerArgumentsAndOffsets) {
    // The instructions that reach a member through a pointer, and the three null checks that go with
    // them. All of these are the same two-instruction shape - follow the handle, add the member's
    // offset - preceded by a check that what was followed was not null, and all of them are what
    // ordinary code over handles compiles into.
    //
    // Every construct here is load-bearing, which is worth saying because the obvious script does not
    // produce all of them. Each was chosen after reading the bytecode the engine emits:
    //
    //   RDSPtr   needs a handle that is followed rather than checked - a member accessed through a
    //            handle member, or a global handle, not a local.
    //   ChkRefS  needs a *reference to a handle*: a global handle given to a `Thing &in` parameter
    //            produces the address of the handle, which has to be dereferenced before it can be
    //            checked. No amount of local handle traffic emits it, and the global's own member
    //            access does not either - that one follows the handle rather than checking it.
    //   ChkNullV needs a handle that is not a plain variable - a function's handle return value, which
    //            the compiler stores into a hidden temporary and then checks.
    //   CHKREF   needs a handle used as an object, such as passing one to a by-value parameter.
    //
    // A test written from intuition about what "uses handles" gets RDSPtr and CHKREF and nothing else,
    // and would have gone on passing if the other two translations had been deleted.
    //
    // ChkRefS additionally needs the optimizer off, which is why this test is the one place that asks
    // for a non-default engine. It is emitted above the reference-to-handle conversion and then
    // rewritten away: `ChkRefS, RDSPtr` becomes `RDSPtr, CHKREF` and `PGA, RDSPtr` becomes `PshGPtr`,
    // so an optimizing engine hands the translator RDSPtr and CHKREF instead and there is no ChkRefS
    // left to name.
    //
    // Getting there took two corrections, and they are worth keeping because both were silent. The
    // script used to reach this conversion through a handle assignment that was missing its `@`, which
    // raises on the first statement - so the comparison that this test rests on was agreeing about two
    // equally empty runs. Adding the `@` made the script run and removed the ChkRefS along with the
    // conversion that had produced it, which is what exposed the optimizer's part in this.
    const std::string script = Source(R"AS(
      class Thing {
        int value = 4;
        float ratio = 0.5f;
        int Value() const { return value; }
      }

      class Holder {
        Thing@ item;
        Holder() { @item = Thing(); }
        int Value() const { return item.value; }
      }

      int g_value = 7;
      Thing@ g_handle;

      int ByValue(Thing thing) { return thing.value; }
      int ByReference(Thing &in thing) { return thing.value; }

      Thing@ Make() { return Thing(); }
      int FromTemporary(Thing &in thing) { return thing.value; }

      void main() {
        Holder h;
        Thing@ good = Thing();

        // The `@` is required, and the omission is worth knowing about: `g_handle = Thing()` is a value
        // assignment through the handle rather than a handle assignment, and since the handle is null
        // it raises `Null pointer access` on this statement - in the interpreter first, so the two arms
        // would agree on an empty output and this test would pass without executing anything below it.
        @g_handle = Thing();

        for (int i = 0; i < 5; i++) {
          show(good.value);
        }

        // RDSPtr and ADDSi: the member reached through a handle member, and through a global handle.
        show(h.Value());
        show(g_handle.value);

        // CHKREF: a handle given to a by-value parameter.
        show(ByValue(good));

        // ChkRefS: the global handle - a reference to a handle - given to a reference parameter.
        show(ByReference(g_handle));

        // ChkNullV: a handle returned from a function, then bound to a reference.
        show(FromTemporary(Make()));

        // RDR4 on a plain global, so that the offsets above are read from somewhere that cannot
        // itself be the source of a fault.
        show(g_value);
      }
    )AS");

    // The default engine first. ChkNullV is in this list because of the optimizer rather than because
    // of the compiler: the hidden temporary a handle return value is stored in is checked by a
    // ChkRefS-and-discard that the peephole passes collapse into ChkNullV, so the instruction only
    // exists in an optimized build.
    ExpectJitMatchesInterpreterUsing(script, "pointer arguments and offsets",
                                     {AngelScript::asBC_ADDSi, AngelScript::asBC_RDSPtr, AngelScript::asBC_RDR4,
                                      AngelScript::asBC_ChkNullV, AngelScript::asBC_CHKREF});

    // The claim the paragraph above rests on, checked rather than assumed: the optimizer really does
    // leave no ChkRefS for the translator. If it did not, the arm below would be coverage of the same
    // instruction twice and this test would not be evidence about the optimizer at all.
    EXPECT_EQ(RunScriptWithConfig(script, EngineConfig{.use_jit = true}).Translated(AngelScript::asBC_ChkRefS), 0u)
        << "an optimizing engine handed the translator a ChkRefS, so the arm below is not the only one "
           "that covers it";

    // And the same script with the peephole passes off, which is the only configuration in which the
    // compiler's own ChkRefS survives to be handed to the translator. ChkNullV is absent here for the
    // same reason it was absent above: it is the optimizer's product, and there is no optimizer.
    //
    // Running the script twice in two configurations is the point rather than a cost - the two lists
    // are the same source compiling to different guards, which is what makes it visible that the pair
    // of instructions is the optimizer's business and not the script's.
    ExpectJitMatchesInterpreterUsing(script, "pointer arguments and offsets, unoptimized",
                                     {AngelScript::asBC_ADDSi, AngelScript::asBC_RDSPtr, AngelScript::asBC_RDR4,
                                      AngelScript::asBC_ChkRefS, AngelScript::asBC_CHKREF},
                                     EngineConfig{.optimize_bytecode = false});
  }

  TEST(Differential, ReferenceToHandleGuardRaisesLikeTheInterpreter) {
    // ChkRefS on the path where it fires. The guard dereferences twice - the operand is the address of
    // a handle, and it is the handle that is tested - so the compiled version is the one guard that
    // cannot be got right by testing the operand itself. A null handle passed to a `Thing &in`
    // parameter is what makes the guard fail, and it is the only construct that does: reading a member
    // through the null handle follows it instead of checking it, which is the other guard's job and
    // not this one's.
    //
    // `@g_handle = null` rather than `g_handle = null`: the latter is a plain assignment and the
    // compiler rejects it, which is a useful thing to know and a reason not to rewrite this line.
    //
    // The optimizer off, for the reason the note above the comparison helpers gives: an optimizing
    // engine rewrites `ChkRefS, RDSPtr` into `RDSPtr, CHKREF` and this script would then be testing
    // RDSPtr's failure path, which is a different instruction with a different fault route.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      class Thing {
        int value = 1;
        int Value() const { return value; }
      }

      Thing@ g_handle;

      int ByReference(Thing &in thing) { return thing.value; }

      void main() {
        // With the `@`, so that the loop runs and the raise below is the one this test names. Without
        // it the assignment is a value assignment through a null handle and raises here instead, which
        // the comparison cannot tell apart from the guard firing - both arms would raise the same
        // exception on the first statement and agree.
        @g_handle = Thing();
        for (int i = 0; i < 5; i++) {
          show(ByReference(g_handle));
        }
        @g_handle = null;
        show(ByReference(g_handle));
      }
    )AS"), "reference to handle guard raises",
        {AngelScript::asBC_ChkRefS}, EngineConfig{.optimize_bytecode = false});
  }

  TEST(Differential, NullVariableGuardRaisesLikeTheInterpreter) {
    // ChkNullV on the path where it fires. The null handle has to arrive as a function's return value,
    // because that is the only thing the compiler emits this guard for: a plain `Thing@` variable
    // converted to `Thing&` is a compile error, and a variable used as an object gets CHKREF instead.
    //
    // The call is on the right of a `+` so that the good call is evaluated first and the function
    // being timed has already been entered and left before the raise - a raise out of the first
    // statement would exercise the guard without exercising the machinery around it.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      class Thing {
        int value = 1;
        int Value() const { return value; }
      }

      Thing@ MakeNull() { return null; }
      int FromTemporary(Thing &in thing) { return thing.value; }

      void main() {
        Thing@ held = Thing();
        show(FromTemporary(held) + FromTemporary(MakeNull()));
      }
    )AS"), "null variable guard raises",
        {AngelScript::asBC_ChkNullV, AngelScript::asBC_CHKREF});
  }

  TEST(Differential, SuspendHeavyFunction) {
    // Statements are separated by suspend instructions, and loops carry one on every iteration, so
    // this is a script whose every statement is a hand-off point. The interpreter services each one;
    // the JIT is expected to run straight past them when the context has nothing to do.
    ExpectJitMatchesInterpreter(Source(R"AS(
      void main() {
        int a = 1;
        int b = 2;
        int c = a + b;
        a = c * 3;
        b = a - c;
        c = a + b;
        show(a);
        show(b);
        show(c);
        for (int i = 0; i < 30; i++) {
          a += i;
          b -= i;
          c = a * b;
        }
        show(a);
        show(b);
        show(c);
      }
    )AS"), "suspend heavy function");
  }

  TEST(Differential, SuspendHandoffsMatchTheInterpreter) {
    // A script of the same shape as the one above, in the watchdog configuration: a line callback
    // installed, which is what makes the engine set `doProcessSuspend`, and so what makes every
    // statement's suspend instruction a hand-off point rather than something a compiled block may run
    // past.
    //
    // What this catches that the test above cannot is a block that hands back at the wrong statement.
    // While the watchdog runs, both arms execute nearly every instruction through the interpreter, so
    // the values printed agree whether a compiled block resumes at the statement it stopped on or at
    // the one after it - and an instruction run twice, or skipped, is invisible in the output. What it
    // is not invisible in is the hand-off count, which `ExpectJitMatchesInterpreter` compares when the
    // configuration installs a callback: the engine calls it once per suspend instruction executed
    // with the line moved on, so being one statement out is being one call out.
    //
    // The script hands back from as many kinds of point as the translator has: a statement, a loop
    // body and its back edge, a call to a script function, a native call, and a comparison whose value
    // is stored to a bool rather than branched on - the shape that reads the value register back, and
    // so the one that would notice a branch having consumed it.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Step(int value) { return value * 2 - 1; }

      void main() {
        int total = 0;
        for (int i = 0; i < 12; i++) {
          if (i % 3 == 0) {
            total += Step(i);
          } else {
            total -= i;
          }

          bool large = total > 20;
          show(large);
        }

        show(total);
      }
    )AS"), "suspend hand-offs", EngineConfig{.line_callback = true});
  }

  TEST(Differential, DivisionAndRemainder) {
    // The twelve divisions in one script, so that a single run exercises every width and both
    // signednesses. The divisors walk a range that includes 1, -1, and values of both signs, because
    // -1 is the divisor with a special case in the guard and a test that only ever divided by 3 would
    // never reach it.
    //
    // This is the half of the translation that has to be right for the ordinary case. The four tests
    // below it are the half that has to be right for the case that raises, which is where a JIT that
    // merely got the arithmetic right would still be wrong.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int DivI(int a, int b) { return a / b; }
      int ModI(int a, int b) { return a % b; }
      uint DivU(uint a, uint b) { return a / b; }
      uint ModU(uint a, uint b) { return a % b; }
      float DivF(float a, float b) { return a / b; }
      float ModF(float a, float b) { return a % b; }
      double DivD(double a, double b) { return a / b; }
      double ModD(double a, double b) { return a % b; }
      int64 DivL(int64 a, int64 b) { return a / b; }
      int64 ModL(int64 a, int64 b) { return a % b; }
      uint64 DivUL(uint64 a, uint64 b) { return a / b; }
      uint64 ModUL(uint64 a, uint64 b) { return a % b; }

      void main() {
        for (int i = -12; i <= 12; i++) {
          if (i == 0) continue;
          show(DivI(1000, i));
          show(ModI(1000, i));
          show(DivI(-1000, i));
          show(ModI(-1000, i));
          show(DivU(uint(4000000000), uint(i)));
          show(ModU(uint(4000000000), uint(i)));
          show(DivL(9000000000000, i));
          show(ModL(9000000000000, i));
          show(DivUL(18000000000000000000, uint64(i)));
        }
        // Divisors are constructed so that none of them is zero - a zero divisor here would raise and
        // end the script before the rest of the range had been divided. The negatives are the point:
        // `%` truncates toward zero rather than flooring, so a negative dividend and a positive
        // divisor produce a negative remainder, and an implementation that got that backwards would
        // agree with the interpreter on every positive case.
        for (int i = 1; i <= 12; i++) {
          float fdiv = float(i) + 0.5f;
          double ddiv = double(i) + 0.5;
          show(DivF(100.0f, fdiv));
          show(ModF(100.0f, fdiv));
          show(DivF(-100.0f, fdiv));
          show(ModF(-100.0f, fdiv));
          show(DivD(100.0, ddiv));
          show(ModD(100.0, ddiv));
          show(DivD(-100.0, ddiv));
          show(ModD(-100.0, ddiv));
        }
      }
    )AS"), "division and remainder",
        {AngelScript::asBC_DIVi, AngelScript::asBC_MODi, AngelScript::asBC_DIVu,
         AngelScript::asBC_MODu, AngelScript::asBC_DIVf, AngelScript::asBC_MODf,
         AngelScript::asBC_DIVd, AngelScript::asBC_MODd, AngelScript::asBC_DIVi64,
         AngelScript::asBC_MODi64, AngelScript::asBC_DIVu64, AngelScript::asBC_MODu64});
  }

  TEST(Differential, DivisionByZeroRaisesLikeTheInterpreter) {
    // The guard's whole purpose. The compiled arm reaches the division with a zero divisor, hands the
    // instruction back, and the interpreter raises the engine's own exception from it - so the two
    // arms have to agree on the message as well as on the fact that something was raised, which is
    // what comparing the exception string checks.
    //
    // The zero is read from a global that no pass can fold, and the loop ahead of it divides by
    // everything but zero first, so the guard is exercised on the path where it must not fire before
    // the one where it must.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      int g_zero = 0;

      int DivI(int a, int b) { return a / b; }
      double DivD(double a, double b) { return a / b; }
      float DivF(float a, float b) { return a / b; }
      uint64 DivUL(uint64 a, uint64 b) { return a / b; }

      void main() {
        for (int i = 1; i < 4; i++) {
          show(DivI(100, i));
          show(int(DivD(100.0, double(i))));
          show(int(DivF(100.0f, float(i))));
          show(int(DivUL(100, uint64(i))));
        }
        show(DivI(100, g_zero));
      }
    )AS"), "division by zero raises",
        {AngelScript::asBC_DIVi, AngelScript::asBC_DIVd, AngelScript::asBC_DIVf,
         AngelScript::asBC_DIVu64});
  }

  TEST(Differential, DivideOverflowRaisesLikeTheInterpreter) {
    // The second exceptional case, and the one the hardware does not report at all. The most negative
    // integer divided by -1 has no representable result; `idiv` signals it as the same divide error
    // as a zero divisor, but the engine raises a *different* message for it. A guard that only tested
    // for zero would compute a quotient here and raise nothing, and the two arms would disagree.
    //
    // The division by -4 ahead of it is the other half of the guard's condition: the divisor is -1
    // there too in spirit but the dividend is not the minimum, so the ordinary result must be
    // produced rather than a raise.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      // Globals rather than locals, because both dividends are constant expressions and a local
      // initialized from one is close enough to a constant that the compiler may fold the division
      // away - which would leave the test passing without ever having emitted the opcode it names.
      int g_smallest = -2147483647 - 1;
      int64 g_smallest64 = -9223372036854775807 - 1;

      int DivI(int a, int b) { return a / b; }
      int64 DivL(int64 a, int64 b) { return a / b; }

      void main() {
        show(DivI(g_smallest, -4));
        show(DivL(g_smallest64, -4));
        show(DivI(g_smallest, -1));
      }
    )AS"), "divide overflow raises",
        {AngelScript::asBC_DIVi, AngelScript::asBC_DIVi64});
  }

  TEST(Differential, RemainderOverflowRaisesLikeTheInterpreter) {
    // The same condition reached through `%` rather than `/`, with no division ahead of it. Kept apart
    // from the test above because the guard is shared between the two opcodes and a mistake in the
    // shared part would otherwise show up as one failure rather than two.
    //
    // `INT_MIN % -1` is an ordinary zero as far as the hardware is concerned, so this is the case
    // where a translator that reasoned from what the instruction does rather than from what the
    // engine says would produce a result instead of an exception.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      int g_smallest = -2147483647 - 1;
      int64 g_smallest64 = -9223372036854775807 - 1;

      int ModI(int a, int b) { return a % b; }
      int64 ModL(int64 a, int64 b) { return a % b; }

      void main() {
        show(ModI(g_smallest, -3));
        show(ModL(g_smallest64, -3));
        show(ModI(g_smallest, -1));
      }
    )AS"), "remainder overflow raises",
        {AngelScript::asBC_MODi, AngelScript::asBC_MODi64});
  }

  TEST(Differential, FloatingPointDivisionEdgeCases) {
    // The cases where "the divisor is zero" is not a comparison against a bit pattern.
    //
    // A NaN divisor is not zero and must divide; a negative zero divisor *is* zero and must raise.
    // Testing the divisor with a plain equality branch gets both of these wrong in opposite
    // directions, so both are asserted here rather than left to the coverage of an ordinary value.
    //
    // Both operands come from globals, because both are constant expressions and a local initialised
    // from one is close enough to a constant that the compiler might fold the arithmetic away before
    // the instruction under test is ever emitted.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      // 1.0e40 does not fit a float and becomes an infinity, so `g_huge - g_huge` is inf - inf, which
      // is a NaN. That is the only NaN this script can make: `0.0 / 0.0` would be a division by zero
      // and would raise before the case under test.
      float g_huge = 1.0e40f;
      float g_one = 1.0f;

      float DivF(float a, float b) { return a / b; }
      double DivD(double a, double b) { return a / b; }

      void main() {
        float nan = g_huge - g_huge;

        // Unordered. The interpreter's `divider == 0` is false for a NaN, so it divides and the
        // result is a NaN - which is not equal to itself, so it prints as 1.
        show(DivF(1.0f, nan) != DivF(1.0f, nan) ? 1 : 0);
        show(DivD(1.0, double(nan)) != DivD(1.0, double(nan)) ? 1 : 0);

        // Ordered and *equal* to zero, but not the bit pattern of zero: 0.0 times -1.0 is -0.0, and
        // `-0.0 == 0` is true, so the interpreter must raise here. The comparison is printed first so
        // that the value really is zero, rather than the NaN from above, before it is divided by.
        double negativeZero = double(g_one - g_one) * -1.0;
        show(negativeZero == 0.0 ? 1 : 0);
        show(DivD(1.0, negativeZero));
      }
    )AS"), "floating point division edge cases",
        {AngelScript::asBC_DIVf, AngelScript::asBC_DIVd});
  }

  TEST(Differential, UnsignedDivisionOfLargeValues) {
    // `div` takes the high half of its dividend from rdx, so an unsigned division that sign-extended
    // the dividend instead of clearing it would be wrong for every value above 2^31 - and would be
    // right for every small one. These are the values that tell the two apart.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      uint DivU(uint a, uint b) { return a / b; }
      uint ModU(uint a, uint b) { return a % b; }
      uint64 DivUL(uint64 a, uint64 b) { return a / b; }
      uint64 ModUL(uint64 a, uint64 b) { return a % b; }

      void main() {
        uint high = uint(4294967295);
        show(DivU(high, 3));
        show(ModU(high, 7));
        show(DivU(high, high));
        show(ModU(high, high));

        uint64 highest = 18446744073709551615;
        show(DivUL(highest, 3));
        show(ModUL(highest, uint64(1000000007)));
        show(DivUL(highest, highest));
        show(ModUL(highest, highest));

        // Exactly 2^63, the first value the sign bit sits on. Read as signed it is the most negative
        // int64, and the quotient would come out negative rather than 3074457345618258602.
        show(DivUL(9223372036854775808, 3));
        show(ModUL(9223372036854775808, 3));
      }
    )AS"), "unsigned division of large values",
        {AngelScript::asBC_DIVu, AngelScript::asBC_MODu, AngelScript::asBC_DIVu64,
         AngelScript::asBC_MODu64});
  }

  TEST(Differential, NullHandleAccessRaisesLikeTheInterpreter) {
    // A null handle dereferenced by calling a method on it. The interpreter raises `Null pointer
    // access`; a compiled block that ran past the call instead would fault inside the game process
    // with no script exception at all, which is the difference between a script that can be caught and
    // a crash.
    //
    // The call that reaches the null is itself a translated `asBC_CALLINTF`, which the opcode list
    // below asserts rather than assumes - it used to say the opposite here, and said it in a form that
    // no longer held the moment the call was translated. What is being tested is therefore not the
    // call but what happens one level in: the callee is compiled too, the fault is raised by the guard
    // its own object load emitted, and the raise has to unwind back out through a *caller* whose frame
    // a translated call installed and whose registers a bare epilogue left describing the callee.
    //
    // The null handled by the call instruction itself is a different path and has its own test, which
    // is where a translation that raised from inside the helper is covered.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      class Thing {
        int value = 1;
        int Value() const { return value; }
      }

      class Holder {
        Thing@ item;
        int Value() const { return item.value; }
      }

      int ByValue(Thing thing) { return thing.value; }

      void main() {
        Holder h;
        Thing@ good = Thing();
        for (int i = 0; i < 3; i++) {
          show(good.value);
        }
        show(h.Value());
        show(ByValue(good));
      }
    )AS"), "null handle access raises", {AngelScript::asBC_CALLINTF});
  }

  TEST(Differential, FuncdefHandleAssignmentIsTranslated) {
    // A funcdef handle, which is the one handle type whose reference copy does *not* go through the
    // object type the script declared. A funcdef has no `asCObjectType` of its own - `asCFuncdefType`
    // derives from `asCTypeInfo` and stops there - so the compiler does not put one in the
    // instruction. It puts the engine's shared placeholder for function references instead, whose
    // name is `$func` (as_scriptfunction.cpp:129) and whose flags are `asOBJ_REF | asOBJ_GC`.
    //
    // That distinction is not academic, because the translation reaches the counted path through
    // `asIScriptEngine::ReleaseScriptObject` and `AddRefScriptObject`, and both of *those* begin with
    // a test for `asOBJ_FUNCDEF` that would send a real funcdef type down a different branch. It is
    // not taken here, and the reason is the one above: the type in the instruction carries no such
    // flag, so the engine's routine falls through to `objType->beh`, which is exactly what the
    // interpreter's own handler reads. The two agree because they are reading one object's behaviour
    // table, and this test is what would fail if that ever stopped being true - which is why it exists
    // rather than being left to the general handle tests, whose types are all script classes.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      funcdef int Modifier(int);

      int Doubled(int value) { return value * 2; }
      int Tripled(int value) { return value * 3; }

      void main() {
        Modifier@ apply = @Doubled;
        for (int i = 0; i < 4; i++) {
          show(apply(i));
          if ((i & 1) == 0) {
            @apply = @Tripled;
          } else {
            @apply = @Doubled;
          }
          show(apply(i));
        }
        @apply = null;
        show(0);
      }
    )AS"), "funcdef handle assignment", {AngelScript::asBC_RefCpyV});
  }

  TEST(Differential, InterfaceMethodCallsAgreeWithTheInterpreter) {
    // `asBC_CALLINTF`, which is `asBC_CALL` reached through an interface: the call site names the
    // interface's method, and which implementation runs - and whether the object is even the right
    // shape - is settled inside the engine at the call. The two forms share the translator's case
    // because they share every step of it, and they share this test's reason too: what a translation
    // could get wrong is the frame, not the dispatch, so what has to be compared is a call whose
    // frame is built the same way by both arms.
    //
    // Two implementations behind one interface, chosen by a parameter rather than by a constant, so
    // that the resolution cannot be folded away at compile time and the call really is indirect.
    // `Area` is called on a handle that is a fresh object each iteration, which also means each call
    // assigns a different implementation over the same site.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      interface IShape {
        int Area() const;
      }

      class Square : IShape {
        int side;
        Square(int s) { side = s; }
        int Area() const { return side * side; }
      }

      class Rectangle : IShape {
        int width;
        int height;
        Rectangle(int w, int h) { width = w; height = h; }
        int Area() const { return width * height; }
      }

      IShape@ Make(int which) {
        if (which == 0) return Square(3);
        return Rectangle(2, 5);
      }

      void main() {
        for (int i = 0; i < 4; i++) {
          IShape@ shape = Make(i & 1);
          show(shape.Area());
        }
      }
    )AS"), "interface method calls",
        {AngelScript::asBC_CALLINTF, AngelScript::asBC_CALL});
  }

  TEST(Differential, NullInterfaceHandleRaisesLikeTheInterpreter) {
    // The failure path of the translation above, and the one way it differs from a translated
    // `asBC_CALL`: there the engine routine always installs a frame, and here it cannot, because the
    // object the method would be resolved against is null. The engine raises instead and returns with
    // *this* frame still current.
    //
    // What makes that safe is that the exit publishes this frame's resume point before the call and
    // returns through the `asBC_JitEntry` that entered the block, and that handler tests the
    // execution status before it dispatches anything. Neither of those is added for this case; the
    // test is what establishes that the pair is sufficient - the alternative is a compiled block
    // running on past a raise, which in a game process is a fault with no script exception behind it.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      interface IShape {
        int Area() const;
      }

      class Square : IShape {
        int side;
        Square(int s) { side = s; }
        int Area() const { return side * side; }
      }

      void main() {
        IShape@ shape = Square(3);
        show(shape.Area());
        @shape = null;
        show(shape.Area());
      }
    )AS"), "null interface handle raises", {AngelScript::asBC_CALLINTF});
  }

  TEST(Differential, ExponentOperations) {
    // The floating point powers. Both arms call the same CRT routine here, so what this can disagree
    // about is not the arithmetic but the operands: the base and the exponent are read out of the
    // frame, and `POWdi` reads its exponent as a *dword* while the other two read a qword. Getting
    // that width wrong reads a plausible number out of the wrong half of the frame rather than
    // failing, which is why the mixed form has a case of its own below.
    //
    // Every result here is finite. An overflow raises, which is the next test, and would end this one
    // with the cases after it unrun.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      double g_base = 2.5;
      int g_exponent = 3;

      void main() {
        double d = 1.5;
        double negative = -2.0;
        float f = 2.0f;

        for (int k = 0; k < 4; k++) {
          show(d ** 2.0);
          show(d ** 0.5);
          show(f ** 2.0f);
          show(g_base ** g_exponent);
          show(d ** 3);
          show(negative ** 3.0);
          show(d ** 0.0);
          d += 0.25;
        }
      }
    )AS"), "exponent operations",
        {AngelScript::asBC_POWf, AngelScript::asBC_POWd, AngelScript::asBC_POWdi});
  }

  TEST(Differential, ExponentOverflowRaisesLikeTheInterpreter) {
    // The other half of the guard. The interpreter raises `TXT_POW_OVERFLOW` when the *result* is an
    // infinity - not when an operand is out of range - so this is the only test that reaches the
    // guard's exit rather than its fast path.
    //
    // The bases are large rather than enormous on purpose: a value already infinite would make the
    // result infinite for a reason that has nothing to do with the exponentiation, and would pass
    // whether or not the guard tested the result. `1.0e300` squared is finite in neither arm.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      double g_large = 1.0e300;
      float g_largef = 1.0e30f;

      double ByDouble(double a, double b) { return a ** b; }
      float ByFloat(float a, float b) { return a ** b; }

      void main() {
        show(ByDouble(g_large, 2.0));
        show(ByFloat(g_largef, 3.0f));
      }
    )AS"), "exponent overflow", {AngelScript::asBC_POWd, AngelScript::asBC_POWf});
  }

  TEST(Differential, IntegerExponentMatchesTheInterpreter) {
    // The integer forms are translated now, by calling the interpreter's own routine, so this is a
    // comparison of two different executions rather than the same interpreter twice - which is what
    // the named opcode asserts. Without it this test passes for either reason, and it passed for the
    // wrong one until the translation existed.
    //
    // The inputs are the routine's four shapes rather than a spread of arithmetic: a plain product, a
    // negative base, an exponent of 0, an exponent of -1 (which truncates to zero), and the two
    // bases the `exponent >= 31` early-out special-cases. `0 ** 0` is not here because it raises -
    // the test below has it.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int g_base = 3;

      int Power(int base, int exponent) { return base ** exponent; }

      void main() {
        show(Power(g_base, 4));
        show(Power(-2, 5));
        show(Power(10, 0));
        show(Power(-1, 7));
        show(Power(7, -1));
        show(Power(1, 200));
        show(Power(-1, 201));
        show(Power(0, 200));
      }
    )AS"), "integer exponent", {AngelScript::asBC_POWi});
  }

  TEST(Differential, IntegerExponentOverflowRaisesLikeTheInterpreter) {
    // The boundaries that are not where arithmetic would put them. Both of the 64-bit cases fit in the
    // type they are computed in, and the interpreter raises anyway, because its rule is a table of
    // maximum bases rather than a check on the result: `3037000499 ** 2` is 9223372030926249001, below
    // the largest int64, and `2097153 ** 3` is below the largest uint64.
    //
    // These are the tests the translation has to pass rather than the ones it would like to. The
    // routine reports the table's verdict and the emitter turns a `true` into a branch to the fault
    // exit, which publishes this instruction's own offset - so the interpreter re-runs the same
    // instruction, reaches the same verdict, and raises. If the branch were missing the compiled arm
    // would store a value and finish, and the two arms would disagree about whether the script threw.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void main() {
        int64 base = 3037000499;
        int64 exponent = 2;
        show(base ** exponent);
      }
    )AS"), "int64 exponent at the table's boundary", {AngelScript::asBC_POWi64});

    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void main() {
        uint64 base = 2097153;
        uint64 exponent = 3;
        show(base ** exponent);
      }
    )AS"), "uint64 exponent at the table's boundary", {AngelScript::asBC_POWu64});

    // The two 32-bit forms, which have their own bounds and take the same route out.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void main() {
        int base = 100000;
        int exponent = 3;
        show(base ** exponent);
      }
    )AS"), "int32 exponent past the table's bound", {AngelScript::asBC_POWi});

    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void main() {
        uint base = 100000;
        uint exponent = 3;
        show(base ** exponent);
      }
    )AS"), "uint32 exponent past the table's bound", {AngelScript::asBC_POWu});

    // And the domain error, which the routine reports as an overflow rather than as a separate
    // condition. `0 ** 0` is the one input that reaches it.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void main() {
        int base = 0;
        int exponent = 0;
        show(base ** exponent);
      }
    )AS"), "the domain error", {AngelScript::asBC_POWi});
  }

  TEST(Differential, NoCountHandleAssignmentIsTranslated) {
    // The cheap half of the reference-copy handlers, and the test that reaches it at all.
    //
    // `asBC_REFCPY` and `asBC_RefCpyV` share a handler body that skips the addref and the release
    // entirely for a type carrying `asOBJ_NOCOUNT` or `asOBJ_VALUE`, leaving one pointer store. Every
    // handle this suite created before `Handle` was registered was to a counted type - a script class,
    // a funcdef or an array - so the branch was dead here, and the translation of it was dead with it.
    //
    // `Handle` is that registration, and the named opcodes are what makes this evidence: without them
    // the comparison would pass identically if the translator had declined both instructions and the
    // interpreter had run them in both arms.
    //
    // Both are named because they differ in where the destination is, which is the only thing that
    // distinguishes their translations: `RefCpyV` writes to a named local and `REFCPY` pops a pointer
    // to the destination off the stack. `@a = b;` is the local form and `@g_slot = a;` is the store
    // into a global, which is what puts a destination pointer on the stack for the second. A
    // translation that confused the two would be a `mov` where a `lea` belongs - writing a handle over
    // a stack address - so a test that reached only one of them would leave that untested.
    //
    // The handles are read back through `Value()` rather than through a `show` overload, so that what
    // is compared is the object each handle names rather than a copy taken on the way out. An
    // assignment that stored the wrong pointer would still print something plausible for a type with
    // one member; two distinguishable objects are what make a wrong pointer visible.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      Handle@ g_slot;

      void main() {
        Handle@ a = Handle(1);
        Handle@ b = Handle(2);
        show(a.Value());
        show(b.Value());
        @a = b;
        show(a.Value());
        @g_slot = a;
        show(g_slot.Value());
        show(b.Value());
      }
    )AS"), "no-count handle assignment", {AngelScript::asBC_REFCPY, AngelScript::asBC_RefCpyV});
  }

  TEST(Differential, CountedHandleAssignmentIsTranslated) {
    // The half of the reference copies that the previous test cannot reach: the branch taken for a
    // type that *does* count references, where the assignment releases what the destination held and
    // takes a reference on the source instead of storing a pointer and stopping.
    //
    // That branch is the one a menu script lives in - `@player.vehicle = other;`, `return
    // GetPlayer(id);`, every handle that outlives the statement that made it - so a translation that
    // merely compiled and got the store right would be worse than no translation at all, because the
    // failure mode is not a wrong value but a leak, or a destructor running one reference too early
    // on an object something else is still holding.
    //
    // Which is why this test counts destructions rather than comparing values. The interpreter's arm
    // is the oracle for the emitted one, and the destructor calls back into the script on the way out
    // of an object, so a missed release shows up as a smaller count and a spurious one as an object
    // destroyed while a live handle still names it. Values alone would agree in every one of those
    // cases, because a leaked object still reads back correctly.
    //
    // The `@` prefix is on every assignment for the reason the compiler test spells out: without it
    // the compiler does not assign the handle at all, and the script compiles to something else.
    const std::string source = Source(R"AS(
      class Entity {
        int id = 0;
        Entity(int start) { id = start; }
        ~Entity() { destructed++; }
      }

      class Holder {
        Entity@ entity;
      }

      int destructed = 0;

      Entity@ Pass(Entity@ source) { return source; }

      void main() {
        {
          Entity@ first = Entity(7);
          Entity@ second = Entity(9);
          show(first.id);
          show(second.id);

          // The counted assignment into a named local: whatever `first` held before is released on
          // the way to storing `second`, and both names now hold the same object. The object it let
          // go of is destroyed *here*, at the assignment, and this prints that it was - an assignment
          // that merely stored the new pointer would print `0` and go on reading back correctly.
          @first = second;
          show(first.id);
          show(second.id);
          show(destructed);

          // Through a call and back, so that the handle that gets assigned is the one the callee
          // returned rather than the one the caller passed. `@player.vehicle = GetVehicle(id);` is
          // this shape, and it is here for that rather than for its reference arithmetic.
          Entity@ passed = Pass(second);
          show(passed.id);

          // The counted assignment with the destination popped off the stack rather than named by the
          // instruction: a member of a value object on the stack, which is where a menu keeps its
          // state.
          Holder holder;
          @holder.entity = second;
          show(holder.entity.id);
        }

        // Nothing else in the function, so this is the whole of the release path: the locals and the
        // holder went out of scope together and whatever they held went with them.
        show(destructed);

        // Read by the test rather than asserted in it, because the exact count depends on how the
        // engine moves a returned handle around - and that is not what this test is about. That the
        // count is not zero is: a release path that never ran is the vacuous case, and two arms that
        // both destroyed nothing would agree about it perfectly.
        show(destructed > 0);
      }
    )AS");

    ExpectJitMatchesInterpreterUsing(source, "counted handle assignment",
                                     {AngelScript::asBC_REFCPY, AngelScript::asBC_RefCpyV, AngelScript::asBC_FREE});

    // The non-vacuity guard, read back from the interpreted arm - which is the oracle the comparison
    // above uses, and therefore the one whose answer has to be checked for having been produced at
    // all. The identifiers are 7 and 9 so that a `1` in this output can only be a destruction count.
    const auto interpreted = RunScript(source, false);
    ASSERT_EQ(interpreted.execution_result, AngelScript::asEXECUTION_FINISHED)
        << interpreted.exception_string;
    ASSERT_FALSE(interpreted.output.empty());
    EXPECT_EQ(interpreted.output.substr(interpreted.output.size() - 5), "true\n")
        << "the interpreter destroyed nothing, so the comparison above is not exercising the "
           "release path. It printed:\n"
        << interpreted.output;
  }

  TEST(Differential, ValueObjectCopies) {
    // `asBC_COPY`, which is the only translation that moves a block of memory rather than a value of
    // a known width, and for a large enough one the only translation that uses the string
    // instructions - and therefore the only one that makes the prologue's conditional save of rdi and
    // rsi do anything. A function that does not contain this opcode leaves those two alone; one that
    // does has to give them back exactly as it found them, which nothing can observe until the
    // function is entered a second time and one of them is carrying a stale value - so the loop below
    // runs the copying code repeatedly, and the reads after it are where a clobbered register would
    // show up.
    //
    // `Vec3` is a *registered* type rather than one declared here, and that is not a style choice:
    // a class declared in a script is given an auto-generated copy operator by the compiler, so
    // assigning one calls that instead and never emits this instruction. Only a host type registered
    // asOBJ_POD with no copy operator of its own produces it, which is also exactly how the menu's
    // `Vector3` is registered.
    //
    // `Block` is here for the other half of the translation. The count is a compile-time constant, so
    // a small copy is unrolled into straight-line moves and a large one is handed to `rep movsb`; a
    // test that only ever copied twelve bytes would leave the second of those never executed. Its two
    // reachable members sit at opposite ends of a hundred and twenty-eight byte object, so a copy
    // that moved the wrong number of bytes fails on one end or the other rather than passing because
    // the bytes it did move happened to be the ones being read.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      Vec3 Offset(Vec3 v, float delta) {
        v.x += delta;
        v.y += delta;
        v.z += delta;
        return v;
      }

      float Mix(Vec3 left, Vec3 right) {
        return left.x * right.x + left.y * right.y + left.z * right.z;
      }

      void main() {
        Vec3 a(1.0f, 2.0f, 3.0f);
        Vec3 b(0.5f, 0.25f, 0.125f);

        for (int i = 0; i < 5; i++) {
          Vec3 sum = a;                 // the copy the instruction exists for
          sum.x = sum.x + b.x;
          sum.y = sum.y + b.y;
          sum.z = sum.z + b.z;

          Vec3 shifted = Offset(sum, 1.0f);   // copies in and out through by-value parameters

          show(shifted.x);
          show(shifted.y);
          show(shifted.z);
          show(Mix(shifted, b));

          a = sum;                      // assignment rather than initialization
          b.x = b.x + 0.5f;
        }

        show(a.x);
        show(a.z);
        show(b.x);
        show(a.GetX());

        // The large copy, and the source is written *after* it: a translation that had aliased the
        // two objects rather than copying between them would agree with the interpreter on the second
        // read and only the first would catch it.
        Block block;
        block.head = 1.5f;
        block.tail = 2.5f;

        Block copy = block;
        block.head = 9.0f;
        block.tail = 9.0f;

        show(copy.head);
        show(copy.tail);
        show(block.head);

        Block assigned;
        assigned = copy;
        copy.tail = 4.5f;

        show(assigned.head);
        show(assigned.tail);
        show(copy.tail);
      }
    )AS"), "POD value object copies",
        {AngelScript::asBC_COPY});
  }

  // ---------------------------------------------------------------------------
  // Constructs the tests above do not compile
  // ---------------------------------------------------------------------------
  //
  // Each of these is a shape the benchmark has a workload for and the tests did not cover, which is
  // the wrong way round: a benchmark row records how fast a script ran, not whether it ran
  // *correctly*, and the one thing it checks - that neither arm raised - is exactly what a wrong
  // translation that stayed in range would pass. So a construct worth timing is a construct worth
  // comparing first, and these are the comparisons for the workloads added alongside them.

  TEST(Differential, EqualityAgainstZeroUsesTZAndTNZ) {
    // The two truth tests the comparisons above do not reach. TS/TNS/TP/TNP answer "which way did the
    // comparison go", and TZ/TNZ answer "equal or not" - against a *stored* result, because a
    // comparison consumed by a branch is folded into a jump and never produces either.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int Identity(int value) { return value; }

      void main() {
        for (int i = -2; i <= 2; i++) {
          int v = Identity(i);
          bool equal = v == 0;
          show(equal);
          bool notEqual = v != 0;
          show(notEqual);
        }

        // Floats take the same two tests after a float comparison, so the row above does not
        // establish them on their own.
        float f = 0.5f;
        for (int i = 0; i < 3; i++) {
          bool zero = f == 0.0f;
          show(zero);
          bool nonZero = f != 0.0f;
          show(nonZero);
          f = f - 0.25f;
        }
      }
    )AS"), "equality against zero",
        {AngelScript::asBC_TZ, AngelScript::asBC_TNZ});
  }

  TEST(Differential, BooleanOperatorsOnStoredFlags) {
    // `&&`, `||` and `!` over values the script holds rather than over expressions written inline.
    // Written inline the compiler sees a run of comparisons and emits one branch each; the operands
    // have to be stored for the short-circuit jumps and the low-byte tests to appear at all, which is
    // the shape a script has whenever it builds a condition and uses it later.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void main() {
        for (int i = 0; i < 8; i++) {
          bool a = (i & 1) == 0;
          bool b = (i & 2) == 0;
          bool c = (i & 4) == 0;

          show(a && b && c);
          show(a || b || c);
          show(!a && (b || !c));
          show(!(a || (b && c)));
          show(!a);
          show(!a || !b);
        }
      }
    )AS"), "boolean operators on stored flags",
        {AngelScript::asBC_NOT, AngelScript::asBC_JLowZ, AngelScript::asBC_JLowNZ});
  }

  TEST(Differential, FunctionPointersThroughAFuncdef) {
    // `asBC_FuncPtr` is the address of a function taken as a value, which is what `@Doubled` is and
    // what nothing else in this file produces - every other call names its callee. The call through
    // the pointer is `asBC_CallPtr`, and it is named here because it is translated: it used to be
    // declined, and the note this test carried then said so.
    //
    // The two halves are worth distinguishing. `asBC_CallPtr`'s target is a *value* - the pointer the
    // script last stored - so the translator cannot know at compile time which kind of callable it
    // will find. What it does know is how to reach the same three engine routines the interpreter's
    // handler reaches, once the pointer's kind is read; the kinds it cannot dispatch are handed back
    // untouched, and the null one is exercised by the test below.
    //
    // The loop is what makes this worth running rather than reading: it reassigns the pointer every
    // iteration, so a translation that cached the first callee - which the shape of the bytecode
    // invites, since the variable's frame slot is a constant - would print the same answer six times
    // and agree with nothing.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      funcdef int Modifier(int);

      int Doubled(int v) { return v * 2; }
      int Tripled(int v) { return v * 3; }

      void main() {
        Modifier@ apply = @Doubled;
        show(apply(4));

        @apply = @Tripled;
        show(apply(4));

        for (int i = 0; i < 6; i++) {
          if ((i & 1) == 0) {
            @apply = @Doubled;
          } else {
            @apply = @Tripled;
          }
          show(apply(i));
        }

        // A pointer that was never assigned is null, and calling through it raises. Not called here:
        // the raising path is the test below.
        Modifier@ unused;
        show(unused is null);
      }
    )AS"), "function pointers through a funcdef",
        {AngelScript::asBC_FuncPtr, AngelScript::asBC_CallPtr});
  }

  TEST(Differential, DelegateCallsThroughAFuncdef) {
    // The other kind of callable an `asBC_CallPtr` can find: a method bound to an object, which is
    // what `Modifier(a.Apply)` produces - the funcdef's own name applied to a method reference, which
    // is the language's only spelling for a delegate and the reason the funcdef has to be named here.
    // It is a separate *kind* inside the engine's handler, with its own stack handling - the receiver
    // has to be pushed before the target is entered, and the target is then entered through the
    // interface route rather than the direct one - so a translation that handled only script
    // functions would disagree here and nowhere else.
    //
    // The methods are `int`-returning and take one parameter, which is the shape least likely to
    // hide a mistake in the argument block: the receiver occupies a slot ahead of the argument, and
    // an off-by-one-pointer-slot error in the push would show up as the wrong multiplier.
    //
    // Rebinding the same variable to a second object is the part that makes the receiver the subject
    // of the test rather than a constant: an implementation that carried the object over from the
    // previous binding, or that read it from a fixed place, would answer the first three calls right
    // and the last one wrong.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      funcdef int Modifier(int);

      class Scale {
        int factor;
        Scale(int f) { factor = f; }
        int Apply(int v) { return v * factor; }
      }

      void main() {
        Scale a(3);
        Scale b(7);

        Modifier@ apply = Modifier(a.Apply);
        show(apply(5));
        show(apply(11));

        @apply = Modifier(b.Apply);
        show(apply(5));

        // Both objects alive at once and both bound, so the receiver has to come from the pointer
        // rather than from whichever object happened to be constructed last.
        Scale@ r = @a;
        @apply = Modifier(r.Apply);
        show(apply(9));
      }
    )AS"), "delegates through a funcdef",
        {AngelScript::asBC_FuncPtr, AngelScript::asBC_CallPtr});
  }

  TEST(Differential, ReturnsAgreeWithTheInterpreter) {
    // `asBC_RET`, which is the one instruction in the set that was translated, verified, and then taken
    // back out on measurement. The `asBC_RET` case in `function_compiler.cpp` is the record: a version
    // that popped the frame with the engine's own `PopCallState` agreed with the interpreter on every
    // test in this suite, and made the benchmark's call-heavy rows twenty to forty-five percent slower,
    // because deciding *whether* to pop needs a virtual call that serialises two pointers and the
    // interpreter reads the same element for a load and a compare.
    //
    // So the two things worth asserting here are that the refusal is deliberate - the opcode is refused
    // rather than silently missing - and that the returns themselves are still exercised, because a
    // block that ends at a return is a shape the rest of the translator has to survive: the caller
    // resumes from an entry point with the callee's frame gone.
    //
    // The subject of the comparison is what a return restores, which is the caller's five saved
    // registers - its program counter above all, since that is where the caller resumes. Every call
    // here has a different argument shape: none, three `int`s, one `double`, and a recursive call whose
    // return comes from a different depth each time, so the recursion is the second subject. A return
    // from four frames down has to restore *that* frame's caller rather than the outermost one, and
    // every frame here is the same size - an implementation that popped to a fixed place, or that
    // popped one frame and stopped, would agree with the interpreter at depth one and disagree from two
    // down. This is not hypothetical: the version that was measured popped the frame twice, and this
    // script caught it - the sums came out 7 and 14 instead of 244 and 488, which is a wrong answer
    // rather than a fault, and is the harder failure to see.
    //
    // What this test does *not* establish is the argument drop the handler ends with, and that is a
    // measurement rather than an omission. Zeroing the emitted word count, so a callee's arguments are
    // never dropped, leaves this test and every other one in this suite green. The reason is that a
    // script function's locals and temporaries are all addressed relative to the frame pointer, and the
    // frame pointer is restored from the call state rather than from the stack pointer; a stale stack
    // pointer therefore only moves where the *next* argument push lands, and the callee reads its
    // arguments relative to the frame it was given, so the two agree wherever the stack pointer happens
    // to be. Nothing in a script can see it, which is why the translation is not worth having for this
    // instruction even though it can be made to agree.
    const std::string script = Source(R"AS(
      int Leaf() { return 7; }

      int Sum(int a, int b, int c) { return a * 100 + b * 10 + c; }

      double Half(double x) { return x / 2.0; }

      int Depth(int n) {
        if (n <= 0) return 100;
        return Depth(n - 1) + n;
      }

      int Caller() {
        int total = Leaf() + Sum(1, 2, 3);
        total += int(Half(9.0));
        total += Depth(4);
        return total;
      }

      void main() {
        show(Caller());
        show(Caller() * 2);
      }
    )AS");
    ExpectJitMatchesInterpreter(script, "returns");

    // And the refusal, asserted so that removing it - or restoring the translation - cannot happen
    // without this test changing. `Unimplemented` rather than `Walked - Translated` for the reason its
    // own comment gives: only the first distinguishes a gap in the translator from an occurrence the
    // translator was never handed.
    const auto compiled = RunScriptWithConfig(script, EngineConfig{.use_jit = true});
    ASSERT_GT(compiled.Walked(AngelScript::asBC_RET), 0u)
        << "the script did not compile to a function that returns, so this test proves nothing";
    EXPECT_EQ(compiled.Translated(AngelScript::asBC_RET), 0u);
    EXPECT_GT(compiled.Unimplemented(AngelScript::asBC_RET), 0u)
        << "the return was neither translated nor reported as refused";

    // The other half of the claim, and the half the paragraphs above would be worth nothing without.
    // The same script, the same comparison, on the configuration that translates the return: the
    // instruction is refused by default because of what it costs, so the thing that has to be true is
    // that it is not refused because of what it does. Everything the comment above describes as the
    // subject - the four argument shapes, the recursion four frames deep, an implementation that pops
    // one frame too many - is only tested at all on this configuration.
    EngineConfig translating;
    translating.translate_returns = true;
    ExpectJitMatchesInterpreterUsing(script, "returns, with the return translated", {AngelScript::asBC_RET},
                                     translating);

    // And the other direction of the same guard: the flag has to change what the translator does.
    // Without this a flag that was accepted and then ignored would leave every assertion here true
    // and the configuration untested.
    const auto with_returns = RunScriptWithConfig(script, EngineConfig{.use_jit = true, .translate_returns = true});
    EXPECT_GT(with_returns.Translated(AngelScript::asBC_RET), 0u)
        << "the option was set and the return was still handed back";
    EXPECT_EQ(with_returns.Unimplemented(AngelScript::asBC_RET), 0u)
        << "the return was translated and also reported as refused";
  }

  TEST(Differential, ReferenceCastAgreesWithTheInterpreter) {
    // `asBC_Cast`, which the translator reaches through four published accessors where the interpreter
    // reads four fields - the object's type, the target type, the interface and inheritance tests, and
    // the reference count.
    //
    // Three of the four are pure questions and one of them is not: the success path takes a reference
    // on the object, and unlike every other instruction in this group the *reference* is the part a
    // wrong translation can be seen making. The observable is a leak, and the script below is built to
    // show one: every object counts itself live on construction and dead on destruction, the count is
    // printed after the scopes that held them have closed, and a translation that took one reference
    // too many leaves a nonzero count where the interpreter prints zero.
    //
    // The other direction is a fault rather than a wrong number - a reference not taken means the
    // object is freed while a handle still points at it, which is a crash or a corrupted member on the
    // next line - and it needs no arrangement beyond the use after the cast, which is what the reads
    // through the cast handles are.
    //
    // What the script covers of the instruction's own branches: a cast that succeeds against the
    // object's *dynamic* type, one that fails against a sibling, one that matches an interface, one
    // that matches a base class, and one whose operand is a null handle. The last is the branch the
    // interpreter guards with `a && *a`, and it is a null *result* rather than an error in every case.
    const std::string source = Source(R"AS(
      int live = 0;

      interface IDraw {
        int Draw();
      }

      class Shape {
        int kind;
        Shape() { live++; kind = 0; }
        ~Shape() { live--; }
      }

      class Circle : Shape {
        int radius;
        Circle(int r) { live++; radius = r; kind = 1; }
        ~Circle() { live--; }
      }

      class Square : Shape {
        int side;
        Square(int s) { live++; side = s; kind = 2; }
        ~Square() { live--; }
      }

      class Sprite : Shape, IDraw {
        int width;
        Sprite(int w) { live++; width = w; kind = 3; }
        ~Sprite() { live--; }
        int Draw() { return width; }
      }

      // Reading through a cast handle is what makes a missed reference visible: the object the handle
      // points at has to still be there when the read happens, on every path.
      int Classify(Shape@ s) {
        Circle@ c = cast<Circle>(s);
        if (c !is null)
          return c.radius;
        Square@ q = cast<Square>(s);
        if (q !is null)
          return q.side;
        Sprite@ p = cast<Sprite>(s);
        if (p !is null)
          return p.width + p.Draw();
        Shape@ b = cast<Shape>(s);
        if (b !is null)
          return b.kind;
        return -100;
      }

      void main() {
        show(live);

        for (int i = 0; i < 4; i++) {
          Circle@ c = Circle(5 + i);
          Shape@ s = c;
          show(Classify(s));

          Square@ q = Square(9 + i);
          Shape@ s2 = q;
          show(Classify(s2));

          Sprite@ p = Sprite(3 + i);
          Shape@ s3 = p;
          show(Classify(s3));

          // The interface cast, whose target is an interface rather than a class.
          IDraw@ d = cast<IDraw>(s3);
          show(d !is null ? d.Draw() : -1);

          // A sibling-class cast that does not match, and the same cast on a null handle.
          Circle@ wrong = cast<Circle>(s2);
          show(wrong is null);

          Shape@ nothing = null;
          Circle@ none = cast<Circle>(nothing);
          show(none is null);
        }

        // The scopes above have closed, so every object constructed in them has been destroyed. A
        // count that is not back to where it started is a reference the cast took and nothing gave
        // back - which is the one failure of this instruction that produces a number rather than a
        // fault.
        show(live);
      }
    )AS");

    ExpectJitMatchesInterpreterUsing(source, "reference cast", {AngelScript::asBC_Cast});

    const auto compiled = RunScriptWithConfig(source, EngineConfig{.use_jit = true});
    EXPECT_GT(compiled.Translated(AngelScript::asBC_Cast), 0u)
        << "the cast was walked but never offered to the translator, so this test says nothing "
           "about the translation";
  }

  TEST(Differential, ScriptObjectConstructionIsTranslated) {
    // `asBC_ALLOC`'s script-object branch: allocate, pre-initialise, write the new object's address
    // into the argument slot the caller reserved, push it, and enter the constructor as a script
    // function.
    //
    // The three things that could go wrong are each something the script has to be able to see. The
    // argument slot is the one that matters most - a translation that skipped it would leave `this`
    // inside the constructor pointing at whatever the caller had there, which for a fresh frame is
    // null, so the constructor would fault rather than print a wrong number. The reference count is
    // the second, and it is why the objects are held in handles and released: an object whose count
    // was never initialised is reported as a crash at the end of the execution rather than as a value
    // here. The third is the exit, which is the call shape - after the constructor is entered the
    // block ends, and a translation that published its own registers over the new frame's would
    // resume the constructor in the wrong place.
    //
    // The constructors take arguments and touch a member, because a parameterless one that assigned a
    // constant would compile to almost nothing and would leave the argument slot unexercised.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      class Point {
        int x;
        int y;
        Point(int a, int b) { x = a; y = b; }
        int Sum() { return x + y; }
      }

      class Counter {
        int n;
        Counter() { n = 0; }
        Counter(int start) { n = start; }
        void Bump(int by) { n += by; }
        int Value() { return n; }
      }

      void main() {
        Point p(3, 4);
        show(p.Sum());

        for (int i = 0; i < 5; i++) {
          Point q(i, i * i);
          show(q.Sum());
        }

        // A handle rather than a value: the object lives on the heap, so this is the branch that
        // pushes the new address rather than one that could have been folded into the frame.
        Point@ h = Point(10, 20);
        show(h.Sum());
        @h = Point(1, 2);
        show(h.Sum());

        Counter c;
        for (int i = 0; i < 4; i++) {
          c.Bump(i);
          show(c.Value());
        }

        Counter@ d = Counter(100);
        d.Bump(5);
        show(d.Value());
      }
    )AS"), "script object construction",
        {AngelScript::asBC_ALLOC});
  }

  TEST(Differential, NullFunctionPointerRaisesLikeTheInterpreter) {
    ExpectJitRaisesLikeInterpreter(Source(R"AS(
      funcdef int Modifier(int);

      int Doubled(int v) { return v * 2; }

      void main() {
        Modifier@ apply = @Doubled;
        show(apply(1));
        @apply = null;
        show(apply(2));
      }
    )AS"), "null function pointer raises");
  }

  TEST(Differential, TryCatchAgreesWithTheInterpreter) {
    // A `try` the compiled code runs straight through. The boundary is not an instruction: the
    // compiler allocates it zero-sized and Output drops it, so what reaches the translator is the try
    // body as ordinary code and the catch handler as ordinary code after it. The VM's exception frame
    // for the range arrives through scriptData->tryCatchInfo, which the interpreter consults and the
    // compiler here never sees.
    //
    // That is the shape worth comparing, and the reason is not the speed: the compiled code covers
    // bytecode positions that the interpreter has an exception frame around, and the two have to
    // agree about which positions those are. A frame that was one instruction out at either end would
    // put the boundary inside the loop rather than around its body, which raises nothing and prints a
    // different number.
    ExpectJitMatchesInterpreter(Source(R"AS(
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

      void main() { show(Work(64)); }
    )AS"), "try block the compiled code hands off");
  }

  TEST(Differential, TryCatchCatchingAnExceptionFromANative) {
    // The other half: a catch that runs, and the shape a menu script takes when a native refuses. The
    // exception is raised by the array add-on's bounds check, so it starts inside a registered method,
    // travels out through the caller's frame, and is caught by a handler in a loop the compiler has
    // partly compiled.
    //
    // `throw` is not a language keyword in this SDK - it is a global function that
    // RegisterExceptionRoutines installs, and the menu does not register it, so a script there cannot
    // raise on its own and the engine is the only thing that can. That is why this test reaches for
    // the bounds check rather than writing `throw(...)`.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Pick(int index) {
        array<int> values = {10, 20, 30};
        return values[index];
      }

      void main() {
        int total = 0;
        for (int i = 0; i <= 6; i++) {
          try {
            total += Pick(i);
          } catch {
            total -= 1;
          }
        }
        show(total);
      }
    )AS"), "catch around an exception from a native");
  }

  TEST(Differential, TryCatchAroundAFaultInsideCompiledCode) {
    // A raise from inside the compiled region, which is the one path where the two engines have to
    // agree about something neither of them owns: the null dereference is a translated instruction, the
    // JIT does not raise, it hands the instruction back - and the frame that catches the exception the
    // interpreter then raises is the one the *interpreter* has around that bytecode position.
    //
    // So this fails if the compiler's idea of where the try's range ends differs from the
    // interpreter's. The arithmetic is arranged so that a handler which never fires, or fires on the
    // wrong iterations, prints a number rather than raising: two of four iterations dereference a null,
    // and each one caught is worth 100.
    ExpectJitMatchesInterpreter(Source(R"AS(
      class Box {
        int value;
      }

      void main() {
        Box@ good = Box();
        good.value = 7;

        int total = 0;
        for (int i = 1; i <= 4; i++) {
          try {
            if ((i & 1) == 0) {
              total += good.value;
            } else {
              Box@ bad = null;
              total += bad.value;
            }
          } catch {
            total += 100;
          }
        }
        show(total);
      }
    )AS"), "catch around a fault inside compiled code");
  }

  TEST(Differential, DictionaryLookupsAgree) {
    // The dictionary add-on, which is the third container the menu registers and the one the earlier
    // tests never touched. Every operation on it is a registered method, so what is compiled here is
    // the marshalling around the calls.
    //
    // The out parameter is a `string` on purpose. `get(const string&, int64&out)` is a dedicated
    // overload for integers and needs no type tag, so an `int` out-parameter compiles to a call
    // without one - the first version of this test used `int` and its assertion on asBC_TYPEID failed
    // for that reason. A `string` has no dedicated overload and binds to `get(const string&, ?&out)`
    // instead, which is the form that has to carry the type tag, so this is the spelling that reaches
    // asBC_TYPEID at all.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void main() {
        dictionary values;
        values.set("alpha", "one");
        values.set("beta", "two");

        show(values.exists("alpha"));
        show(values.exists("gamma"));

        string value = "";
        show(values.get("beta", value));
        show(value);

        show(values.get("gamma", value));
        show(value);

        values.set("gamma", "three");
        show(values.get("gamma", value));
        show(value);

        show(values.getSize());
        values.deleteAll();
        show(values.exists("alpha"));
        show(values.getSize());
      }
    )AS"), "dictionary lookups", {AngelScript::asBC_TYPEID});
  }

  TEST(Differential, DoWhileWithBreakAndContinue) {
    // A loop whose test is at the bottom, so its entry edge and its back edge are the same edge, with
    // `break` and `continue` inside it. Every other loop in this file is a `for`, and the three
    // statements are the only ones that leave a loop body at a point that is not its end.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Sum(int limit) {
        int total = 0;
        int i = 0;
        do {
          i++;
          if ((i & 3) == 0) {
            continue;
          }
          total += i;
        } while (i < limit);
        return total;
      }

      int Counted(int limit) {
        int total = 0;
        int i = 0;
        while (true) {
          i++;
          if (i > limit) {
            break;
          }
          if ((i & 1) == 0) {
            continue;
          }
          total += i;
        }
        return total;
      }

      void main() {
        // Zero is the case a bottom-tested loop gets wrong: the body has to run once even though the
        // condition is false from the start, which a translation that treated the test as an entry
        // test would skip entirely.
        show(Sum(0));
        show(Sum(1));
        show(Sum(9));
        show(Counted(1));
        show(Counted(10));
      }
    )AS"), "do-while with break and continue");
  }

  TEST(Differential, ArrayLiteralConstruction) {
    // `array<int> values = {1, 2, 3};` - the list initialiser, which compiles to the list operations
    // the add-on exposes rather than to a loop of insertLast. None of them is translated, so this is
    // a comparison of the hand-off around a construction rather than of the construction; what it
    // would catch is a resume point that ran the initialiser twice, which would quietly build a
    // longer array than the script asked for.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Sum(const array<int> &in values) {
        int total = 0;
        for (uint i = 0; i < values.length(); i++) {
          total += values[i];
        }
        return total;
      }

      void main() {
        array<int> values = {1, 2, 3, 4, 5, 6, 7, 8};
        show(values.length());
        show(Sum(values));

        // Empty and single-element initialisers, which take different paths through the add-on.
        array<int> empty = {};
        show(empty.length());

        array<int> one = {42};
        show(one.length());
        show(Sum(one));
      }
    )AS"), "array literal construction");
  }

  TEST(Differential, StringComparisonAndRecursionAgree) {
    // Two shapes with nothing in common except that each is a thing a script does that nothing above
    // compiles: comparing strings rather than building them, and a function that re-enters itself.
    //
    // Recursion is here for the one thing that is particular to it - the same compiled function being
    // live more than once at a time. The JIT's frame is the interpreter's own stack slot, so a
    // recursion bug would not be a wrong answer in the leaf, it would be a corrupt return address
    // several frames up, surfacing as whatever the unwinding happened to hit.
    ExpectJitMatchesInterpreter(Source(R"AS(
      int Descend(int depth, int seed) {
        if (depth <= 0) {
          return seed;
        }
        return seed + Descend(depth - 1, (seed * 3 + depth) & 1023);
      }

      void main() {
        show(Descend(0, 5));
        show(Descend(6, 0));
        for (int i = 0; i < 4; i++) {
          show(Descend(i, i));
        }

        string candidate = "adder";
        string other = "adder";
        string third = "banshee";

        show(candidate == other);
        show(third != candidate);
        show(candidate < third);
        show(candidate > third);
        show(candidate <= other);
        show(candidate >= candidate);
        show(candidate == third);
      }
    )AS"), "string comparison and recursion");
  }

  TEST(Differential, NullOperandsAreTranslated) {
    // `asBC_PshNull` and `asBC_ClrVPtr` are one optimizer pass away from not existing: either the
    // peephole passes fold a null handle away, or they rewrite the pair into something else, and an
    // optimizing engine - which is the default, and what the menu uses - hands neither to the
    // translator. So this runs unoptimized, which is the configuration the note above the comparison
    // helpers describes and the same reason the ChkRefS tests choose it.
    //
    // What the two have to agree on is that a handle is one machine word. PshNull pushes a null word
    // and ClrVPtr is that word becoming the value of a handle-shaped variable, so a translation that
    // widened either - a pointer-sized store into a word-sized slot, say - would write past the
    // variable. The handles are read back afterwards so that the damage would be visible rather than
    // merely present.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      Handle@ g_slot;

      void main() {
        int seed = 1;
        Handle@ a = Handle(1);
        Handle@ b = null;

        Handle@ chosen = (seed > 0) ? a : b;
        show(chosen.value);

        @g_slot = (seed > 0) ? b : a;
        show(g_slot is null);

        Handle@ copy = b;
        show(copy is null);
        show(a is null);

        @g_slot = null;
        show(g_slot is null);
        show(a.value);
      }
    )AS"), "null operands", {AngelScript::asBC_PshNull, AngelScript::asBC_ClrVPtr},
        EngineConfig{.optimize_bytecode = false});
  }

  TEST(Differential, BooleanNormalizationIsTranslated) {
    // A comparison produces a bool in a 32-bit register and nothing guarantees the bits above the low
    // one are clear; `asBC_ClrHi` is what clears them before the value is stored into a bool. Its
    // absence is invisible in a branch, because a branch tests the low bit - it shows up when the bool
    // is *carried*, which is why every statement below stores into a bool variable and then reads it
    // back as a number rather than testing it.
    //
    // Unoptimized for the same reason as the nullable operands above: with the peephole passes on,
    // most of these comparisons are folded and no ClrHi is emitted at all.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      bool Flag(int value) { return value != 0; }

      void main() {
        int x = 5;

        bool greater = x > 0;
        bool less = x < 10;
        bool both = (x > 0) && (x < 10);
        bool either = (x > 0) || (x > 100);

        show(greater ? 1 : 0);
        show(less ? 1 : 0);
        show(both ? 1 : 0);
        show(either ? 1 : 0);

        bool negative = x < 0;
        show(negative ? 1 : 0);

        bool fromInt = Flag(-1);
        show(fromInt ? 1 : 0);
        show(Flag(0) ? 1 : 0);
      }
    )AS"), "boolean normalization", {AngelScript::asBC_ClrHi}, EngineConfig{.optimize_bytecode = false});
  }

  TEST(Differential, ValueConversionExchangeIsTranslated) {
    // `asBC_SwapPtr`, the last opcode the emitter had a case for that nothing in this suite produced.
    //
    // It exists because a call can be handed two addresses that arrive in the wrong order: a conversion
    // operator producing a value type too large for the registers writes through a buffer the caller
    // supplies, and that buffer's address is pushed *after* the address of the object the operator is
    // being called on - so the two have to be exchanged before the call, because the callee reads them
    // positionally. The direction matters: swapping them the other way hands the callee the result
    // buffer as its object and the object as the buffer, which writes a `Vec3` over a `Handle` - and
    // the `Handle` here is a static the harness keeps alive precisely so that damage would be visible.
    //
    // Unoptimized for the same reason the two tests above it are: the peephole removes a `SwapPtr` whose
    // two predecessors below it are `PSF`/`PshVPtr`/`PshNull` (as_bytecode.cpp:305), and this shape has
    // exactly that pair beneath it, so an optimizing engine exchanges them at compile time and emits
    // nothing. The test was written with the optimizer on first and reported the opcode was never
    // compiled, which is the measurement the harness comment for `HandleToVec3` records.
    //
    // The comment on `HandleToVec3` in the harness is the other half of this: it is the registration
    // that makes the shape reachable at all, and it exists for this test.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      Vec3 Convert(const Handle &in h) {
        Vec3 v = h;
        return v;
      }

      void main() {
        Handle@ one = Handle(1);
        Handle@ two = Handle(2);

        Vec3 a = Convert(one);
        Vec3 b = Convert(two);

        show(int(a.x));
        show(int(a.y));
        show(int(b.x));

        // The handles again, which is what a swapped pair would have written over.
        show(one.value);
        show(two.value);
      }
    )AS"), "value conversion exchange", {AngelScript::asBC_SwapPtr},
        EngineConfig{.optimize_bytecode = false});
  }

  TEST(Differential, ReturningAnObjectByReferenceIsTranslated) {
    // The third way an object crosses a function boundary, and the one this suite had not covered: not
    // by value and not by handle, but by reference. The compiler pushes the returned pointer with
    // `asBC_PshRPtr` (as_compiler.cpp:17543) on a path that neither of the other two reach, so a
    // translation of it with no test is a translation nothing would catch being wrong.
    //
    // Both a native and a script function return one, because the instruction is emitted by the shared
    // call code but the two produce the pointer from different places - the native returns it, the
    // script function's frame has it. Reading a component through the reference is what makes the
    // difference: a translation that pushed an uninitialised register would print a zero, and one that
    // pushed the wrong address would print a different number, so the script moves the value between
    // reads.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      Vec3 g_vec;

      Vec3& Forward() { return g_vec; }

      void main() {
        show(GetGlobalVec().GetX());
        g_vec.x = 7.0f;
        show(GetGlobalVec().GetX());
        show(Forward().GetX());
        g_vec.x = -2.5f;
        show(Forward().GetX());
      }
    )AS"), "object returned by reference", {AngelScript::asBC_PshRPtr});
  }

  TEST(Differential, GlobalCompoundAssignmentIsTranslated) {
    // `asBC_LdGRdR4` is the load half of a compound assignment to a global: a read-modify-write where
    // the address is computed rather than held in a variable. It is an optimizer product - the passes
    // rewrite the explicit address arithmetic into the instruction - and it is the only one of the
    // four instructions added by this group that needs the optimizer *on*, which is why this test
    // takes the default configuration rather than the unoptimized one its neighbours use.
    //
    // Both widths, because the `4` in the name is the width of the value read and a translation that
    // read a different number of bytes would be right for one of these and wrong for the other.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      int g_count = 1;
      float g_scale = 1.0f;

      void main() {
        g_count += 5;
        g_count *= 2;
        g_count -= 3;
        g_scale *= 3.0f;
        g_scale += 0.5f;
        g_scale /= 2.0f;
        show(g_count);
        show(g_scale);
      }
    )AS"), "global compound assignment", {AngelScript::asBC_LdGRdR4});
  }

  TEST(Differential, ListHeaderStoresAreTranslated) {
    // The two instructions that write the initialisation-list buffer's header, named together because
    // they are one case in the emitter and their handlers are the same three statements with different
    // field names on them (as_context.cpp:4622 and :4651).
    //
    // Two literals rather than one, because the two instructions are emitted under different
    // conditions and only one of them is in both. `SetListSize` carries the element count and goes
    // into every list pattern; `SetListType` records a *runtime* element type and is emitted only for
    // the `?` element of a pattern whose type is deduced from the value - which is what a `dictionary`
    // literal is (`{repeat {string, ?}}`), and what a typed `array` literal is not. So an `array<int>`
    // literal alone would have named an instruction it never produced.
    //
    // The elements are compared one by one rather than summed, so a header written with the wrong
    // offset - a count landing where the type id belongs, a type id landing over an element - shows up
    // as a wrong value rather than as a plausible total.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void main() {
        array<int> values = {11, 22, 33, 44, 55};
        show(values.length());
        for (uint i = 0; i < values.length(); i++) {
          show(values[i]);
        }

        array<double> reals = {1.5, 2.5, 3.5};
        show(reals.length());
        for (uint i = 0; i < reals.length(); i++) {
          show(reals[i]);
        }

        dictionary counts = {{"one", 1}, {"two", 2}, {"three", 3}};
        show(counts.getSize());
        show(int(counts["one"]));
        show(int(counts["two"]));
        show(int(counts["three"]));
      }
    )AS"), "list header stores",
      {AngelScript::asBC_SetListSize, AngelScript::asBC_SetListType});
  }

  TEST(Differential, ListElementAddressPushIsTranslated) {
    // The address of one element, pushed for the element's own assignment to write through. What makes
    // it worth its own test is that it is the one of the four list instructions whose *offset* is not
    // a header field: `off` here walks to the element, so a translation that dropped the addition
    // would hand every element the same address and the literal would come out as a repeat of its
    // first element - a failure that is invisible in the length and visible only in the contents.
    //
    // An array of a non-primitive type, because that is what the compiler emits this for: a primitive
    // element is stored by `WRTV4`/`SetV4`, and the copy path this exercises - `COPY` through the
    // address this instruction pushed - is the object one. `array` of a script class needs the class
    // registered with a factory, so the shape is a literal of a registered value type instead: `Vec3`
    // is one, and it is already registered by the harness.
    //
    // Read through the object's *properties* rather than a method, because a method call would end the
    // block before the `COPY` and the copy is the half this test exists to check.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void main() {
        array<Vec3> path = {Vec3(1.0f, 2.0f, 3.0f), Vec3(4.0f, 5.0f, 6.0f), Vec3(7.0f, 8.0f, 9.0f)};
        show(path.length());
        for (uint i = 0; i < path.length(); i++) {
          show(path[i].x);
          show(path[i].y);
          show(path[i].z);
        }
      }
    )AS"), "list element address", {AngelScript::asBC_PshListElmnt});
  }

  TEST(Differential, HeapValueTypeConstructionIsTranslated) {
    // `asBC_ALLOC`'s value-type branch, which is reached by a shape that looks nothing like a heap
    // allocation: **a value type passed by value is built on the heap at the call site**, so
    // `Take(Vec3(1.0f, 2.0f, 3.0f))` allocates, constructs, and writes the new address into the
    // argument slot the callee will read its parameter from.
    //
    // This branch spent three sessions recorded as unreachable, on the reasoning that the only
    // variables `IsVariableOnHeap` puts on the heap are module globals, reference members and factory
    // stubs, none of which is a function the JIT is offered. The reasoning was about the wrong
    // question. The globals below do produce no `ALLOC` the compiler ever sees - that half was right -
    // but a by-value argument is a fourth case, and it is an ordinary script shape: two calls forward
    // a value type through two allocations.
    //
    // The three-part body is deliberate. The first call is the plain case; the second is the same
    // instruction reached from inside a callee, which is where a translation that only got the
    // outermost frame right would show up; and the readings through `.x`, `.y`, `.z` are *properties*
    // rather than a method, because `GetX()` is a `asBC_Thiscall1` call site and a call would end the
    // block before the `ALLOC` this test names.
    ExpectJitMatchesInterpreterUsing(Source(R"AS(
      void Take(Vec3 v) { show(v.x); show(v.y); show(v.z); }
      void Forward(Vec3 v) { Take(v); }
      void main() {
        Take(Vec3(1.0f, 2.0f, 3.0f));
        Forward(Vec3(4.0f, 5.0f, 6.0f));
      }
    )AS"), "heap value type construction", {AngelScript::asBC_ALLOC});
  }

  TEST(Differential, AValueTypeConstructorThatRaisesFreesWhatItAllocated) {
    // The tail of the value-type branch, which is the half the branch was refused over: when the
    // constructor raises, the interpreter hands the fresh memory back to the allocator and nulls the
    // variable that was about to receive it, because an object that never finished being built must
    // not be left looking alive.
    //
    // `Fault(int)` raises for a negative argument, so the third statement is a construction that
    // fails while the first two succeed. Both arms must end with the same exception and the same
    // output, and the compiled arm must not go on to print `999`: a translation that skipped the tail
    // would carry on with a variable pointing at freed memory and would be caught here by the output
    // rather than by a crash, which is the only way a use-after-free is reliably visible.
    ExpectJitRaisesLikeInterpreterUsing(Source(R"AS(
      void Take(Fault f) { show(f.value); }
      void main() {
        Take(Fault(7));
        Take(Fault(8));
        Take(Fault(-1));
        show(999);
      }
    )AS"), "value type constructor that raises", {AngelScript::asBC_ALLOC});
  }

  TEST(Differential, AThiscall1ShapedMethodThatRaisesAgreesWithTheInterpreter) {
    // The instruction this component refuses by default, tested from the side that matters.
    // `Fault::Get(int)` has the one signature `as_compiler.cpp:17433` picks `asBC_Thiscall1` for - an
    // object method returning a reference and taking one four-byte integer - and it raises when its
    // argument is negative.
    //
    // That raise is the whole point of the test, and it is why the method is not a plain getter. A
    // native can only raise through `asIScriptContext::SetException`, and that call is gated on a
    // private field of the context that the interpreter's Thiscall1 handler sets before it dispatches
    // the call and that `asCScriptEngine::CallObjectMethodRetPtr` - the engine's own fast path, the one
    // the handler exists to use - does not set in any of its branches. So this is the behaviour a
    // translation that reached the fast path directly would lose, and it is not a subtlety that can be
    // argued about: with `asBC_Thiscall1` handed back to the interpreter, both arms raise and this test
    // passes; routed through the fast path, the compiled arm's `SetException` returns `asERROR` and
    // raises nothing, and the test fails on the exception string rather than on anything quieter.
    const std::string script = Source(R"AS(
      void main() {
        Fault f(10);
        show(f.Get(1));
        show(f.Get(-1));
        show(999);
      }
    )AS");
    ExpectJitRaisesLikeInterpreterReaching(script, "thiscall1-shaped method that raises",
                                           {AngelScript::asBC_Thiscall1});

    // And the same script on the configuration that translates it, which is where the paragraph above
    // stops being a claim about a refusal and becomes a test of a translation. The route it takes is
    // the general `CallSystemFunction`, which sets and clears the field itself - so the raise survives,
    // and the point of running it here is that this is the only place where that is checked rather
    // than argued. A translation that quietly reached the fast path instead would fail *this* assertion
    // and pass the one above, which is the pair that pins the choice.
    EngineConfig translating;
    translating.translate_object_method_calls = true;
    ExpectJitRaisesLikeInterpreterUsing(script, "thiscall1-shaped method that raises, with the call translated",
                                        {AngelScript::asBC_Thiscall1}, translating);

    const auto with_thiscall = RunScriptWithConfig(script, EngineConfig{.use_jit = true,
                                                                       .translate_object_method_calls = true});
    EXPECT_GT(with_thiscall.Translated(AngelScript::asBC_Thiscall1), 0u)
        << "the option was set and the method call was still handed back";
    EXPECT_EQ(with_thiscall.Unimplemented(AngelScript::asBC_Thiscall1), 0u);
  }

  TEST(Differential, AVariadicGenericNativeCalledFromACompiledLoopAgreesWithTheInterpreter) {
    // The generic calling convention, which is what the menu registers every `log::*` and `thread::*`
    // with, and which nothing in this component had put under a compiler before: the benchmark's note
    // says its stubs stand in for natives registered `asCALL_CDECL` or `asCALL_GENERIC`, and until
    // `VariadicTrace` existed every binding in both targets was the first.
    //
    // Variadic on top of generic, because the two are not separable here. `asCALL_CDECL` cannot be
    // variadic at all - the engine refuses the registration outright (as_scriptengine.cpp:2986) - so a
    // variadic binding is necessarily a generic one, and generic is therefore the only convention under
    // which the engine's variadic marshalling branch is reachable. That branch is not a detail of
    // argument passing: it builds an `asCGenericVariadic` instead of an `asCGeneric`
    // (as_context.cpp:5865) and derives the pop size from the count the call actually carried, so a
    // mis-translated `asBC_CALLSYS` for a variadic call leaves the stack pointer wrong by however many
    // arguments were passed - which is a wrong result here and an unwind through the caller's frame in
    // a script that raises.
    //
    // The call is in a loop rather than once, so that the arguments are re-marshalled per iteration on a
    // stack the compiled block is also using, and the script is shaped so that a dropped or misread
    // argument changes the printed total rather than only the timing.
    //
    // What this does *not* pin is the home space. The JIT's runtime call helper was missing the 32-byte
    // reservation the ABI requires at every call, and the failure needed a callee whose prologue
    // actually spills its register arguments there - which this ABI forces only for a C-variadic
    // function, and the JIT's direct callees are fixed-argument helpers. So the fix is documented in
    // `function_compiler.cpp` and demonstrated by an experiment rather than by this test, and saying so
    // is better than a comment that implies a coverage this suite does not have.
    constexpr const char* source = R"AS(
      int Work(int iterations) {
        int total = 0;
        for (int i = 0; i < iterations; i++)
          total += VariadicTrace(3, i, i + 1, i + 2);
        return total;
      }
      void main() { show(Work(12)); }
    )AS";

    // The one number the script produces if the count and the three values arrive in the slots the
    // declaration names, and zero if they do not. Every test in this file refuses to write an
    // expectation out by hand because the interpreter is the specification - but the interpreter cannot
    // be the specification for *this*, and the reason is worth stating: a binding that read the wrong
    // argument slots would read them the same wrong way in both arms, so the comparison above would
    // report agreement on a number neither engine should have produced. This is the one assertion here
    // that is about the binding rather than about the translator, and it is what stops the comparison
    // from being satisfiable by two identically broken runs.
    const auto interpreted = RunScript(Source(source), false);
    ASSERT_EQ(interpreted.execution_result, AngelScript::asEXECUTION_FINISHED)
        << "the script raised, so the variadic call was not marshalled: " << interpreted.exception_string;
    EXPECT_EQ(interpreted.output, "234\n")
        << "the variadic arguments did not arrive in the slots the declaration names, so the comparison "
           "below would be comparing two runs that were both wrong";

    ExpectJitMatchesInterpreter(Source(source), "a variadic generic native called from a compiled loop");
  }

} // namespace base::asjit::test
