#include <specs/specs.h>
using namespace bandit::fakes;

go_bandit([](){
  describe("it:", [&](){
    voidfunc_t it_func;
    fake_reporter_ptr reporter;
    unique_ptr<contextstack_t> contexts;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());
      contexts = unique_ptr<contextstack_t>(new contextstack_t());
    });

    auto call_it = [&]() {
      it("my it", it_func, *(reporter.get()), *(contexts.get()), bandit::adapters::snowhouse);
    };

    describe("with succeeding test", [&](){
      before_each([&](){
        it_func = [](){};
      });

      it("tells reporter it's starting", [&](){
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_starting: my it"));
      });

      it("tells reporter it's succeeded", [&](){
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_succeeded: my it"));
      });
    });

    describe("with failing test", [&](){
      before_each([&](){
        it_func = [](){ AssertThat(3, Equals(2)); };
      });

      it("tells reporter it's failed", [&](){
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_failed: my it (Expected: equal to 2 Actual: 3 )"));
      });

    });

    describe("with crashing test", [&](){
      before_each([&](){
        it_func = [](){ throw std::logic_error("serious crash"); };
      });

      it("tells reporter it's failed", [&](){
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_unknown_error: my it"));
      });
    });
    
  });
});
