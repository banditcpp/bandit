#include <specs/specs.h>

go_bandit([](){

  describe("dots_reporter:", [&](){
    unique_ptr<std::stringstream> stm;
    unique_ptr<bandit::dots_reporter> reporter;
  
    before_each([&](){
      stm = unique_ptr<std::stringstream>(new std::stringstream());
      reporter = unique_ptr<bandit::dots_reporter>(new dots_reporter(*(stm.get())));
    });

    auto output = [&](){ return stm->str(); };

    describe("an empty test run", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->test_run_complete();
      });

      it("reports no tests where run", [&](){
        AssertThat(output(), Equals("\nCould not find any tests.\n"));
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
    
    });

  });

});
