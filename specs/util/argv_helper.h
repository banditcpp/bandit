#ifndef BANDIT_SPECS_ARGV_HELPER_H
#define BANDIT_SPECS_ARGV_HELPER_H

#include <string.h>

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
      : argc_(argc) 
    {
      non_const_argv_ = new char*[argc];
      for(int i=0; i < argc; i++)
      {
		std::string s(argv[i]);
        non_const_argv_[i] = new char[s.size() + 1];
        for(size_t c=0;c<s.size();c++)
		{
			non_const_argv_[i][c] = s[c];
		}
		non_const_argv_[i][s.size()] = 0;
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

    char** argv()
    {
      return non_const_argv_;
    }

    int argc()
    {
      return argc_;
    }

    private:
    int argc_;
    char** non_const_argv_;
  };

}}}
#endif
