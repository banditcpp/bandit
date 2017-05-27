#ifndef BANDIT_SPECS_ARGV_HELPER_H
#define BANDIT_SPECS_ARGV_HELPER_H

#include <list>
#include <string>

namespace bandit {
  namespace specs {
    namespace util {
      // main() is supposed to receive its arguments as a non const 'char* argv[]'.
      // This is a pain to create for each test.
      //
      // This class makes up an argc/argv pair from a simple brace-enclosed initializer list.
      // The first executable "argument" is already included.
      struct argv_helper {
        argv_helper(std::list<std::string>&& args)
            : argc_(args.size() + 1) {
          args.emplace_front("executable");
          non_const_argv_ = new char*[argc_];
          size_t i = 0;
          for (auto arg : args) {
            non_const_argv_[i] = new char[arg.size() + 1];
            arg.copy(non_const_argv_[i], arg.size());
            non_const_argv_[i][arg.size()] = '\0';
            ++i;
          }
        }

        argv_helper(const argv_helper&) = default;

        ~argv_helper() {
          for (int i = 0; i < argc_; i++) {
            delete[] non_const_argv_[i];
          }

          delete[] non_const_argv_;
        }

        char** argv() {
          return non_const_argv_;
        }

        int argc() {
          return argc_;
        }

      private:
        int argc_;
        char** non_const_argv_;
      };
    }
  }
}
#endif
