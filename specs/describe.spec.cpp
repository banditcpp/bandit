#include <specs/specs.h>

using namespace bandit::fakes;

go_bandit([](){

  describe("describe:", [](){
    bandit::detail::voidfunc_t describe_fn;
    fake_reporter_ptr reporter;
    unique_ptr<contextstack_t> context_stack;
    unique_ptr<fake_context> global_context;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());

      context_stack = unique_ptr<contextstack_t>(new contextstack_t());

      global_context = unique_ptr<fake_context>(new fake_context());
      context_stack->push_back(global_context.get());
    });


    auto call_describe = [&](){
        describe("context name", describe_fn, *reporter, *context_stack);
    };

    describe("with a succeeding 'it'", [&](){
      int context_stack_size_while_running;

      before_each([&](){
        context_stack_size_while_running = 0;
        describe_fn = [&](){context_stack_size_while_running = context_stack->size();};
      });

      it("tells its parent context that execution has started", [&](){
        // This is important as once execution has started,
        // before_each and after_each calls cannot be guaranteed to
        // be run before any 'it' method.

        call_describe();
        AssertThat(global_context->call_log(), Has().AtLeast(1).EqualTo("execution_is_starting"));
      });

      it("tells reporter it's starting a run", [&](){
        call_describe();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_starting: context name"));
      });

      it("tells reporter it's finished a run", [&](){
        call_describe();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_ended: context name"));
      });

      it("pushes a new context during execution", [&](){
        call_describe();
        AssertThat(context_stack_size_while_running, Equals(2));
      });

      it("pops the context from the stack after execution so that only the global context is left", [&](){
          call_describe();
          AssertThat(*context_stack, Is().OfLength(1));
      });

    });

    describe("with test run error", [&](){
        //
        // This can occur if after_each or before_each are called
        // after execution has started for a context.
        // 
    
      before_each([&](){
        describe_fn = [&](){ throw bandit::test_run_error("we dun goofed!"); };
      });

      it("doesn't propagate the error", [&](){
        call_describe();
      });

      it("tells reporter to report the error", [&](){
        call_describe();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("test_run_error: context name (we dun goofed!)"));
      });
    
    });

    describe("skip", [&](){
    
      it("pushes a context marked as skipped on the stack", [&](){
        bool is_skipped_while_running_context;
        describe_skip("context name", 
          [&](){ is_skipped_while_running_context = context_stack->back()->is_skipped(); },
          *reporter, *context_stack);
        AssertThat(is_skipped_while_running_context, IsTrue());
      });
    
    });
  });

});
