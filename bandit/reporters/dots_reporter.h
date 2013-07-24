#ifndef BANDIT_DOTS_REPORTER_H
#define BANDIT_DOTS_REPORTER_H

namespace bandit {

  struct dots_reporter : public reporter
  {
    void context_starting(const char*) {}
    void context_ended(const char*) {}
    void it_starting(const char*) {}
    void it_succeeded(const char*) 
    {
      std::cout << ".";
    }

    void it_failed(const char*, const assertion_exception&)
    {
      std::cout << "F";
    }

    void it_error(const char*)
    {
      std::cout << "E";
    }
  };
}

#endif
