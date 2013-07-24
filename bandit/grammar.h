#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

namespace bandit {

  using namespace std;
  using namespace detail;

  inline void describe(const char* desc, voidfunc_t func, reporter& reporter)
  {
    reporter.context_starting(desc);

    context ctxt;
    context_stack().push_back(&ctxt);
    func();
    context_stack().pop_back();

    reporter.context_ended(desc);
  }

  inline void describe(const char* desc, voidfunc_t func)
  {
    describe(desc, func, default_reporter());
  }

  inline void before_each(voidfunc_t func)
  {
    context_stack().back()->register_before_each(func);
  }

  inline void it(const char* desc, voidfunc_t func, reporter& reporter,
      contextstack_t& context_stack, std::function<void (voidfunc_t)> assertion_exception_adapter)
  {
    reporter.it_starting(desc);

    try
    {
      assertion_exception_adapter([&](){
          for_each(context_stack.begin(), 
            context_stack.end(), 
            [](context* ctxt){
            ctxt->run_before_eaches();
            });

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

  }

  inline void it(const char* desc, voidfunc_t func)
  {
    it(desc, func, default_reporter(), context_stack(), registered_adapter());
  }

}

#endif
