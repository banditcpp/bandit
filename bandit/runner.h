#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  namespace detail {

    inline run_policy_ptr create_run_policy(const options& opt)
    {
      return run_policy_ptr(new bandit_run_policy(opt.skip(), opt.only()));
    }

    inline listener_ptr create_reporter(const options& opt,
        const failure_formatter* formatter, const colorizer& colorizer)
    {
      std::string name(opt.reporter() ? opt.reporter() : "");

      if(name == "singleline")
      {
        return unique_ptr<listener>(new single_line_reporter(*formatter, colorizer));
      }

      if(name == "xunit")
      {
        return unique_ptr<listener>(new xunit_reporter(*formatter));
      }

      return unique_ptr<listener>(new dots_reporter(*formatter, colorizer));
    }

    typedef std::function<listener_ptr (const std::string&, const failure_formatter*)> reporter_factory_fn;
    typedef std::function<listener* (listener*)> register_reporter_fn;

    inline failure_formatter_ptr create_formatter(const options& opt)
    {
      if(opt.formatter() == options::formatters::FORMATTER_VS)
      {
        return failure_formatter_ptr(new visual_studio_failure_formatter());
      }

      return failure_formatter_ptr(new default_failure_formatter());
    }
  }

  inline int run(const options& opt, const detail::spec_registry& specs,
      contextstack_t& context_stack, listener& listener)
  {
    if(opt.help())
    {
      opt.print_usage();
      return 0;
    }

    if(opt.version())
    {
      std::cout << "bandit version " << BANDIT_VERSION << std::endl;
      return 0;
    }

    auto call_func = [](const voidfunc_t& func) {
      func();
    };

    listener.test_run_starting();

    bool hard_skip = false;
    bandit_context global_context("", hard_skip);
    context_stack.push_back(&global_context);

    for_each(specs.begin(), specs.end(), call_func);

    listener.test_run_complete();

    return listener.did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[])
  {
    options opt(argc, argv);
    failure_formatter_ptr formatter(create_formatter(opt));
    bandit::detail::colorizer colorizer(!opt.no_color());
    listener_ptr reporter(create_reporter(opt, formatter.get(), colorizer));

    registered_listener(reporter.get());

    run_policy_ptr run_policy = create_run_policy(opt);
    registered_run_policy(run_policy.get());

    return run(opt, detail::specs(), context_stack(), *reporter);
  }
}

#endif
