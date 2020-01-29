#include <specs/specs.h>

namespace bd = bandit::detail;

go_bandit([]() {
  describe("reporter::xunit", [&]() {
    std::stringstream stm;
    failure_formatter::posix formatter;
    std::unique_ptr<reporter::xunit> reporter;

    auto output = [&]() { return stm.str(); };

    before_each([&]() {
      stm.str(std::string());
      reporter.reset(new reporter::xunit(stm, formatter, true));
    });

    describe("an empty test run", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->test_run_complete();
      });

      it("adds a header to the output", [&]() {
        AssertThat(output(), StartsWith("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
      });

      it("outputs an empty test report", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"0\" errors=\"0\" failures=\"0\" time=\"0."));
        AssertThat(output(), Contains(
            "\">\n"
            "</testsuite>\n"));
      });
    });

    describe("a test run with one, successful, test", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");
        reporter->it_succeeded("my test");
        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("outputs info about the successful test", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"1\" errors=\"0\" failures=\"0\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t<testcase classname=\"my context\" name=\"my test\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t</testcase>\n"
            "</testsuite>\n"));
      });
    });

    describe("a test run with one, failing test", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");

        bd::assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("my test", exception);

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("outputs the failing test", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"1\" errors=\"0\" failures=\"1\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t<testcase classname=\"my context\" name=\"my test\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t\t<failure message=\"some_file:123: assertion failed!\" />\n"
            "\t</testcase>\n"
            "</testsuite>\n"));
      });
    });

    describe("a test run with one test with an unknown error", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");

        reporter->it_unknown_error("my test");

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("outputs the erroneous test", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"1\" errors=\"0\" failures=\"1\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t<testcase classname=\"my context\" name=\"my test\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t\t<failure message=\"Unknown exception\" />\n"
            "\t</testcase>\n"
            "</testsuite>\n"));
      });
    });

    describe("a test run with one test failing with characters that need escaping", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("my context & < > \\ \"");
        reporter->it_starting("my test & < > \\ \"");

        bd::assertion_exception exception("assertion failed & < > \\ \"", "some_file", 123);
        reporter->it_failed("my test & < > \\ \"", exception);

        reporter->context_ended("my context & < > \\ \"");
        reporter->test_run_complete();
      });

      it("outputs the escaped characters", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"1\" errors=\"0\" failures=\"1\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t<testcase classname=\"my context &amp; &lt; &gt; &apos; &quot;\" "
            "name=\"my test &amp; &lt; &gt; &apos; &quot;\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t\t<failure message=\"some_file:123: assertion failed &amp; &lt; &gt; &apos; &quot;\" />\n"
            "\t</testcase>\n"
            "</testsuite>\n"));
      });
    });

    describe("a context with a skipped test", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("my context");

        reporter->it_starting("my test");
        reporter->it_succeeded("my test");
        reporter->it_skip("my skipped test");

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("outputs info about the skipped test", [&]() {
        AssertThat(output(), Contains(
            "<testsuite name=\"bandit\" tests=\"1\" errors=\"0\" failures=\"0\" skipped=\"1\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t<testcase classname=\"my context\" name=\"my test\" time=\""));
        AssertThat(output(), Contains(
            "\">\n"
            "\t</testcase>\n"
            "\t<testcase classname=\"my context\" name=\"my skipped test\" time=\"0.0\">\n"
            "\t\t<skipped />\n"
            "\t</testcase>\n"
            "</testsuite>\n"));
      });
    });
  });
});
