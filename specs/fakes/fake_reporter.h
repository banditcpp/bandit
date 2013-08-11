#ifndef BANDIT_SPECS_FAKE_REPORTER_H
#define BANDIT_SPECS_FAKE_REPORTER_H

namespace bandit { namespace fakes {
  struct fake_reporter : 
    public bandit::detail::listener, 
    public bandit::specs::logging_fake
  {
    fake_reporter() : test_run_status_(true)
    {}

    void test_run_starting()
    {
      log() << "test_run_starting" << std::endl;
    }

    void test_run_complete()
    {
      log() << "test_run_complete" << std::endl;
    }

    void context_starting(const char* desc)
    {
      log() << "context_starting: " <<  desc << std::endl;
    }

    void context_ended(const char* desc)
    {
      log() << "context_ended: " <<  desc << std::endl;
    }

    void test_run_error(const char* desc, const struct bandit::detail::test_run_error& err)
    {
      log() << "test_run_error: " <<  desc <<  " (" <<  strip_newline(err.what()) <<  ")" << std::endl;
    }

    void it_starting(const char* desc)
    {
      log() << "it_starting: " <<  desc << std::endl;
    }

    void it_succeeded(const char* desc)
    {
      log() << "it_succeeded: " <<  desc << std::endl;
    }

    void it_failed(const char* desc, const bandit::detail::assertion_exception& ex)
    {
      log() << "it_failed: " <<  desc <<  " ("  <<  strip_newline(ex.what()) <<  ")" << std::endl;
    }

    void it_unknown_error(const char* desc)
    {
      log() << "it_unknown_error: " <<  desc << std::endl;
    }

    void it_skip(const char* desc)
    {
      log() << "it_skip: " <<  desc << std::endl;
    }

    bool did_we_pass() const
    {
      return test_run_status_;
    }

    void set_test_run_status(bool status)
    {
      test_run_status_ = status;
    }

    private:
    bool test_run_status_;
  };
  typedef std::unique_ptr<fake_reporter> fake_reporter_ptr;
}}

#endif
