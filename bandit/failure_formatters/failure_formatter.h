#ifndef BANDIT_FAILURE_FORMATTER_H
#define BANDIT_FAILURE_FORMATTER_H

namespace bandit {

  struct failure_formatter
  {
    virtual std::string format(const assertion_exception&) const = 0;
  };
}

#endif
