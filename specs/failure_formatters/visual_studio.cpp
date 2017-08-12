#include <specs/specs.h>
namespace bd = bandit::detail;

go_bandit([]() {
  describe("Visual Studio failure formatter", [&]() {
    failure_formatter::visual_studio formatter;

    it("formats assertions with file and line number", [&]() {
      bd::assertion_exception exception("message", "file", 321);
      AssertThat(formatter.format(exception), Equals("file(321): message"));
    });

    it("formats assertions without file and line number", [&]() {
      bd::assertion_exception exception("message");
      AssertThat(formatter.format(exception), Equals("bandit: message"));
    });
  });
});
