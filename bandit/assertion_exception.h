#ifndef BANDIT_ASSERTION_EXCEPTION_H
#define BANDIT_ASSERTION_EXCEPTION_H

namespace bandit {

  struct assertion_exception : public std::runtime_error
  {
    assertion_exception(const std::string& message, const std::string& file_name, const unsigned int line_number) :
      std::runtime_error(message), message_(message), file_name_(file_name), line_number_(line_number) 
    {}

    private:
      const std::string& message_;
      const std::string& file_name_;
      const unsigned int line_number_;
  };
}

#endif
