#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

#include <bandit/options.h>
#include <bandit/registration/registrar.h>
#include <bandit/reporters/reporters.h>
#include <bandit/failure_formatters/failure_formatters.h>
#include <bandit/run_policies/run_policies.h>
#include <bandit/version.h>

namespace bandit {

  namespace detail {

    inline run_policy_ptr create_run_policy(const options& opt)
    {
      return run_policy_ptr(new bandit_run_policy(opt.skip(), opt.only(), opt.break_on_failure(), opt.dry_run()));
    }

    inline listener_ptr create_reporter(const options& opt,
        const failure_formatter* formatter, const colorizer& colorizer)
    {
      std::string name(opt.reporter() ? opt.reporter() : "");

      if(name == "singleline")
      {
        return std::unique_ptr<detail::listener>(new single_line_reporter(*formatter, colorizer));
      }

      if(name == "xunit")
      {
        return std::unique_ptr<detail::listener>(new xunit_reporter(*formatter));
      }

      if(name == "info")
      {
        return std::unique_ptr<detail::listener>(new info_reporter(*formatter, colorizer));
      }

      if(name == "spec")
      {
        return std::unique_ptr<detail::listener>(new spec_reporter(*formatter, colorizer));
      }

      return std::unique_ptr<detail::listener>(new dots_reporter(*formatter, colorizer));
    }

    typedef std::function<listener_ptr (const std::string&, const failure_formatter*)> reporter_factory_fn;
    typedef std::function<detail::listener* (detail::listener*)> register_reporter_fn;

    inline failure_formatter_ptr create_formatter(const options& opt)
    {
      if(opt.formatter() == options::formatters::FORMATTER_VS)
      {
        return failure_formatter_ptr(new visual_studio_failure_formatter());
      }

      return failure_formatter_ptr(new default_failure_formatter());
    }
  }

  inline int run(const detail::options& opt, const detail::spec_registry& specs,
      detail::contextstack_t& context_stack, detail::listener& listener)
  {
    if(opt.help() || !opt.parsed_ok())
    {
      opt.print_usage();
      return !opt.parsed_ok();
    }

    if(opt.version())
    {
      std::cout << "bandit version " << BANDIT_VERSION << std::endl;
      return 0;
    }

    auto call_func = [](const detail::voidfunc_t& func) {
      func();
    };

    listener.test_run_starting();

    bool hard_skip = false;
    detail::bandit_context global_context("", hard_skip);
    context_stack.push_back(&global_context);

    for_each(specs.begin(), specs.end(), call_func);

    listener.test_run_complete();

    return listener.did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[], int allow_further = true)
  {
    detail::options opt(argc, argv);
    if (!allow_further &&
        (opt.has_further_arguments() || opt.has_unknown_options())) {
      opt.print_usage();
      return 1;
    }
    detail::failure_formatter_ptr formatter(create_formatter(opt));
    bandit::detail::colorizer colorizer(!opt.no_color());
    detail::listener_ptr reporter(create_reporter(opt, formatter.get(), colorizer));

    detail::register_listener(reporter.get());

    detail::run_policy_ptr run_policy = create_run_policy(opt);
    register_run_policy(run_policy.get());

    return run(opt, detail::specs(), detail::context_stack(), *reporter);
  }
}

#endif
