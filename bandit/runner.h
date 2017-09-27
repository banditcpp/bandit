#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

#include <bandit/options.h>
#include <bandit/registration/registrar.h>
#include <bandit/reporters.h>
#include <bandit/failure_formatters.h>
#include <bandit/run_policies.h>
#include <bandit/version.h>

namespace bandit {
  namespace detail {
    struct settings {
      colorizer_ptr colorizer;
      failure_formatter_ptr formatter;
      reporter_ptr reporter;
      run_policy_ptr run_policy;

      settings(const options& opt)
          : colorizer(create_colorizer(opt)),
            formatter(create_formatter(opt)),
            reporter(create_reporter(opt, *formatter, *colorizer)),
            run_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run())) {}

    private:
      static inline colorizer_ptr create_colorizer(const options& opt) {
        switch (opt.colorizer()) {
        case options::colorizers::OFF:
          return colorizer_ptr(new colorizer::off());
        case options::colorizers::DARK:
          return colorizer_ptr(new colorizer::dark());
        case options::colorizers::LIGHT:
        default:
          return colorizer_ptr(new colorizer::light());
        }
      }

      static inline failure_formatter_ptr create_formatter(const options& opt) {
        switch (opt.formatter()) {
        case options::formatters::VS:
          return failure_formatter_ptr(new failure_formatter::visual_studio());
        case options::formatters::POSIX:
        default:
          return failure_formatter_ptr(new failure_formatter::posix());
        }
      }

      static inline reporter_ptr create_reporter(const options& opt,
          const failure_formatter_t& formatter, const colorizer_t& colorizer) {
        switch (opt.reporter()) {
        case options::reporters::SINGLELINE:
          return reporter_ptr(new bandit::reporter::singleline(formatter, colorizer));
        case options::reporters::XUNIT:
          return reporter_ptr(new bandit::reporter::xunit(formatter));
        case options::reporters::INFO:
          return reporter_ptr(new bandit::reporter::info(formatter, colorizer));
        case options::reporters::SPEC:
          return reporter_ptr(new bandit::reporter::spec(formatter, colorizer));
        case options::reporters::CRASH:
          return reporter_ptr(new bandit::reporter::crash(formatter));
        case options::reporters::DOTS:
        default:
          return reporter_ptr(new bandit::reporter::dots(formatter, colorizer));
        }
      }
    };

    using reporter_factory_fn = std::function<reporter_ptr(const std::string&, const failure_formatter_t*)>;
    using register_reporter_fn = std::function<detail::reporter_t*(detail::reporter_t*)>;
  }

  inline int run(const detail::options& opt, const detail::spec_registry& specs,
      context::stack_t& context_stack, detail::reporter_t& reporter) {
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

    detail::settings settings(opt);
    detail::register_reporter(settings.reporter.get());
    detail::register_run_policy(settings.run_policy.get());

    return run(opt, detail::specs(), context::stack(), *settings.reporter);
  }
}
#endif
