#include <specs/specs.h>
namespace bd = bandit::detail;

go_bandit([]() {
  describe("Qt Creator failure formatter", [&]() {
    failure_formatter::qt_creator formatter;

    it("formats assertions with file and line number", [&]() {
      bd::assertion_exception exception("message", "file", 321);
      AssertThat(formatter.format(exception), Equals("file://file:321: message"));
    });

    it("formats assertions without file and line number", [&]() {
      bd::assertion_exception exception("message");
      AssertThat(formatter.format(exception), Equals("message"));
    });
  });
});
