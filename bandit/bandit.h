#ifndef BANDIT_BANDIT_H
#define BANDIT_BANDIT_H

#ifdef _MSC_VER
#if (_MSC_VER < 1900)
// Visual Studio versions before 2015 do not support the noexcept keyword
#define _ALLOW_KEYWORD_MACROS
#define noexcept
#endif
#endif

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <deque>
#include <stdexcept>

#define BANDIT_VERSION "2.0.0"

namespace bandit { namespace detail {
  typedef std::function<void ()> voidfunc_t;
}}

#include <bandit/assertion_frameworks/snowhouse/snowhouse/snowhouse.h>
using namespace snowhouse;

#include <bandit/assertion_frameworks/matchers/matchers/matchers.h>
using namespace Matchers;

#include <bandit/external/optionparser.h>
#include <bandit/options.h>
#include <bandit/test_run_error.h>
#include <bandit/registration/registration.h>
#include <bandit/assertion_exception.h>
#include <bandit/failure_formatters/failure_formatters.h>
#include <bandit/adapters/adapters.h>
#include <bandit/listener.h>
#include <bandit/reporters/reporters.h>
#include <bandit/context.h>
#include <bandit/run_policies/run_policies.h>
#include <bandit/grammar.h>
#include <bandit/runner.h>

#endif
