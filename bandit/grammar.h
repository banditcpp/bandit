#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

namespace bandit {

  using namespace std;
  using namespace detail;

  inline void describe(const char* desc, voidfunc_t func)
  {
    cout << "Running \"" << desc << "\"" << endl;
    context ctxt;
    context_stack().push_back(&ctxt);
    func();
    context_stack().pop_back();
    cout << "Finished running context \"" << desc << "\"" << endl;
  }

  inline void before_each(voidfunc_t func)
  {
    context_stack().back()->register_before_each(func);
  }

  inline void it(const char* , voidfunc_t func)
  {
    for_each(context_stack().begin(), context_stack().end(), [](context* ctxt){
        ctxt->run_before_eaches();
        });
    func();
  }

}

#endif
