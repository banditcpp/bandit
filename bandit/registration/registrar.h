#ifndef BANDIT_REGISTRAR_H
#define BANDIT_REGISTRAR_H

namespace bandit { namespace detail {

    struct spec_registrar
    {
      spec_registrar(bandit::detail::spec_registry& registry,
          bandit::detail::voidfunc_t func)
      {
        registry.push_back(func);
      }
    };

}}

#define go_bandit(context) \
  static bandit::detail::spec_registrar bandit_registrar(bandit::detail::specs(), context);

#endif
