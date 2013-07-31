#include <specs/specs.h>
using namespace bandit::fakes;

go_bandit([](){
  describe("it:", [&](){
    voidfunc_t it_func;
    fake_reporter_ptr reporter;
    unique_ptr<contextstack_t> contexts;
    unique_ptr<fake_context> context;
    bandit::adapters::snowhouse_adapter assertion_adapter;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());
      contexts = unique_ptr<contextstack_t>(new contextstack_t());
      context = unique_ptr<fake_context>(new fake_context());
      contexts->push_back(context.get());
    });

    auto call_it = [&]() {
      it("my it", it_func, *reporter, *contexts, assertion_adapter);
    };

    it("tells the current context that execution has started", [&](){
      // This is important as once execution has started,
      // before_each and after_each calls cannot be guaranteed to
      // be run before any 'it' method.
      
      call_it();
      AssertThat(context->call_log(), Has().AtLeast(1).EqualTo("execution_is_starting"));
    });

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

      it("calls before_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
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

      it("calls before_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
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

      it("calls before_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&](){
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
      });
    });
    
  });
});
