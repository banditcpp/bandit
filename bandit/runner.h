#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  inline int run(int /*argc*/, char* /*argv*/[], const detail::spec_registry& specs,
      reporter& reporter, contextstack_t& context_stack)
  {
    auto call_func = [](const voidfunc_t& func) {
      func();
    };

    reporter.test_run_starting();

    bandit_context global_context;
    context_stack.push_back(&global_context);

    for_each(specs.begin(), specs.end(), call_func);

    reporter.test_run_complete();

    return reporter.did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[])
  {
    return run(argc, argv, detail::specs(), default_reporter(), context_stack());
  }
}

#endif
