#ifndef BANDIT_SPECS_FAKE_REPORTER_H
#define BANDIT_SPECS_FAKE_REPORTER_H

namespace bandit { namespace fakes {
  struct fake_reporter : 
    public bandit::reporter, 
    public bandit::specs::logging_fake
  {
    fake_reporter() : failing_tests_(0)
    {}

    void test_run_starting()
    {
      log("test_run_starting");
    }

    void test_run_complete()
    {
      log("test_run_complete");
    }

    void context_starting(const char* desc)
    {
      log("context_starting: ", desc);
    }

    void context_ended(const char* desc)
    {
      log("context_ended: ", desc);
    }

    void test_run_error(const char* desc, const struct test_run_error& err)
    {
      log("test_run_error: ", desc, " (", err.what(), ")");
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

    unsigned int number_of_failed_tests() const
    {
      return failing_tests_;
    }

    void set_failing_tests(unsigned int count)
    {
      failing_tests_ = count;
    }

    private:
    unsigned int failing_tests_;
  };
  typedef unique_ptr<fake_reporter> fake_reporter_ptr;
}}

#endif
