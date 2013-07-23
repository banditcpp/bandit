#ifndef BANDIT_REPORTER_H
#define BANDIT_REPORTER_H

namespace bandit {
  struct reporter
  {
    virtual void context_starting(const char* desc) = 0;
    virtual void context_ended(const char* desc) = 0;

    virtual void it_starting(const char* desc) = 0;
  };
}

#endif
