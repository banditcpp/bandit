#include <specs/specs.h>

namespace bd = bandit::detail;
using namespace bandit::fakes;

// TODO: Move to own file in specs.
// TODO: Comment rationale for this.
struct argv_helper
{
  argv_helper(int argc, const char* argv[])
    : argc_(argc), argv_(argv)
  {
    non_const_argv_ = new char*[argc];
    for(int i=0; i < argc; i++)
    {
      non_const_argv_[i] = new char[strlen(argv[i] + 1)];
      strcpy(non_const_argv_[i], argv_[i]);
    }
  }

  ~argv_helper()
  {
    for(int i=0; i < argc_; i++)
    {
      delete[] non_const_argv_[i];
    }

    delete[] non_const_argv_;
  }

  char** non_const()
  {
    return non_const_argv_;
  }

  int argc()
  {
    return argc_;
  }

  private:
  int argc_;
  const char** argv_;
  char** non_const_argv_;
};


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
