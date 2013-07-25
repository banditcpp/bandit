#ifndef BANDIT_DOTS_REPORTER_H
#define BANDIT_DOTS_REPORTER_H

namespace bandit {

  struct dots_reporter : public reporter
  {
    dots_reporter(std::ostream& stm)
      : stm_(stm)
    {}

    dots_reporter()
      : stm_(std::cout)
    {}

    void test_run_starting() 
    {
      specs_run_ = 0;
      specs_succeeded_ = 0;
      specs_failed_ = 0;
      failures_.clear();
      contexts_.clear();
    }

    void test_run_complete() 
    {
      stm_ << std::endl;

      if(specs_run_ == 0)
      {
        stm_ << "Could not find any tests." << std::endl;
        return;
      }

      if(specs_failed_ > 0)
      {
        stm_ << "There were failures!" << std::endl;
        std::for_each(failures_.begin(), failures_.end(), [&](const std::string& failure) {
            stm_ << failure << std::endl;
        });
        stm_ << std::endl;
      }

      stm_ << "Test run complete. " << specs_run_ << " tests run. " << specs_succeeded_ << " succeeded. " << specs_failed_ << " failed." << std::endl;
    }

    void context_starting(const char* desc) 
    {
      contexts_.push_back(std::string(desc));
    }

    void context_ended(const char*) 
    {
      contexts_.pop_back();
    }

    void it_starting(const char*) 
    {
      specs_run_++;
    }

    void it_succeeded(const char*) 
    {
      specs_succeeded_++;
      stm_ << ".";
    }

    void it_failed(const char* desc, const assertion_exception& ex)
    {
      specs_failed_++;

      std::stringstream ss;
      ss << std::endl;
      ss << current_context_name() << " " << desc << ":" << std::endl;
      ss << ex.file_name() << ":" << ex.line_number() << ": ";
      ss << ex.what();
      ss << std::endl;

      failures_.push_back(ss.str());

      stm_ << "F";
    }

    void it_unknown_error(const char*)
    {
      specs_failed_++;
      stm_ << "E";
    }

    unsigned int number_of_failed_tests() const
    {
      return specs_failed_;
    }

  private:
    std::string current_context_name()
    {
      std::string name;

      for_each(contexts_.begin(), contexts_.end(), [&](const std::string context){
          if(name.size() > 0)
          {
            name += " ";
          }

          name += context;
      });

      return name;
    }

  private:
    std::ostream& stm_;
    std::deque<std::string> contexts_;
    std::list<std::string> failures_;
    int specs_run_;
    int specs_succeeded_;
    int specs_failed_;
  };
}

#endif
