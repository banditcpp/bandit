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
      assertion_adapter_ptr adapter;
      colorizer_ptr colorizer;
      failure_formatter_ptr formatter;
      reporter_ptr reporter;
      run_policy_ptr run_policy;

      settings_t(const options& opt)
          : adapter(new adapter::snowhouse),
            colorizer(create_colorizer(opt)),
            formatter(create_formatter(opt)),
            reporter(create_reporter(opt)),
            run_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run())) {}

      assertion_adapter_t& get_adapter() {
        return *adapter;
      }

      reporter_t& get_reporter() {
        return *reporter;
      }

      run_policy_t& get_policy() {
        return *run_policy;
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
