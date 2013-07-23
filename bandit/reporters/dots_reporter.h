#ifndef BANDIT_DOTS_REPORTER_H
#define BANDIT_DOTS_REPORTER_H

namespace bandit {

  struct dots_reporter : public reporter
  {
    void context_starting(const char*) {}
    void context_ended(const char*) {}
    void it_starting(const char*) {}
  };
}

#endif
