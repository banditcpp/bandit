#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  inline int run(int /*argc*/, char* /*argv*/[], const detail::spec_registry& specs)
  {
    auto call_func = [](const voidfunc_t& func) {
      func();
    };

    for_each(specs.begin(), specs.end(), call_func);

    return 0;
  }

  inline int run(int argc, char* argv[])
  {
    return run(argc, argv, detail::specs());
  }
}

#endif
