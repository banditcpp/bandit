#ifndef BANDIT_OPTIONS_H
#define BANDIT_OPTIONS_H

#include <vector>
#include <iostream>

#include <bandit/external/optionparser.h>

namespace bandit { namespace detail {

    // TODO: print any unknown options
    struct options
    {

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
        return options_[HELP] != NULL;
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
        return options_[VERSION] != NULL;
      }

      const char* reporter() const
      {
        return options_[REPORTER].arg;
      }

      bool no_color() const
      {
        return options_[NO_COLOR] != NULL;
      }

      typedef enum
      {
        FORMATTER_DEFAULT,
        FORMATTER_VS,
        FORMATTER_UNKNOWN
      } formatters;

      formatters formatter() const
      {
        std::string arg = options_[FORMATTER].arg ? options_[FORMATTER].arg : "";
        if(arg == "vs")
        {
          return formatters::FORMATTER_VS;
        }

        return formatters::FORMATTER_DEFAULT;
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
          return options_[BREAK_ON_FAILURE] != NULL;
      }

      bool dry_run() const
      {
          return options_[DRY_RUN] != NULL;
      }

      private:
        enum option_index { UNKNOWN, VERSION, HELP, REPORTER, NO_COLOR,
          FORMATTER, SKIP, ONLY, BREAK_ON_FAILURE, DRY_RUN };

        static const option::Descriptor* usage()
        {
          static const option::Descriptor usage[] =
          {
            {UNKNOWN, 0, "", "", option::Arg::None,         "USAGE: <executable> [options]\n\n"
                                                            "Options:" },
            {VERSION, 0, "", "version", option::Arg::None, "  --version, \tPrint version of bandit"},
            {HELP, 0, "", "help", option::Arg::None,        "  --help, \tPrint usage and exit."},
            {REPORTER, 0, "", "reporter", option::Arg::Optional, "  --reporter=<reporter>, \tSelect reporter (dots, singleline, xunit, info, spec)"},
            {NO_COLOR, 0, "", "no-color", option::Arg::None,     "  --no-color, \tSuppress colors in output"},
            {FORMATTER, 0, "", "formatter", option::Arg::Optional, "  --formatter=<formatter>, \tSelect formatting of errors (default, vs)"},
            {SKIP, 0, "", "skip", option::Arg::Optional, "  --skip=<substring>, \tskip all 'describe' and 'it' containing substring"},
            {ONLY, 0, "", "only", option::Arg::Optional, "  --only=<substring>, \tonly run 'describe' and 'it' containing substring"},
            {BREAK_ON_FAILURE, 0, "", "break-on-failure", option::Arg::Optional, "  --break-on-failure, \tstop test run on first failing test"},
            {DRY_RUN, 0, "", "dry-run", option::Arg::Optional, "  --dry-run, \tdon't run tests. Just list progress. Use to list available tests"},
            {0, 0, 0, 0, 0, 0}
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
