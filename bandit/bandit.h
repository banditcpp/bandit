#ifndef BANDIT_BANDIT_H
#define BANDIT_BANDIT_H

#include <cassert>
#include <functional>
#include <iostream>
#include <list>
#include <deque>

namespace bandit { namespace detail {
  typedef std::function<void ()> voidfunc_t;
}}

#include <bandit/registration/registration.h>
#include "context.h"
#include "grammar.h"

#endif
