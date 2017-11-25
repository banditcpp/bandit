#include <specs/specs.h>

namespace bd = bandit::detail;

go_bandit([]() {
  describe("reporter::crash", [&]() {
    std::stringstream stm;
    std::unique_ptr<reporter::crash> reporter;
    failure_formatter::posix formatter;

    before_each([&]() {
      stm.str(std::string());
      reporter.reset(new reporter::crash(stm, formatter));
    });

    auto output = [&]() {
      return stm.str();
    };

    it("has empty output on empty test set", [&]() {
      reporter->test_run_starting();
      reporter->test_run_complete();
      AssertThat(output(), IsEmpty());
    });

    it("has expected output for a successful test", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->it_starting("passes");
      reporter->it_succeeded("passes");
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), Equals("context | passes\n"));
    });

    it("has expected output for a successful and a failing test", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->it_starting("passes");
      reporter->it_succeeded("passes");
      reporter->it_starting("fails");
      bd::assertion_exception exception("assertion failed!", "some_file", 123);
      reporter->it_failed("fails", exception);
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), Contains("context | passes\ncontext | fails\nFAILED"));
      AssertThat(output(), Contains("# FAILED context fails:\nsome_file:123: assertion failed!"));
    });

    it("has expected output for a non-assertion_exception", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->it_starting("throws an unknown exception");
      reporter->it_unknown_error("throws an unknown exception");
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), Contains("context | throws an unknown exception\nUNKNOWN EXCEPTION"));
      AssertThat(output(), Contains("# FAILED context throws an unknown exception: Unknown exception"));
    });

    it("has expected output for a failing test with nested contexts", [&]() {
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
      AssertThat(output(), Contains("context | passes\ncontext | nested context | fails\nFAILED"));
      AssertThat(output(), Contains("# FAILED context nested context fails:\nsome_file:123: assertion failed!"));
    });

    it("has expected output for test run errors within a context", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->context_starting("nested context");
      bd::test_run_error error("error!");
      reporter->test_run_error("desc", error);
      reporter->context_ended("nested context");
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), Contains("# ERROR context nested context: desc: error!\n"));
    });

    it("has no output for skipped tests", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->it_skip("skips");
      reporter->it_starting("passes");
      reporter->it_succeeded("passes");
      reporter->it_skip("skips");
      reporter->it_starting("fails");
      bd::assertion_exception exception("assertion failed!", "some_file", 123);
      reporter->it_failed("fails", exception);
      reporter->it_skip("skips");
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), Contains("context | fails\nFAILED"));
      AssertThat(output(), Contains("# FAILED context fails:\nsome_file:123: assertion failed!"));
    });

    it("has empty output in case all tests are skipped", [&]() {
      reporter->test_run_starting();
      reporter->context_starting("context");
      reporter->it_skip("skips first");
      reporter->it_skip("skips second");
      reporter->it_skip("skips last");
      reporter->context_ended("context");
      reporter->test_run_complete();
      AssertThat(output(), IsEmpty());
    });
  });
});
