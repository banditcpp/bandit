#ifndef BANDIT_REGISTAR_H
#define BANDIT_REGISTAR_H

namespace bandit { namespace detail {

    struct spec_registar
    {
      spec_registar(bandit::detail::spec_registry& registry,
          bandit::detail::voidfunc_t func)
      {
        registry.push_back(func);
      }
    };

}}

#define run_bandit(context) \
  static bandit::detail::spec_registar bandit_registrar(bandit::detail::specs(), context);

#endif
