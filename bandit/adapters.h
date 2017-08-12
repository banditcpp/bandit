#ifndef BANDIT_ADAPTERS_H
#define BANDIT_ADAPTERS_H

#include <bandit/adapters/snowhouse.h>

namespace bandit {
  namespace detail {
    inline assertion_adapter_t& registered_adapter() {
      static adapter::snowhouse default_adapter;
      return default_adapter;
    }
  }
}
#endif
