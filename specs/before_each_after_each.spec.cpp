#include <specs/specs.h>

namespace bf = bandit::fakes;

go_bandit([](){

    describe("before_each/after_each", [&](){
      std::unique_ptr<bandit::detail::contextstack_t> context_stack;
      std::unique_ptr<bf::fake_context> context;
    
      before_each([&](){
        context = std::unique_ptr<bf::fake_context>(new bf::fake_context());
        context_stack = std::unique_ptr<bandit::detail::contextstack_t>(new bandit::detail::contextstack_t());
        context_stack->push_back(context.get());
      });
      
      describe("before_each", [&](){
        bandit::detail::voidfunc_t before_each_fn;

        before_each([&](){
          before_each_fn = [](){};
          });

        it("registers itself for the current context in the stack", [&](){
          before_each(before_each_fn, *context_stack); 
          Assert::That(context->call_log(), Has().Exactly(1).EqualTo("register_before_each"));
          });

      }); 

      describe("after_each", [&](){
          bandit::detail::voidfunc_t after_each_fn;
        
        before_each([&](){
          after_each_fn = [](){};
        });
      
        it("registers itself for the current context in the stack", [&](){
          after_each(after_each_fn, *context_stack); 
          Assert::That(context->call_log(), Has().Exactly(1).EqualTo("register_after_each"));
        });
      
      });
    });

    describe("before_each/after_each integration", [&](){
        bandit::specs::logging_fake logger;

      before_each([&](){
        logger.log() << "first before_each called" << std::endl;
      });

      before_each([&](){
        logger.log() << "second before_each called" << std::endl;
      });

      after_each([&](){
        logger.log() << "first after_each called" << std::endl;
      });

      after_each([&](){
        logger.log() << "second after_each called" << std::endl;
      });

      it("should only have called the before_each functions for the first test", [&](){
        Assert::That(logger.call_log(), Has().Exactly(1).EqualTo("first before_each called"));
        Assert::That(logger.call_log(), Has().Exactly(1).EqualTo("second before_each called"));
        Assert::That(logger.call_log(), Has().None().Containing("after_each"));
      });

      it("should have called 'before_each' function twice, and 'after_each' functions once for the second test", [&](){
        Assert::That(logger.call_log(), Has().Exactly(2).EqualTo("first before_each called"));
        Assert::That(logger.call_log(), Has().Exactly(2).EqualTo("second before_each called"));
        Assert::That(logger.call_log(), Has().Exactly(1).EqualTo("first after_each called"));
        Assert::That(logger.call_log(), Has().Exactly(1).EqualTo("second after_each called"));
      });
    });
});
