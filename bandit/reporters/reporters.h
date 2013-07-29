#ifndef BANDIT_REPORTERS_H
#define BANDIT_REPORTERS_H

#include <bandit/reporters/progress_reporter.h>
#include <bandit/reporters/test_run_summary.h>
#include <bandit/reporters/dots_reporter.h>
#include <bandit/reporters/single_line_reporter.h>

namespace bandit { namespace detail {

  // TODO: Change to "registered_reporter"
  inline listener& default_reporter(listener* reporter = NULL)
  {
    static struct listener* reporter_;

    if(reporter)
    {
      reporter_ = reporter;
    }

    return *reporter_;
  }

}}

#endif
