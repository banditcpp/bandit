#ifndef BANDIT_SPEC_REGISTRY_H
#define BANDIT_SPEC_REGISTRY_H

#include <list>
#include <functional>

namespace bandit {
  namespace detail {
    using spec_registry = std::list<std::function<void()>>;

    inline detail::spec_registry& specs() {
      static detail::spec_registry registry;
      return registry;
    }
  }
}
#endif
