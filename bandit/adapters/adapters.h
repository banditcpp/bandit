#ifndef BANDIT_ADAPTERS_H
#define BANDIT_ADAPTERS_H

#include <bandit/adapters/snowhouse.h>

namespace bandit {

  inline std::function<void (detail::voidfunc_t)> registered_adapter()
  {
    static std::function<void (detail::voidfunc_t)> adapter = bandit::adapters::snowhouse;
    return adapter;
  }
}

#endif
