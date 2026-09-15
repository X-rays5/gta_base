//
// Created by X-ray on 18/12/2025.
//

#include <gtest/gtest.h>

#include <filesystem>
#include <iostream>
#include <random>
#include <string>
#include <system_error>

namespace {
  /**
   * A scratch working directory for the run, removed with it.
   *
   * Everything base::common's vfs names is relative to the working directory - the game moves into one
   * under appdata as soon as it starts - and a test binary that is not given one writes a script's data,
   * its settings and its translations wherever it was launched from. Launched from the repository root,
   * which is what running the exe by hand is, that is the repository root: three directories of the last
   * run's leavings sitting in `git status` beside the source.
   *
   * %TEMP% rather than the build directory, so that one configuration's leavings cannot be read by
   * another. A test that finds a translation or a setting it did not write is a test whose result
   * depends on what ran before it, and the settings default file is exactly that shape: it is read at
   * startup if it is there and written at shutdown, so a build tree that kept it would hand the next
   * run the last one's.
   *
   * The name is random for the reason the per-fixture script directories are: ctest is free to run these
   * tests in parallel, and each of those runs is this program, so a fixed name would have one run
   * deleting the directory another was still using.
   *
   * Entered before the first test and left after the last, so that a fixture destructor - which is where
   * a translation and a setting are written - still runs inside it. Nothing is made on the way in beyond
   * the directory itself; the registries make what they need as they need it.
   */
  class ScratchWorkingDir {
  public:
    ScratchWorkingDir() : original_(std::filesystem::current_path()) {
      std::error_code ec;
      dir_ = std::filesystem::temp_directory_path() / ("gta_base_menu_test_" + std::to_string(std::random_device{}()));
      std::filesystem::remove_all(dir_, ec);
      std::filesystem::create_directories(dir_, ec);
      std::filesystem::current_path(dir_, ec);

      if (ec) {
        // Reported rather than passed over, because what it means is that every test below writes into
        // wherever this was launched from - which is the thing this class exists to prevent, and is
        // otherwise invisible from inside a passing run.
        std::cerr << "menu_test: no scratch working directory (" << ec.message() << "), writing into "
                  << original_.string() << '\n';
      }
    }

    /// Left before being removed, because Windows will not delete the working directory of a running
    /// process. The removal is best-effort for the same reason the rest of this is: a logger still
    /// holding a file open inside it is not a reason to fail a test run, and what it leaves behind is a
    /// directory in %TEMP% that costs nothing.
    ~ScratchWorkingDir() {
      std::error_code ec;
      std::filesystem::current_path(original_, ec);
      std::filesystem::remove_all(dir_, ec);
    }

    ScratchWorkingDir(const ScratchWorkingDir&) = delete;
    ScratchWorkingDir(ScratchWorkingDir&&) = delete;
    ScratchWorkingDir& operator=(const ScratchWorkingDir&) = delete;
    ScratchWorkingDir& operator=(ScratchWorkingDir&&) = delete;

  private:
    std::filesystem::path dir_;
    std::filesystem::path original_;
  };
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);

  // After InitGoogleTest, and gtest is the reason it cannot be before: it records the directory a
  // relative --gtest_output is resolved against while the tests are being registered, ahead of main,
  // so that a test program is free to move itself. A report asked for by relative path is therefore
  // still written where it was asked for from, and not into the scratch directory below.
  const ScratchWorkingDir scratch;

  return RUN_ALL_TESTS();
}
