#ifndef BANDIT_SPECS_FAKE_REPORTER_H
#define BANDIT_SPECS_FAKE_REPORTER_H

namespace bandit { namespace fakes {
  struct fake_reporter : 
    public bandit::reporter, 
    public bandit::specs::logging_fake
  {
    void context_starting(const char* desc)
    {
      log("context_starting: ", desc);
    }

    void context_ended(const char* desc)
    {
      log("context_ended: ", desc);
    }

    void it_starting(const char* desc)
    {
      log("it_starting: ", desc);
    }

    void it_succeeded(const char* desc)
    {
      log("it_succeeded: ", desc);
    }
  };
  typedef unique_ptr<fake_reporter> fake_reporter_ptr;
}}

#endif
