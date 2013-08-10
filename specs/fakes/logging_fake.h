#ifndef BANDIT_SPECS_LOGGING_FAKE_H
#define BANDIT_SPECS_LOGGING_FAKE_H
#include <sstream>

namespace bandit { namespace specs {

  struct logging_fake
  {
    std::ostream& log()
    {
      return logstm_;
    }

    std::string strip_newline(const char* val)
    {
      std::string no_newline = val;
      std::transform(no_newline.begin(), no_newline.end(), no_newline.begin(), [](const char& c) {
          return (c == '\n' || c == '\r') ? ' ' : c;
      });
      return no_newline;
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
