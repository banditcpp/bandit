#include <specs/specs.h>

using namespace bandit::fakes;

go_bandit([](){

  describe("describe:", [](){
    bandit::detail::voidfunc_t describe_fn;
    fake_reporter_ptr reporter;
    unique_ptr<contextstack_t> context_stack;

    before_each([&](){
      reporter = fake_reporter_ptr(new fake_reporter());
      context_stack = unique_ptr<contextstack_t>(new contextstack_t());
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
        AssertThat(context_stack_size_while_running, Equals(1));
      });

      it("pops the context from the stack after execution", [&](){
          call_describe();
          AssertThat(*context_stack, Is().Empty());
      });

    });


  });

});
