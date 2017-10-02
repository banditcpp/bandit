#ifndef BANDIT_SETTINGS_H
#define BANDIT_SETTINGS_H

#include <bandit/adapters.h>
#include <bandit/colorizers.h>
#include <bandit/failure_formatters.h>
#include <bandit/options.h>
#include <bandit/reporters.h>
#include <bandit/run_policies.h>

namespace bandit {
  namespace detail {
    struct settings_t {
      settings_t(const options& opt)
          : adapter(new adapter::snowhouse),
            colorizer(create_colorizer(opt)),
            formatter(create_formatter(opt)),
            reporter(create_reporter(opt)),
            run_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run())) {}

      settings_t() : settings_t(options(0, nullptr)) {}

      context::stack_t& get_contexts() {
        return context_stack;
      }

      assertion_adapter_t& get_adapter() {
        return *adapter;
      }

      void set_adapter(assertion_adapter_t* adapter_) {
        adapter.reset(adapter_);
      }

      reporter_t& get_reporter() {
        return *reporter;
      }

      void set_reporter(reporter_t* reporter_) {
        reporter.reset(reporter_);
      }

      run_policy_t& get_policy() {
        return *run_policy;
      }

      void set_policy(run_policy_t* run_policy_) {
        run_policy.reset(run_policy_);
      }

      void describe(const std::string& desc, std::function<void()> func, bool hard_skip) {
        reporter->context_starting(desc);

        context_stack.back()->execution_is_starting();

        context::bandit ctxt(desc, hard_skip);
        context_stack.push_back(&ctxt);

        try {
          func();
        } catch (const bandit::detail::test_run_error& error) {
          reporter->test_run_error(desc, error);
        }

        context_stack.pop_back();

        reporter->context_ended(desc);
      }

      void before_each(std::function<void()> func) {
        context_stack.throw_if_empty("before_each");
        context_stack.back()->register_before_each(func);
      }

      void after_each(std::function<void()> func) {
        context_stack.throw_if_empty("after_each");
        context_stack.back()->register_after_each(func);
      }

      void it(const std::string& desc, std::function<void()> func, bool hard_skip) {
        context_stack.throw_if_empty("it");
        if (hard_skip || !run_policy->should_run(desc, context_stack)) {
          reporter->it_skip(desc);
        } else {
          reporter->it_starting(desc);
          context_stack.back()->execution_is_starting();

          bool success = false;
          context::interface* last_successful_before_each_context = nullptr;
          try_with_adapter(desc, true, [&] {
            for (auto context : context_stack) {
              context->run_before_eaches();
              last_successful_before_each_context = context;
            }

            func();
            success = true;
          });

          try_with_adapter(desc, success, [&] {
            bool do_run_after_each = false;
            std::for_each(context_stack.rbegin(), context_stack.rend(), [&](context::interface* context) {
              if (context == last_successful_before_each_context) {
                do_run_after_each = true;
              }
              if (do_run_after_each) {
                context->run_after_eaches();
              }
            });

            if (success) {
              reporter->it_succeeded(desc);
            }
          });
        }
      }

      // A function is required to initialize a static settings variable in a header file
      // and this struct aims at encapsulating this function
      static void register_settings(settings_t* settings) {
        if (settings == nullptr) {
          throw std::runtime_error("Invalid null settings passed to "
                                   "bandit::detail::register_settings");
        }
        get_settings_address() = settings;
      }

      static settings_t& registered_settings() {
        auto settings = get_settings_address();
        if (settings == nullptr) {
          throw std::runtime_error("No settings set. Please call "
                                   "bandit::detail::register_settings with a non-null settings");
        }
        return *settings;
      }

    private:
      static settings_t*& get_settings_address() {
        static settings_t* settings_ = nullptr;
        return settings_;
      }

      void try_with_adapter(const std::string& desc, bool allow_fail, std::function<void()> do_it) {
        if (allow_fail) {
          try {
            adapter->adapt_exceptions([&] { do_it(); });
          } catch (const bandit::detail::assertion_exception& ex) {
            reporter->it_failed(desc, ex);
            run_policy->encountered_failure();
          } catch (const std::exception& ex) {
            std::string err = std::string("exception: ") + ex.what();
            reporter->it_failed(desc, bandit::detail::assertion_exception(err));
            run_policy->encountered_failure();
          } catch (...) {
            reporter->it_unknown_error(desc);
            run_policy->encountered_failure();
          }
        } else {
          try {
            do_it();
          } catch (...) {
            /* ignore */
          }
        }
      }

      colorizer_ptr create_colorizer(const options& opt) const {
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

      failure_formatter_ptr create_formatter(const options& opt) const {
        switch (opt.formatter()) {
        case options::formatters::VS:
          return failure_formatter_ptr(new failure_formatter::visual_studio());
        case options::formatters::POSIX:
        default:
          return failure_formatter_ptr(new failure_formatter::posix());
        }
      }

      reporter_ptr create_reporter(const options& opt) const {
        switch (opt.reporter()) {
        case options::reporters::SINGLELINE:
          return reporter_ptr(new bandit::reporter::singleline(*formatter, *colorizer));
        case options::reporters::XUNIT:
          return reporter_ptr(new bandit::reporter::xunit(*formatter));
        case options::reporters::INFO:
          return reporter_ptr(new bandit::reporter::info(*formatter, *colorizer));
        case options::reporters::SPEC:
          return reporter_ptr(new bandit::reporter::spec(*formatter, *colorizer));
        case options::reporters::CRASH:
          return reporter_ptr(new bandit::reporter::crash(*formatter));
        case options::reporters::DOTS:
        default:
          return reporter_ptr(new bandit::reporter::dots(*formatter, *colorizer));
        }
      }

      context::stack_t context_stack;
      assertion_adapter_ptr adapter;
      colorizer_ptr colorizer;
      failure_formatter_ptr formatter;
      reporter_ptr reporter;
      run_policy_ptr run_policy;
    };

    inline void register_settings(settings_t* settings) {
      settings_t::register_settings(settings);
    }

    inline settings_t& registered_settings() {
      return settings_t::registered_settings();
    }
  }
}
#endif
