#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  inline int run(int /*argc*/, char* /*argv*/[], const detail::spec_registry& specs, reporter& reporter)
  {
    auto call_func = [](const voidfunc_t& func) {
      func();
    };

    reporter.test_run_starting();

    for_each(specs.begin(), specs.end(), call_func);

    reporter.test_run_complete();

    return reporter.number_of_failed_tests();
  }

  inline int run(int argc, char* argv[])
  {
    return run(argc, argv, detail::specs(), default_reporter());
  }
}

#endif
