#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  inline int run(int /*argc*/, char* /*argv*/[])
  {
    for_each(bandit::detail::specs().begin(),
        bandit::detail::specs().end(),
        [](voidfunc_t& func){
          func();
        });
    return 0;
  }
}

#endif
