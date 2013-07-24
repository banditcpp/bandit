#ifndef BANDIT_REPORTER_H
#define BANDIT_REPORTER_H

namespace bandit {
  struct reporter
  {
    virtual void test_run_starting() = 0;

    virtual void context_starting(const char* desc) = 0;
    virtual void context_ended(const char* desc) = 0;

    virtual void it_starting(const char* desc) = 0;
    virtual void it_succeeded(const char* desc) = 0;
    virtual void it_failed(const char* desc, const assertion_exception& ex) = 0;
    virtual void it_unknown_error(const char* desc) = 0;
  };
}

#endif
