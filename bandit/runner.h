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
    detail::choice_options choices;

    choices.colorizers.add("off", [&](detail::settings_t& settings) {
      settings.set_colorizer(new colorizer::off());
    });
    choices.colorizers.add("dark", [&](detail::settings_t& settings) {
      settings.set_colorizer(new colorizer::dark());
    });
    choices.colorizers.add("light", [&](detail::settings_t& settings) {
      settings.set_colorizer(new colorizer::light());
    }, true);
    choices.formatters.add("vs", [&](detail::settings_t& settings) {
      settings.set_formatter(new failure_formatter::visual_studio());
    });
    choices.formatters.add("posix", [&](detail::settings_t& settings) {
      settings.set_formatter(new failure_formatter::posix());
    }, true);
    choices.reporters.add("singleline", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::singleline(settings.get_formatter(), settings.get_colorizer()));
    });
    choices.reporters.add("xunit", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::xunit(settings.get_formatter()));
    });
    choices.reporters.add("info", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::info(settings.get_formatter(), settings.get_colorizer()));
    });
    choices.reporters.add("spec", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::spec(settings.get_formatter(), settings.get_colorizer()));
    });
    choices.reporters.add("crash", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::crash(settings.get_formatter()));
    });
    choices.reporters.add("dots", [&](detail::settings_t& settings) {
      settings.set_reporter(new bandit::reporter::dots(settings.get_formatter(), settings.get_colorizer()));
    }, true);

    detail::options opt(argc, argv, choices);

    if (!allow_further &&
        (opt.has_further_arguments() || opt.has_unknown_options())) {
      opt.print_usage();
      return 1;
    }

    detail::settings_t settings;

    // XXX: It would be nice to have that moved into bandit::options::arguments
    if (!opt.apply_colorizer(settings)) {
      opt.print_usage_colorizer();
      return 1;
    }
    if (!opt.apply_formatter(settings)) {
      opt.print_usage_formatter();
      return 1;
    }
    if (!opt.apply_reporter(settings)) {
      opt.print_usage_reporter();
      return 1;
    }

    settings.set_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run()));

    detail::register_settings(&settings);
    return run(opt, detail::specs());
  }
}
#endif
