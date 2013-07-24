#ifndef BANDIT_SPECS_LOGGING_FAKE_H
#define BANDIT_SPECS_LOGGING_FAKE_H
#include <sstream>

namespace bandit { namespace specs {

  struct logging_fake
  {
    template <typename T>
    void log(T val, bool add_newline = true)
    {
      logstm_ << val;
      if(add_newline)
      {
        logstm_ << "\n";
      }
    }

    void log(const char* val, bool add_newline = true)
    {
      std::string no_newline = val;
      std::transform(no_newline.begin(), no_newline.end(), no_newline.begin(), [](const char& c) {
          return (c == '\n' || c == '\r') ? ' ' : c;
      });

      logstm_ << no_newline;

      if(add_newline)
      {
        logstm_ << "\n";
      }
    }

    template <typename First, typename... Args>
    void log(First first, Args... args)
    {
      log(first, false);
      log(args...);
    }

    std::string call_log()
    {
      return logstm_.str();
    }

    private:
    std::stringstream logstm_;
  };
}}
#endif
