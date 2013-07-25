#include <specs/specs.h>

namespace bf = bandit::fakes;

go_bandit([](){

    describe("before_each/after_each", [&](){
      unique_ptr<contextstack_t> context_stack;
      unique_ptr<bf::fake_context> context;
    
      before_each([&](){
        context = unique_ptr<bf::fake_context>(new bf::fake_context());
        context_stack = unique_ptr<contextstack_t>(new contextstack_t());
        context_stack->push_back(context.get());
      });
      
      describe("before_each", [&](){
        voidfunc_t before_each_fn;

        before_each([&](){
          before_each_fn = [](){};
          });

        it("registers itself for the current context in the stack", [&](){
          before_each(before_each_fn, *(context_stack.get())); 
          AssertThat(context->call_log(), Has().Exactly(1).EqualTo("register_before_each"));
          });

      }); 

      describe("after_each", [&](){
        voidfunc_t after_each_fn;
        
        before_each([&](){
          after_each_fn = [](){};
        });
      
        it("registers itself for the current context in the stack", [&](){
          after_each(after_each_fn, *(context_stack.get())); 
          AssertThat(context->call_log(), Has().Exactly(1).EqualTo("register_after_each"));
        });
      
      });
    });
});
