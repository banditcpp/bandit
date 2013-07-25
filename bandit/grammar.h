#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

namespace bandit {

  using namespace std;
  using namespace detail;

  inline void describe(const char* desc, voidfunc_t func, reporter& reporter, contextstack_t& context_stack)
  {
    reporter.context_starting(desc);

    context_stack.back()->execution_is_starting();

    bandit_context ctxt;
    context_stack.push_back(&ctxt);
    try
    {
      func();
    }
    catch(const test_run_error& error)
    {
      reporter.test_run_error(desc, error);
    }

    context_stack.pop_back();

    reporter.context_ended(desc);
  }

  inline void describe(const char* desc, voidfunc_t func)
  {
    describe(desc, func, default_reporter(), context_stack());
  }

  inline void before_each(voidfunc_t func, contextstack_t& context_stack)
  {
    context_stack.back()->register_before_each(func);
  }

  inline void before_each(voidfunc_t func)
  {
    before_each(func, context_stack());
  }

  inline void after_each(voidfunc_t func, contextstack_t& context_stack)
  {
    context_stack.back()->register_after_each(func);
  }

  inline void after_each(voidfunc_t func)
  {
    after_each(func, context_stack());
  }

  inline void it(const char* desc, voidfunc_t func, reporter& reporter,
      contextstack_t& context_stack, std::function<void (voidfunc_t)> assertion_exception_adapter)
  {
    reporter.it_starting(desc);

    context_stack.back()->execution_is_starting();

    auto run_before_eaches = [&](){
      for_each(context_stack.begin(), context_stack.end(), [](context* ctxt){
          ctxt->run_before_eaches();
      });
    };

    auto run_after_eaches = [&](){
      for_each(context_stack.begin(), context_stack.end(), [](context* ctxt){
          ctxt->run_after_eaches();
      });
    };

    try
    {
      assertion_exception_adapter([&](){
          run_before_eaches();

          func();
          reporter.it_succeeded(desc);
      });
    }
    catch(const bandit::assertion_exception& ex)
    {
      reporter.it_failed(desc, ex);
    }
    catch(...)
    {
      reporter.it_unknown_error(desc);
    }

    try
    {
      run_after_eaches();
    }
    catch(...)
    {
      reporter.it_unknown_error(desc);
    }
  }

  inline void it(const char* desc, voidfunc_t func)
  {
    it(desc, func, default_reporter(), context_stack(), registered_adapter());
  }

}

#endif
