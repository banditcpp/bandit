#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

#include <bandit/registration/registrar.h>
#include <bandit/settings.h>
#include <bandit/version.h>

namespace bandit {
  inline int run(const detail::options& opt, const detail::spec_registry& specs,
      context::stack_t& context_stack,
      detail::reporter_t& reporter = detail::registered_reporter()) {
    if (opt.help() || !opt.parsed_ok()) {
      opt.print_usage();
      return !opt.parsed_ok();
    }

    if (opt.version()) {
      std::cout << "bandit version " << BANDIT_VERSION << std::endl;
      return 0;
    }

    reporter.test_run_starting();

    bool hard_skip = false;
    context::bandit global_context("", hard_skip);
    context_stack.push_back(&global_context);

    for (auto func : specs) {
      func();
    };

    reporter.test_run_complete();

    return reporter.did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[], bool allow_further = true) {
    detail::options opt(argc, argv);
    if (!allow_further &&
        (opt.has_further_arguments() || opt.has_unknown_options())) {
      opt.print_usage();
      return 1;
    }

    detail::settings_t settings(opt);
    detail::register_reporter(settings.reporter.get());
    detail::register_run_policy(settings.run_policy.get());

    return run(opt, detail::specs(), context::stack());
  }
}
#endif
