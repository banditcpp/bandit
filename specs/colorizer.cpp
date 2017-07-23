#include <specs/specs.h>

go_bandit([]() {
  describe("colorizer: ", [&]() {
#if !defined(_WIN32) || defined(BANDIT_CONFIG_COLOR_ANSI)
    describe("colors enabled", [&]() {
      bandit::detail::colorizer colorizer;

      it("can set color to green", [&]() {
        AssertThat(colorizer.green(), Equals("\033[1;32m"));
      });

      it("set color to red", [&]() {
        AssertThat(colorizer.red(), Equals("\033[1;31m"));
      });
      it("resets color", [&]() {
        AssertThat(colorizer.reset(), Equals("\033[0m"));
      });
    });
#endif

    describe("colors disabled", [&]() {
      bandit::detail::colorizer colorizer(false);

      it("ignores setting color to green", [&]() {
        AssertThat(colorizer.green(), Equals(""));
      });

      it("ignores setting color to red", [&]() {
        AssertThat(colorizer.red(), Equals(""));
      });

      it("ignores resetting colors", [&]() {
        AssertThat(colorizer.reset(), Equals(""));
      });
    });
  });
});
