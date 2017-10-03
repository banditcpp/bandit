#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

#include <bandit/registration/registrar.h>
#include <bandit/options.h>
#include <bandit/settings.h>
#include <bandit/version.h>

#include <bandit/colorizers.h>
#include <bandit/failure_formatters.h>
#include <bandit/reporters.h>
#include <bandit/run_policies.h>

namespace bandit {
  inline int run(const detail::options& opt, const detail::spec_registry& specs,
      detail::settings_t& settings = detail::registered_settings()) {
    if (opt.help() || !opt.parsed_ok()) {
      opt.print_usage();
      return !opt.parsed_ok();
    }

    if (opt.version()) {
      std::cout << "bandit version " << BANDIT_VERSION << std::endl;
      return 0;
    }

    settings.get_reporter().test_run_starting();

    bool hard_skip = false;
    context::bandit global_context("", hard_skip);
    settings.get_contexts().push_back(&global_context);

    for (auto func : specs) {
      func();
    };

    settings.get_reporter().test_run_complete();

    return settings.get_reporter().did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[], bool allow_further = true) {
    detail::options opt(argc, argv);
    if (!allow_further &&
        (opt.has_further_arguments() || opt.has_unknown_options())) {
      opt.print_usage();
      return 1;
    }

    detail::settings_t settings;
    switch (opt.colorizer()) {
    case detail::options::colorizers::OFF:
      settings.set_colorizer(new colorizer::off());
      break;
    case detail::options::colorizers::DARK:
      settings.set_colorizer(new colorizer::dark());
      break;
    case detail::options::colorizers::LIGHT:
    default:
      settings.set_colorizer(new colorizer::light());
    }

    switch (opt.formatter()) {
    case detail::options::formatters::VS:
      settings.set_formatter(new failure_formatter::visual_studio());
      break;
    case detail::options::formatters::POSIX:
    default:
      settings.set_formatter(new failure_formatter::posix());
    }

    switch (opt.reporter()) {
    case detail::options::reporters::SINGLELINE:
      settings.set_reporter(new bandit::reporter::singleline(settings.get_formatter(), settings.get_colorizer()));
      break;
    case detail::options::reporters::XUNIT:
      settings.set_reporter(new bandit::reporter::xunit(settings.get_formatter()));
      break;
    case detail::options::reporters::INFO:
      settings.set_reporter(new bandit::reporter::info(settings.get_formatter(), settings.get_colorizer()));
      break;
    case detail::options::reporters::SPEC:
      settings.set_reporter(new bandit::reporter::spec(settings.get_formatter(), settings.get_colorizer()));
      break;
    case detail::options::reporters::CRASH:
      settings.set_reporter(new bandit::reporter::crash(settings.get_formatter()));
      break;
    case detail::options::reporters::DOTS:
    default:
      settings.set_reporter(new bandit::reporter::dots(settings.get_formatter(), settings.get_colorizer()));
    }

    settings.set_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run()));

    detail::register_settings(&settings);
    return run(opt, detail::specs());
  }
}
#endif
