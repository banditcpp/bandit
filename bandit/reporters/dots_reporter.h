#ifndef BANDIT_DOTS_REPORTER_H
#define BANDIT_DOTS_REPORTER_H

namespace bandit {

  struct dots_reporter : public progress_reporter
  {
    dots_reporter(std::ostream& stm, const failure_formatter& failure_formatter)
      : progress_reporter(failure_formatter), stm_(stm) 
    {}

    dots_reporter(const failure_formatter& failure_formatter)
      : progress_reporter(failure_formatter), stm_(std::cout)
    {}

    void test_run_complete() 
    {
      progress_reporter::test_run_complete();

      stm_ << std::endl;

      test_run_summary summary(specs_run_, specs_failed_, specs_succeeded_, failures_, 
          test_run_errors_);
      summary.write(stm_);
    }

    void test_run_error(const char* desc, const struct test_run_error& err)
    {
      progress_reporter::test_run_error(desc, err);

      std::stringstream ss;
      ss << std::endl;
      ss << "Failed to run \"" << current_context_name() << "\": error \"" << err.what() << "\"" << std::endl;

      test_run_errors_.push_back(ss.str());
    }

    void it_succeeded(const char* desc) 
    {
      progress_reporter::it_succeeded(desc);
      stm_ << ".";
    }

    void it_failed(const char* desc, const assertion_exception& ex)
    {
      progress_reporter::it_failed(desc, ex);
      stm_ << "F";
    }

    void it_unknown_error(const char* desc)
    {
      progress_reporter::it_unknown_error(desc);
      stm_ << "E";
    }

    private:
    std::ostream& stm_;
  };
}

#endif
