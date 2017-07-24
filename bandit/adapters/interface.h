#ifndef BANDIT_ADAPTERS_INTERFACE_H
#define BANDIT_ADAPTERS_INTERFACE_H

#include <bandit/types.h>

namespace bandit {
  namespace adapter {
    struct interface {
      virtual void adapt_exceptions(detail::voidfunc_t) = 0;
    };
  }

  namespace detail {
    using assertion_adapter_t = adapter::interface;
  }
}
#endif
