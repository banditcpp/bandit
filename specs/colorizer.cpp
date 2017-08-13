#include <specs/specs.h>

go_bandit([]() {
  describe("colorizer::light", [&]() {
#if !defined(_WIN32) || defined(BANDIT_CONFIG_COLOR_ANSI)
    describe("colors enabled", [&]() {
      colorizer::light colorizer;

      it("sets 'good' color to green", [&]() {
        AssertThat(colorizer.good(), Equals("\033[1;32m"));
      });

      it("sets 'bad' color to red", [&]() {
        AssertThat(colorizer.bad(), Equals("\033[1;31m"));
      });

      it("resets color", [&]() {
        AssertThat(colorizer.reset(), Equals("\033[0m"));
      });
    });
#endif

    describe("colors disabled", [&]() {
      colorizer::light colorizer(false);

      it("ignores setting 'good' color", [&]() {
        AssertThat(colorizer.good(), Equals(""));
      });

      it("ignores setting 'bad' color", [&]() {
        AssertThat(colorizer.bad(), Equals(""));
      });

      it("ignores resetting colors", [&]() {
        AssertThat(colorizer.reset(), Equals(""));
      });
    });
  });
});
