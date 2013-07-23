#ifndef BANDIT_SPEC_REGISTRY_H
#define BANDIT_SPEC_REGISTRY_H

namespace bandit {
  namespace detail {

    struct spec_registry
    {
      void push_back(voidfunc_t spec)
      {
        specs_.push_back(spec);
      }

      int run(int /*argc*/, char* /*argv*/[])
      {
        for_each(specs_.begin(), specs_.end(), [](voidfunc_t& func){
            func();
            });
        return 0;
      }

      private:
      std::list<voidfunc_t> specs_;
    };

  }

  inline detail::spec_registry& specs()
  {
    static detail::spec_registry registry;
    return registry;
  }
}

#endif
