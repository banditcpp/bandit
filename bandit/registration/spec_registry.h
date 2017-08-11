#ifndef BANDIT_SPEC_REGISTRY_H
#define BANDIT_SPEC_REGISTRY_H

#include <list>
#include <functional>

namespace bandit {
  namespace detail {
    typedef std::list<std::function<void()>> spec_registry;

    inline detail::spec_registry& specs() {
      static detail::spec_registry registry;
      return registry;
    }
  }
}
#endif
