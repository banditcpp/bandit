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
  };
  typedef unique_ptr<fake_reporter> fake_reporter_ptr;
}}

using namespace bandit::specs;

run_bandit([](){

  describe("describe:", [](){
    bandit::detail::voidfunc_t a_describe_fn;
    bandit::specs::fake_reporter_ptr reporter;

    before_each([&](){
      a_describe_fn = [](){};
      reporter = fake_reporter_ptr(new fake_reporter());
    });

    it("tells reporter it's starting a run", [&](){
      describe("something", a_describe_fn, reporter.get());
      AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_starting: something"));
    });
  });

});
