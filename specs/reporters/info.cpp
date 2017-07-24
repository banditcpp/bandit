#include <specs/specs.h>
namespace bd = bandit::detail;

go_bandit([]() {
  describe("reporter::info", [&]() {
    std::stringstream stm;
    std::unique_ptr<reporter::info> reporter;
    failure_formatter::posix formatter;
    bd::colorizer colorizer(false);

    before_each([&]() {
      stm.str(std::string());
      reporter = std::unique_ptr<reporter::info>(new reporter::info(stm, formatter, colorizer));
    });

    auto output = [&]() {
      return stm.str();
    };

    describe("an empty test run", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->test_run_complete();
      });

      it("reports no run tests in summary", [&]() {
        AssertThat(output(), Equals("\nTests run: 0\n\n"));
      });

      it("is considered successful", [&]() {
        AssertThat(reporter->did_we_pass(), IsTrue());
      });
    });

    describe("one successful test run", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_starting("passes");
        reporter->it_succeeded("passes");
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows the context", [&]() {
        AssertThat(output(), Contains("begin context"));
        AssertThat(output(), Contains("end context"));
      });

      it("shows the context summary", [&]() {
        AssertThat(output(), Contains("end context 1 total"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("[ PASS ] it passes"));
      });

      it("reports one run test in the summary", [&]() {
        AssertThat(output(), Contains("Tests run: 1"));
      });

      it("reports one passed test in the summary", [&]() {
        AssertThat(output(), Contains("Passed: 1"));
      });

      it("does not report failed tests in the summary", [&]() {
        AssertThat(output(), !Contains("Failed: "));
      });

      it("does not report skipped tests", [&]() {
        AssertThat(output(), !Contains("Skipped: "));
      });

      it("ends successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsTrue());
      });
    });

    describe("one successful and one failing test run", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_starting("passes");
        reporter->it_succeeded("passes");
        reporter->it_starting("fails");
        bd::assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("fails", exception);
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows the context", [&]() {
        AssertThat(output(), Contains("begin context"));
        AssertThat(output(), Contains("end context"));
      });

      it("shows the context summary", [&]() {
        AssertThat(output(), Contains("end context 2 total 1 failed"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("[ PASS ] it passes"));
      });

      it("shows the failing test as failed", [&]() {
        AssertThat(output(), Contains("[ FAIL ] it fails"));
      });

      it("reports two run tests", [&]() {
        AssertThat(output(), Contains("Tests run: 2"));
      });

      it("reports one passed test in the summary", [&]() {
        AssertThat(output(), Contains("Passed: 1"));
      });

      it("reports one failed test in the summary", [&]() {
        AssertThat(output(), Contains("Failed: 1"));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("Skipped: "));
      });

      it("does not end successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsFalse());
      });

      it("reports a list of failures", [&]() {
        AssertThat(output(), Contains("List of failures:\n"));
      });

      it("reports the failed assertion", [&]() {
        AssertThat(output(), Contains("(*) context fails:\nsome_file:123: assertion failed!"));
      });

      it("only reports assertion failure once", [&]() {
        AssertThat(output(), Has().Exactly(1).EndingWith("assertion failed!"));
      });
    });

    describe("a test run with a non assertion_exception thrown", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_starting("throws an unknown exception");
        reporter->it_unknown_error("throws an unknown exception");
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows the context summary", [&]() {
        AssertThat(output(), Contains("end context 1 total 1 failed"));
      });

      it("shows the unknown exception as error", [&]() {
        AssertThat(output(), Contains("-ERROR-> it throws"));
      });

      it("reports one failed test in the summary", [&]() {
        AssertThat(output(), Contains("Failed: 1"));
      });

      it("does not report passed tests in the summary", [&]() {
        AssertThat(output(), !Contains("Passed: "));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("Skipped: "));
      });

      it("reports the failed test", [&]() {
        AssertThat(output(), Contains("(*) context throws an unknown exception: Unknown exception"));
      });

      it("does not end successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsFalse());
      });
    });

    describe("a failing test run with nested contexts", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_starting("passes");
        reporter->it_succeeded("passes");
        reporter->context_starting("nested context");
        reporter->it_starting("fails");
        bd::assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("fails", exception);
        reporter->context_ended("a nested context");
        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("shows the context", [&]() {
        AssertThat(output(), Contains("begin context\n"));
        AssertThat(output(), Contains("\nend context 2 total 1 failed"));
      });

      it("shows the nested context", [&]() {
        AssertThat(output(), Contains("\n  begin nested context"));
        AssertThat(output(), Contains("\n  end nested context 1 total 1 failed"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("  [ PASS ] it passes\n"));
      });

      it("shows the failing test as failed", [&]() {
        AssertThat(output(), Contains("    [ FAIL ] it fails\n"));
      });

      it("reports two run tests", [&]() {
        AssertThat(output(), Contains("Tests run: 2"));
      });

      it("reports one passed test in the summary", [&]() {
        AssertThat(output(), Contains("Passed: 1"));
      });

      it("reports one failed test in the summary", [&]() {
        AssertThat(output(), Contains("Failed: 1"));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("Skipped: "));
      });

      it("reports the failed assertion", [&]() {
        AssertThat(output(), Contains("(*) context nested context fails:\nsome_file:123: assertion failed!"));
      });

      it("reports a failed test run", [&]() {
        AssertThat(reporter->did_we_pass(), IsFalse());
      });
    });

    describe("a context with test run errors", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        bd::test_run_error error("error!");
        reporter->test_run_error("context", error);
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("reports a list of run errors", [&]() {
        AssertThat(output(), Contains("List of run errors:\n"));
      });

      it("reports that the context has failed", [&]() {
        AssertThat(output(), Contains(" (*) Failed to run \"context\": error \"error!\""));
      });

      it("reports no run tests in summary but one error", [&]() {
        AssertThat(output(), Contains("Tests run: 0\nErrors: 1"));
      });

      it("does not end succesful", [&]() {
        AssertThat(reporter->did_we_pass(), IsFalse());
      });
    });

    describe("a context with a failing and a skipped test", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_starting("fails");
        bd::assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("fails", exception);
        reporter->it_skip("skips");
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows the failing test as failed", [&]() {
        AssertThat(output(), Contains("  [ FAIL ] it fails\n"));
      });

      it("does not show the skipped test", [&]() {
        AssertThat(output(), !Contains("[ SKIP ]"));
      });

      it("shows the skipped test in the context summary", [&]() {
        AssertThat(output(), Contains("end context 2 total 1 skipped 1 failed"));
      });

      it("does not report passed tests in the summary", [&]() {
        AssertThat(output(), !Contains("Passed: "));
      });

      it("reports one failed test in the summary", [&]() {
        AssertThat(output(), Contains("Failed: 1"));
      });

      it("reports one skipped test in the summary", [&]() {
        AssertThat(output(), Contains("Skipped: 1"));
      });
    });

    describe("a context where all tests are skipped", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->it_skip("skips first");
        reporter->it_skip("skips second");
        reporter->it_skip("skips last");
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows only the context", [&]() {
        AssertThat(output(), Contains("begin context\nend context 3 total 3 skipped"));
      });

      it("reports zero run tests in the summary", [&]() {
        AssertThat(output(), Contains("Tests run: 0"));
      });

      it("does not report passed tests in the summary", [&]() {
        AssertThat(output(), !Contains("Passed: "));
      });

      it("does not report failed tests in the summary", [&]() {
        AssertThat(output(), !Contains("Failed: "));
      });

      it("reports three skipped tests in the summary", [&]() {
        AssertThat(output(), Contains("Skipped: 3"));
      });

      it("ends successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsTrue());
      });
    });

    describe("a nested context where all tests are skipped", [&]() {
      before_each([&]() {
        reporter->test_run_starting();
        reporter->context_starting("context");
        reporter->context_starting("nested context");
        reporter->context_starting("really nested context");
        reporter->it_skip("skips first");
        reporter->context_ended("really nested context");
        reporter->it_skip("skips second");
        reporter->context_ended("nested context");
        reporter->it_skip("skips last");
        reporter->it_starting("passes");
        reporter->it_succeeded("passes");
        reporter->context_ended("context");
        reporter->test_run_complete();
      });

      it("shows only the passed test and not the nested contexts", [&]() {
        AssertThat(output(), Contains("begin context"));
        AssertThat(output(), !Contains("nested context"));
        AssertThat(output(), Contains("  [ PASS ] it passes\nend context 4 total 3 skipped"));
      });

      it("reports one run test in the summary", [&]() {
        AssertThat(output(), Contains("Tests run: 1"));
      });

      it("reports one passed test in the summary", [&]() {
        AssertThat(output(), Contains("Passed: 1"));
      });

      it("reports three skipped tests in the summary", [&]() {
        AssertThat(output(), Contains("Skipped: 3"));
      });

      it("ends successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsTrue());
      });
    });
  });
});
