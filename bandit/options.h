#ifndef BANDIT_OPTIONS_H
#define BANDIT_OPTIONS_H

#include <algorithm>
#include <vector>
#include <iostream>

#include <bandit/external/optionparser.h>

namespace bandit { namespace detail {

    // TODO: print any unknown options
    struct options
    {
      struct argument : public option::Arg {
        static const std::vector<std::string> reporter_list()
        {
          return {
            "dots",
            "singleline",
            "xunit",
            "info",
            "spec",
          };
        }

        static const std::vector<std::string> formatter_list()
        {
          return {
            "default",
            "vs",
          };
        }

        static std::string comma_separated_list(std::vector<std::string> list) {
          std::string csl;
          auto first = list.begin();
          if (first != list.end()) {
            csl += *first;
            std::for_each(++first, list.end(), [&](std::string reporter) {
              csl += ", " + reporter;
            });
          }
          return csl;
        }

        static std::string name(const option::Option& option)
        {
          std::string copy(option.name);
          return copy.substr(0, option.namelen);
        }

        static option::ArgStatus Required(const option::Option& option, bool msg)
        {
          if (option.arg != nullptr) {
            return option::ARG_OK;
          }
          if (msg) {
            std::cerr << "Option '" << name(option) << "' requires an argument\n";
          }
          return option::ARG_ILLEGAL;
        }

        static option::ArgStatus OneOf(const option::Option& option, bool msg, const std::vector<std::string> &&list)
        {
          auto status = Required(option, msg);
          if (status == option::ARG_OK
           && std::find(list.begin(), list.end(), option.arg) == list.end()) {
            if (msg) {
              std::cerr
                << "Option argument of '" << name(option) << "' must be one of: "
                << comma_separated_list(list)
                << std::endl;
            }
            status = option::ARG_ILLEGAL;
          }
          return status;
        }

        static option::ArgStatus Reporter(const option::Option& option, bool msg)
        {
          return OneOf(option, msg, reporter_list());
        }

        static option::ArgStatus Formatter(const option::Option& option, bool msg)
        {
          return OneOf(option, msg, formatter_list());
        }
      };

      options(int argc, char* argv[])
      {
        argc -= (argc>0); argv += (argc>0); // Skip program name (argv[0]) if present
        option::Stats stats(usage(), argc, argv);
        options_.resize(stats.options_max);
        std::vector<option::Option> buffer(stats.buffer_max);
        option::Parser parse(usage(), argc, argv, options_.data(), buffer.data());
        parsed_ok_ = !parse.error();
        has_further_arguments_ = (parse.nonOptionsCount() != 0);
        has_unknown_options_ = (options_[UNKNOWN] != nullptr);
      }

      bool help() const
      {
        return options_[HELP] != nullptr;
      }

      bool parsed_ok() const
      {
        return parsed_ok_;
      }

      bool has_further_arguments() const
      {
        return has_further_arguments_;
      }

      bool has_unknown_options() const
      {
        return has_unknown_options_;
      }

      void print_usage() const
      {
        option::printUsage(std::cout, usage());
      }

      bool version() const
      {
        return options_[VERSION] != nullptr;
      }

      const char* reporter() const
      {
        return options_[REPORTER].arg;
      }

      bool no_color() const
      {
        return options_[NO_COLOR] != nullptr;
      }

      enum class formatters {
        DEFAULT,
        VS,
        UNKNOWN
      };

      formatters formatter() const
      {
        std::string arg = options_[FORMATTER].arg ? options_[FORMATTER].arg : "";
        if(arg == "vs")
        {
          return formatters::VS;
        }

        return formatters::DEFAULT;
      }

      const char* skip() const
      {
        return options_[SKIP].arg ? options_[SKIP].arg : "";
      }

      const char* only() const
      {
        return options_[ONLY].arg ? options_[ONLY].arg : "";
      }

      bool break_on_failure() const
      {
        return options_[BREAK_ON_FAILURE] != nullptr;
      }

      bool dry_run() const
      {
        return options_[DRY_RUN] != nullptr;
      }

    private:
      enum option_index {
        UNKNOWN,
        VERSION,
        HELP,
        REPORTER,
        NO_COLOR,
        FORMATTER,
        SKIP,
        ONLY,
        BREAK_ON_FAILURE,
        DRY_RUN,
      };

      static std::string append_list(std::string desc, std::vector<std::string> list) {
        return desc + ": " + argument::comma_separated_list(list);
      };

      static const option::Descriptor* usage()
      {
        static std::string reporter_help = append_list("  --reporter=<reporter>, \tSelect reporter", argument::reporter_list());
        static std::string formatter_help = append_list("  --formatter=<formatter>, \tSelect error formatter", argument::formatter_list());
        static const option::Descriptor usage[] = {
          { UNKNOWN, 0, "", "", argument::None,
            "USAGE: <executable> [options]\n\n"
            "Options:" },
          { VERSION, 0, "", "version", argument::None,
            "  --version, \tPrint version of bandit" },
          { HELP, 0, "", "help", argument::None,
            "  --help, \tPrint usage and exit." },
          { REPORTER, 0, "", "reporter", argument::Reporter, reporter_help.c_str() },
          { NO_COLOR, 0, "", "no-color", argument::None,
            "  --no-color, \tSuppress colors in output" },
          { FORMATTER, 0, "", "formatter", argument::Formatter, formatter_help.c_str() },
          { SKIP, 0, "", "skip", argument::Required,
            "  --skip=<substring>, \tskip all 'describe' and 'it' containing substring" },
          { ONLY, 0, "", "only", argument::Required,
            "  --only=<substring>, \tonly run 'describe' and 'it' containing substring" },
          { BREAK_ON_FAILURE, 0, "", "break-on-failure", argument::None,
            "  --break-on-failure, \tstop test run on first failing test" },
          { DRY_RUN, 0, "", "dry-run", argument::None,
            "  --dry-run, \tdon't run tests, just list progress."
            "Use to list available tests" },
          { 0, 0, 0, 0, 0, 0 }
        };

        return usage;
      }

    private:
      std::vector<option::Option> options_;
      bool parsed_ok_;
      bool has_further_arguments_;
      bool has_unknown_options_;

    };

}}

#endif
