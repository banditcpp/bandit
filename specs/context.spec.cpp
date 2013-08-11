#include <specs/specs.h>

go_bandit([](){

  describe("bandit_context:", [&](){
  
    std::unique_ptr<bandit::detail::bandit_context> context;

    before_each([&](){
      bool hard_skip = false;
      context = std::unique_ptr<bandit::detail::bandit_context>(
        new bandit::detail::bandit_context("context name", hard_skip));
    });

    it("is ok to register before_each as it is not executing", [&](){
      context->register_before_each([](){});
    });

    it("is ok to register after_each as it is not executing", [&](){
      context->register_after_each([](){});
    });

    describe("is executing", [&](){
    
      before_each([&](){
        context->execution_is_starting();
      });
    
      it("is not ok to register before_each", [&](){
        AssertThrows(bandit::detail::test_run_error, context->register_before_each([](){}));
        Assert::That(LastException<bandit::detail::test_run_error>().what(), 
          Equals("before_each was called after 'describe' or 'it'"));
      });

      it("is not ok to register after_each", [&](){
        AssertThrows(bandit::detail::test_run_error, context->register_after_each([](){}));
        Assert::That(LastException<bandit::detail::test_run_error>().what(), 
         Equals("after_each was called after 'describe' or 'it'"));
      });
    });
  
  });

});
