#include <specs/specs.h>

go_bandit([]() {
  describe("colorizers", [&]() {
#if !defined(_WIN32) || defined(BANDIT_CONFIG_COLOR_ANSI)
    describe("colorizer::light", [&]() {
      colorizer::light colorizer;

      it("sets 'bad' color to red", [&]() {
        AssertThat(colorizer.bad(), Equals("\033[1;31m"));
      });

      it("sets 'good' color to green", [&]() {
        AssertThat(colorizer.good(), Equals("\033[1;32m"));
      });

      it("sets 'neutral' color to yellow", [&]() {
        AssertThat(colorizer.neutral(), Equals("\033[1;33m"));
      });

      it("sets 'info' color to blue", [&]() {
        AssertThat(colorizer.info(), Equals("\033[1;34m"));
      });

      it("emphasizes using color white", [&]() {
        AssertThat(colorizer.emphasize(), Equals("\033[1;37m"));
      });

      it("resets color", [&]() {
        AssertThat(colorizer.reset(), Equals("\033[0m"));
      });
    });
#endif

    describe("colorizer::off", [&]() {
      colorizer::off colorizer;

      it("ignores setting 'good' color", [&]() {
        AssertThat(colorizer.good(), Equals(""));
      });

      it("ignores setting 'bad' color", [&]() {
        AssertThat(colorizer.bad(), Equals(""));
      });

      it("ignores setting 'neutral' color", [&]() {
        AssertThat(colorizer.good(), Equals(""));
      });

      it("ignores setting 'info' color", [&]() {
        AssertThat(colorizer.good(), Equals(""));
      });

      it("ignores emphasizing", [&]() {
        AssertThat(colorizer.good(), Equals(""));
      });

      it("ignores resetting colors", [&]() {
        AssertThat(colorizer.reset(), Equals(""));
      });
    });
  });
});
