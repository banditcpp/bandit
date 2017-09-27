#ifndef BANDIT_ADAPTERS_INTERFACE_H
#define BANDIT_ADAPTERS_INTERFACE_H

#include <functional>
#include <memory>

namespace bandit {
  namespace adapter {
    struct interface {
      virtual void adapt_exceptions(std::function<void()>) = 0;
    };
  }

  namespace detail {
    using assertion_adapter_t = adapter::interface;
    using assertion_adapter_ptr = std::unique_ptr<assertion_adapter_t>;
  }
}
#endif
