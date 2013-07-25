#include <specs/specs.h>

namespace bd = bandit::detail;
using namespace bandit::fakes;
using namespace bandit::specs::util;


go_bandit([](){

  describe("run:", [&](){
    unique_ptr<bd::spec_registry> specs;
    unique_ptr<argv_helper> argv;
    fake_reporter_ptr reporter;

    auto call_run = [&](){
        bandit::run(argv->argc(), argv->non_const(), *(specs.get()), *(reporter.get()));
    };
  
    before_each([&](){
      specs = unique_ptr<bd::spec_registry>(new bd::spec_registry());

      reporter = fake_reporter_ptr(new fake_reporter());

      const char* args[] = {"executable"};
      argv = unique_ptr<argv_helper>(new argv_helper(1, args));
    });

    describe("one spec registered", [&](){
      int number_of_specs_called;
    
      before_each([&](){
        number_of_specs_called = 0;
        specs->push_back([&](){ number_of_specs_called++; });
      });
    
      it("calls each context", [&](){
        call_run();
        AssertThat(number_of_specs_called, Equals(1));
      });

      it("tells reporter a test run is about to start", [&](){
        call_run();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("test_run_starting"));
      });

      it("tells reporter a test run has completed", [&](){
        call_run();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("test_run_complete"));
      });
    });

  
  });

});
