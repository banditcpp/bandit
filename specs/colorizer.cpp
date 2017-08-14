#include <specs/specs.h>

go_bandit([]() {
  describe("colorizers", [&]() {
#if !defined(_WIN32) || defined(BANDIT_CONFIG_COLOR_ANSI)
    describe("colorizer::light", [&]() {
      colorizer::backend color;
      colorizer::light colorizer;

      it("sets 'bad' color to red", [&]() {
        AssertThat(colorizer.bad(), Equals(color.red()));
      });

      it("sets 'good' color to green", [&]() {
        AssertThat(colorizer.good(), Equals(color.green()));
      });

      it("sets 'neutral' color to yellow", [&]() {
        AssertThat(colorizer.neutral(), Equals(color.yellow()));
      });

      it("sets 'info' color to blue", [&]() {
        AssertThat(colorizer.info(), Equals(color.blue()));
      });

      it("emphasizes using color white", [&]() {
        AssertThat(colorizer.emphasize(), Equals(color.white()));
      });

      it("resets color", [&]() {
        AssertThat(colorizer.reset(), Equals(color.reset()));
      });
    });

    describe("colorizer::dark", [&]() {
      colorizer::backend color;
      colorizer::dark colorizer;

      it("sets 'bad' color to dark red", [&]() {
        AssertThat(colorizer.bad(), Equals(color.dark_red()));
      });

      it("sets 'good' color to dark green", [&]() {
        AssertThat(colorizer.good(), Equals(color.dark_green()));
      });

      it("sets 'neutral' color to brown", [&]() {
        AssertThat(colorizer.neutral(), Equals(color.brown()));
      });

      it("sets 'info' color to dark blue", [&]() {
        AssertThat(colorizer.info(), Equals(color.dark_blue()));
      });

      it("emphasizes using color dark gray", [&]() {
        AssertThat(colorizer.emphasize(), Equals(color.dark_gray()));
      });

      it("resets color", [&]() {
        AssertThat(colorizer.reset(), Equals(color.reset()));
      });
    });
#endif

    describe("colorizer::off", [&]() {
      colorizer::off colorizer;

      it("ignores setting 'good' color", [&]() {
        AssertThat(colorizer.good(), IsEmpty());
      });

      it("ignores setting 'bad' color", [&]() {
        AssertThat(colorizer.bad(), IsEmpty());
      });

      it("ignores setting 'neutral' color", [&]() {
        AssertThat(colorizer.good(), IsEmpty());
      });

      it("ignores setting 'info' color", [&]() {
        AssertThat(colorizer.good(), IsEmpty());
      });

      it("ignores emphasizing", [&]() {
        AssertThat(colorizer.good(), IsEmpty());
      });

      it("ignores resetting colors", [&]() {
        AssertThat(colorizer.reset(), IsEmpty());
      });
    });
  });
});
