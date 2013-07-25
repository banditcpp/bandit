#ifndef BANDIT_SPECS_ARGV_HELPER_H
#define BANDIT_SPECS_ARGV_HELPER_H

namespace bandit { namespace specs { namespace util {

  //
  // main() is supposed to receive its arguments as a non const 'char* argv[]'.
  // This is a pain to create for each test. It's a whole lot easier to create
  // a 'const char* argv[]' construct.
  //
  // This class helps copy from 'const char**' to 'char**' and handle cleanup
  // automatically.
  //
  struct argv_helper
  {
    argv_helper(int argc, const char* argv[])
      : argc_(argc), argv_(argv)
    {
      non_const_argv_ = new char*[argc];
      for(int i=0; i < argc; i++)
      {
        non_const_argv_[i] = new char[strlen(argv[i] + 1)];
        strcpy(non_const_argv_[i], argv_[i]);
      }
    }

    ~argv_helper()
    {
      for(int i=0; i < argc_; i++)
      {
        delete[] non_const_argv_[i];
      }

      delete[] non_const_argv_;
    }

    char** non_const()
    {
      return non_const_argv_;
    }

    int argc()
    {
      return argc_;
    }

    private:
    int argc_;
    const char** argv_;
    char** non_const_argv_;
  };

}}}
#endif
