#ifndef BANDIT_REPORTERS_H
#define BANDIT_REPORTERS_H

#include <bandit/reporters/reporter.h>
#include <bandit/reporters/dots_reporter.h>

namespace bandit { namespace detail {

  inline reporter& default_reporter()
  {
    static dots_reporter reporter;
    return reporter;
  }

}}

#endif
