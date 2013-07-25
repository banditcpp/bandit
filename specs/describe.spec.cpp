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
        describe("something", describe_fn, *(reporter.get()), *(context_stack.get()));
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
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_starting: something"));
      });

      it("tells reporter it's finished a run", [&](){
        call_describe();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("context_ended: something"));
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


  });

});
