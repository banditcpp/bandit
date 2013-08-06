#include <specs/specs.h>

go_bandit([](){

  describe("single line reporter", [&](){
    unique_ptr<std::stringstream> stm;
    unique_ptr<bandit::single_line_reporter> reporter;
    default_failure_formatter formatter;
    bandit::detail::colorizer colorizer(false);
  
    before_each([&](){
      stm = unique_ptr<std::stringstream>(new std::stringstream());
      reporter = unique_ptr<bandit::single_line_reporter>(
        new single_line_reporter(*stm, formatter, colorizer));
    });

    auto output = [&](){ return stm->str(); };
  
   describe("an empty test run", [&](){
   
     before_each([&](){
       reporter->test_run_starting();
       reporter->test_run_complete();
     });

     it("reports that no tests were run", [&](){
       AssertThat(output(), Equals("\nCould not find any tests.\n"));
     });
   
      it("is not considered successful", [&](){
        AssertThat(reporter->did_we_pass(), Equals(false));
      });
   }); 

    describe("a successful test run", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");
        reporter->it_succeeded("my test");
        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("reports a successful test run", [&](){
        AssertThat(output(), EndsWith("Test run complete. 1 tests run. 1 succeeded.\n"));
      });
    
      it("displays progress for the test", [&](){
        AssertThat(output(), StartsWith("\rExecuted 0 tests."
                                        "\rExecuted 1 tests."));
      });

      it("reports a successful test run", [&](){
        AssertThat(reporter->did_we_pass(), Equals(true));
      });
    });

    describe("a failing test run", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");

        assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("my test", exception);

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("reports a failing test run in summary", [&](){
        AssertThat(output(), EndsWith("Test run complete. 1 tests run. 0 succeeded. 1 failed.\n"));
      });

      it("reports the failed assertion", [&](){
        AssertThat(output(), Contains("my context my test:\nsome_file:123: assertion failed!"));
      });

      it("reports failing test in progress", [&](){
        AssertThat(output(), StartsWith("\rExecuted 0 tests."
                                        "\rExecuted 1 tests. 0 succeeded. 1 failed."));
      });
    
      it("reports a failed test run", [&](){
        AssertThat(reporter->did_we_pass(), Equals(false));
      });
    });
  
    describe("a test run with a non assertion_exception thrown", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->it_starting("my test");

        reporter->it_unknown_error("my test");

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("reports failing test in progress", [&](){
        AssertThat(output(), StartsWith("\rExecuted 0 tests."
                                        "\rExecuted 1 tests. 0 succeeded. 1 failed."));
      });

      it("reports the failed test", [&](){
        AssertThat(output(), Contains("my context my test:\nUnknown exception"))
      });
    
    });

    describe("a failing test run with nested contexts", [&](){

      before_each([&](){
        reporter->test_run_starting();
        reporter->context_starting("my context");
        reporter->context_starting("a nested context");
        reporter->it_starting("my test");

        assertion_exception exception("assertion failed!", "some_file", 123);
        reporter->it_failed("my test", exception);

        reporter->context_ended("a nested context");
        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("reports a failing test run in summary", [&](){
        AssertThat(output(), EndsWith("Test run complete. 1 tests run. 0 succeeded. 1 failed.\n"));
      });

      it("reports the failed assertion", [&](){
        AssertThat(output(), Contains("my context a nested context my test:\nsome_file:123: assertion failed!"));
      });

      it("displays a failed test in progress report", [&](){
        AssertThat(output(), StartsWith("\rExecuted 0 tests."
                                        "\rExecuted 1 tests. 0 succeeded. 1 failed."));
      });

      it("reports a failed test run", [&](){
        AssertThat(reporter->did_we_pass(), Equals(false));
      });
    
    });

    describe("a context with test run errors", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->context_starting("my context");

        test_run_error error("we dun goofed!");
        reporter->test_run_error("my context", error);

        reporter->context_ended("my context");
        reporter->test_run_complete();
      });

      it("reports that the context has failed", [&](){
        AssertThat(output(), Contains("Failed to run \"my context\": error \"we dun goofed!\""));
      });

      it("reports test run errors in summary", [&](){
        AssertThat(output(), EndsWith("Test run complete. 0 tests run. 0 succeeded. 1 test run errors.\n"))
      });

      it("reports a failed test run", [&](){
        AssertThat(reporter->did_we_pass(), Equals(false));
      });
    });

    
  });

});
