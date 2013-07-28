#ifndef BANDIT_REPORTERS_H
#define BANDIT_REPORTERS_H

#include <bandit/reporters/reporter.h>
#include <bandit/reporters/dots_reporter.h>
#include <bandit/reporters/single_line_reporter.h>

namespace bandit { namespace detail {

  // TODO: Change to "registered_reporter"
  inline reporter& default_reporter(reporter* reporter = NULL)
  {
    static struct reporter* reporter_;

    if(reporter)
    {
      reporter_ = reporter;
    }

    return *reporter_;
  }

}}

#endif
