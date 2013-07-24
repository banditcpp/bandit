#ifndef BANDIT_ADAPTERS_SNOWHOUSE_H
#define BANDIT_ADAPTERS_SNOWHOUSE_H

namespace bandit { namespace adapters {

  auto snowhouse = [](detail::voidfunc_t func)
  {
    try
    {
      func();
    }
    catch(const snowhouse::AssertionException& ex)
    {
      throw assertion_exception(ex.GetMessage(), ex.GetFilename(), ex.GetLineNumber());
    }
  };

}}
#endif
