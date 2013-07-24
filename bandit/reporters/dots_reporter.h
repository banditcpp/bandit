#ifndef BANDIT_DOTS_REPORTER_H
#define BANDIT_DOTS_REPORTER_H

namespace bandit {

  struct dots_reporter : public reporter
  {
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
      std::cout << std::endl;

      if(specs_failed_ > 0)
      {
        std::cout << "There were failures!" << std::endl;
        std::for_each(failures_.begin(), failures_.end(), [](const std::string& failure) {
            std::cout << failure << std::endl;
        });
        std::cout << std::endl;
      }

      std::cout << "Test run complete. " << specs_run_ << " tests run. " << specs_succeeded_ << " succeeded. " << specs_failed_ << " failed." << std::endl;
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
      std::cout << ".";
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

      std::cout << "F";
    }

    void it_unknown_error(const char*)
    {
      specs_failed_++;
      std::cout << "E";
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
      std::deque<std::string> contexts_;
      std::list<std::string> failures_;
      int specs_run_;
      int specs_succeeded_;
      int specs_failed_;
  };
}

#endif
