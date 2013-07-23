#include <specs/specs.h>
using namespace bandit::fakes;

go_bandit([](){
  describe("it:", [&](){
    voidfunc_t it_func;
    fake_reporter_ptr reporter;
    contextstack_t contexts;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());
    });

    describe("with succeeding test", [&](){
      before_each([&](){
        it_func = [](){};
      });

      it("tells reporter it's starting", [&](){
        it("my it", it_func, *(reporter.get()), contexts);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_starting: my it"));
      });

      it("tells reporter it's succeeded", [&](){
        it("my it", it_func, *(reporter.get()), contexts);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_succeeded: my it"));
      });
    });

    describe("with failing test", [&](){
      before_each([&](){
        it_func = [](){ AssertThat(3, Equals(2)); };
      });

      it("tells reporter it's failed", [&](){
        it("my it", it_func, *(reporter.get()), contexts);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_failed: my it"));
      });
    });
    
  });
});
