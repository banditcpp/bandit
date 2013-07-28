#ifndef BANDIT_RUNNER_H
#define BANDIT_RUNNER_H

namespace bandit {

  namespace detail {

    inline reporter_ptr create_reporter(const options& opt,
        const failure_formatter* formatter)
    {
      std::string name(opt.reporter() ? opt.reporter() : "");

      if(name == "singleline")
      {
        return unique_ptr<reporter>(new single_line_reporter(*formatter));
      }

      return unique_ptr<reporter>(new dots_reporter(*formatter));
    }

    typedef std::function<reporter_ptr (const std::string&, const failure_formatter*)> reporter_factory_fn;
    typedef std::function<reporter* (reporter*)> register_reporter_fn;
  }

  inline int run(const options& opt, const detail::spec_registry& specs,
      contextstack_t& context_stack, 
      reporter& reporter)
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

    reporter.test_run_starting();

    bandit_context global_context;
    context_stack.push_back(&global_context);

    for_each(specs.begin(), specs.end(), call_func);

    reporter.test_run_complete();

    return reporter.did_we_pass() ? 0 : 1;
  }

  inline int run(int argc, char* argv[])
  {
    options opt(argc, argv);
    failure_formatter_ptr formatter(new default_failure_formatter());
    reporter_ptr reporter(create_reporter(opt, formatter.get()));

    default_reporter(reporter.get());

    return run(opt, detail::specs(), context_stack(), *(reporter.get()));
  }
}

#endif
