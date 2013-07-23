#include <bandit/bandit.h>
using namespace bandit;

#include <snowhouse/snowhouse.h>
using namespace snowhouse;

#include <specs/fakes/logging_fake.h>

namespace bandit { namespace specs {
  struct fake_reporter : 
    public bandit::reporter, 
    public bandit::specs::logging_fake
  {
    void context_starting(const char* desc)
    {
      log("context_starting: ", desc);
    }

    void context_ended(const char* desc)
    {
      log("context_ended: ", desc);
    }
  };
  typedef unique_ptr<fake_reporter> fake_reporter_ptr;
}}

using namespace bandit::specs;

run_bandit([](){

  describe("describe:", [](){
    bandit::detail::voidfunc_t describe_fn;
    bandit::specs::fake_reporter_ptr reporter;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());
    });

    describe("with a succeeding 'it'", [&](){

      before_each([&](){
        describe_fn = [](){};
      });

      it("tells reporter it's starting a run", [&](){
        describe("something", describe_fn, reporter.get());
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_starting: something"));
      });

      it("tells reporter it's finished a run", [&](){
        describe("something", describe_fn, reporter.get());
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_ended: something"));
      });

    });


  });

});
