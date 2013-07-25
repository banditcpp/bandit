#ifndef BANDIT_OPTIONS_H
#define BANDIT_OPTIONS_H

namespace bandit { namespace detail {

    // TODO: print any unknown options
    // TODO: check for parser errors
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
      }

      bool help()
      {
        return options_[HELP];
      }

      void print_usage()
      {
        option::printUsage(std::cout, usage());
      }

      bool version()
      {
        return options_[VERSION];
      }

      private:
        enum option_index { UNKNOWN, VERSION, HELP };
        static const option::Descriptor* usage()
        {
          static const option::Descriptor usage[] =
          {
            {UNKNOWN, 0, "", "", option::Arg::None,         "USAGE: <executable> [options]\n\n"
                                                            "Options:" },
            {VERSION, 0, "v", "version", option::Arg::None, "  --version, -v, \tPrint version of bandit"},
            {HELP, 0, "", "help", option::Arg::None,        "  --help, \tPrint usage and exit."},
            {0, 0, 0, 0, 0, 0}
          };

          return usage;
        }

      private:
        std::vector<option::Option> options_;
        bool parsed_ok_;

    };

}}

#endif
