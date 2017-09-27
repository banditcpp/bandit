#ifndef BANDIT_SETTINGS_H
#define BANDIT_SETTINGS_H

#include <bandit/colorizers.h>
#include <bandit/failure_formatters.h>
#include <bandit/options.h>
#include <bandit/reporters.h>
#include <bandit/run_policies.h>

namespace bandit {
  namespace detail {
    struct settings_t {
      colorizer_ptr colorizer;
      failure_formatter_ptr formatter;
      reporter_ptr reporter;
      run_policy_ptr run_policy;

      settings_t(const options& opt)
          : colorizer(create_colorizer(opt)),
            formatter(create_formatter(opt)),
            reporter(create_reporter(opt)),
            run_policy(new run_policy::bandit(opt.filter_chain(), opt.break_on_failure(), opt.dry_run())) {}

    private:
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
  }
}
#endif
