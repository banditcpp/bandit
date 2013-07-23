#ifndef BANDIT_REPORTERS_H
#define BANDIT_REPORTERS_H

namespace bandit {

  struct reporter
  {
    virtual void context_starting(const char* decs) = 0;
    virtual void context_ended(const char* decs) = 0;
  };
}

#endif
