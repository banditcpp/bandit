#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit { namespace detail {
  struct spec_runner
  {
    spec_runner(bandit::detail::voidfunc_t func)
    {
      func();
    }
  };
}}

#define run_bandit(context) \
  static bandit::detail::spec_runner bandit_runner(context);

#endif
