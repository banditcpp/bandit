#include <specs/specs.h>
namespace bd = bandit::detail;

go_bandit([]() {
  describe("reporter::spec", [&]() {
    std::stringstream stm;
    std::unique_ptr<reporter::spec> reporter;
    failure_formatter::posix formatter;
    colorizer::off colorizer;

    before_each([&]() {
      stm.str(std::string());
      reporter.reset(new reporter::spec(stm, formatter, colorizer));
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
        AssertThat(output(), Equals("\nCould not find any tests.\n"));
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
        AssertThat(output(), Contains("describe context"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("\t- it passes ... OK"));
      });

      it("reports one run and passed test in the summary", [&]() {
        AssertThat(output(), Contains("Test run complete. 1 tests run. 1 succeeded."));
      });

      it("does not report failed tests in the summary", [&]() {
        AssertThat(output(), !Contains("failed"));
      });

      it("does not report skipped tests", [&]() {
        AssertThat(output(), !Contains("skipped"));
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
        AssertThat(output(), Contains("describe context"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("\t- it passes ... OK"));
      });

      it("shows the failing test as failed", [&]() {
        AssertThat(output(), Contains("\t- it fails ... FAILED"));
      });

      it("reports failures in the summary", [&]() {
        AssertThat(output(), Contains("There were failures!"));
      });

      it("reports two run tests, one ok, one failed", [&]() {
        AssertThat(output(), Contains("Test run complete. 2 tests run. 1 succeeded. 1 failed."));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("skipped"));
      });

      it("does not end successfully", [&]() {
        AssertThat(reporter->did_we_pass(), IsFalse());
      });

      it("reports the failed assertion", [&]() {
        AssertThat(output(), Contains("context fails:\nsome_file:123: assertion failed!"));
      });

      it("only reports assertion failure once", [&]() {
        AssertThat(output(), Has().Exactly(1).EqualTo("context fails:"));
        AssertThat(output(), Has().Exactly(1).EndingWith(": assertion failed!"));
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

      it("shows the unknown exception as error", [&]() {
        AssertThat(output(), Contains("\t- it throws an unknown exception ... ERROR"));
      });

      it("reports failures in the summary", [&]() {
        AssertThat(output(), Contains("There were failures!"));
      });

      it("reports one test in the summary that failed", [&]() {
        AssertThat(output(), Contains("Test run complete. 1 tests run. 0 succeeded. 1 failed."));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("skipped"));
      });

      it("reports the failed test", [&]() {
        AssertThat(output(), Contains("context throws an unknown exception:\nUnknown exception"));
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
        AssertThat(output(), Contains("describe context\n"));
      });

      it("shows the nested context", [&]() {
        AssertThat(output(), Contains("\tdescribe nested context\n"));
      });

      it("shows the passing test as passed", [&]() {
        AssertThat(output(), Contains("\t- it passes ... OK\n"));
      });

      it("shows the failing test as failed", [&]() {
        AssertThat(output(), Contains("\t\t- it fails ... FAILED\n"));
      });

      it("does not report skipped tests in the summary", [&]() {
        AssertThat(output(), !Contains("skipped"));
      });

      it("reports the failed assertion", [&]() {
        AssertThat(output(), Contains("context nested context fails:\nsome_file:123: assertion failed!"));
      });

      it("does not end successfully", [&]() {
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

      it("reports that the context has failed", [&]() {
        AssertThat(output(), Contains("Failed to run \"context\": error \"error!\""));
      });

      it("reports no run tests in summary but one error", [&]() {
        AssertThat(output(), Contains("Test run complete. 0 tests run. 0 succeeded. 1 test run errors."));
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
        AssertThat(output(), Contains("\t- it fails ... FAILED\n"));
      });

      it("shows the skipped test as skipped", [&]() {
        AssertThat(output(), Contains("\t- it skips ... SKIPPED\n"));
      });

      it("reports 0 passed, 1 skipped, 1 failed test", [&]() {
        AssertThat(output(), Contains("Test run complete. 1 tests run. 0 succeeded. 1 skipped. 1 failed."));
      });
    });

    reporter.reset(); // necessary so that reporter dtor is called before colorizer dtor
  });
});
