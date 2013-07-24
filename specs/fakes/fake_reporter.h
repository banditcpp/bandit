#ifndef BANDIT_SPECS_FAKE_REPORTER_H
#define BANDIT_SPECS_FAKE_REPORTER_H

namespace bandit { namespace fakes {
  struct fake_reporter : 
    public bandit::reporter, 
    public bandit::specs::logging_fake
  {
    void test_run_starting()
    {
      log("test_run_starting");
    }

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

    void it_failed(const char* desc, const assertion_exception& ex)
    {
      log("it_failed: ", desc, " (" , ex.what(), ")");
    }

    void it_unknown_error(const char* desc)
    {
      log("it_unknown_error: ", desc);
    }
  };
  typedef unique_ptr<fake_reporter> fake_reporter_ptr;
}}

#endif
