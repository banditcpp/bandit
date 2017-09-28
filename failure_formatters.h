#ifndef BANDIT_FAILURE_FORMATTERS_H
#define BANDIT_FAILURE_FORMATTERS_H

#include "failure_formatters/posix.h"
#include "failure_formatters/visual_studio.h"

namespace bandit {
  namespace detail {
    inline failure_formatter_t& registered_failure_formatter() {
      static failure_formatter::posix default_formatter;
      return default_formatter;
    }
  }
}
#endif
