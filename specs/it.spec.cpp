#include <specs/specs.h>
using namespace bandit::fakes;

go_bandit([](){
  describe("it:", [&](){
    voidfunc_t it_func;
    fake_reporter_ptr reporter;
    contextstack_t contexts;

    describe("with succeeding test", [&](){
      before_each([&](){
        it_func = [](){};
        reporter = fake_reporter_ptr(new fake_reporter());
      });

      it("tells reporter it's starting", [&](){
        it("my it", it_func, *(reporter.get()), contexts);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_starting: my it"));
      });
    });
    
  });
});
