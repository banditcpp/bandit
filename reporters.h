#ifndef BANDIT_REPORTERS_H
#define BANDIT_REPORTERS_H

#include <bandit/reporters/dots.h>
#include <bandit/reporters/singleline.h>
#include <bandit/reporters/xunit.h>
#include <bandit/reporters/info.h>
#include <bandit/reporters/spec.h>
#include <bandit/reporters/crash.h>

namespace bandit {
  namespace detail {
    struct reporter {
      // A function is required to initialize a static reporter variable in a header file
      // and this struct aims at encapsulating this function
      static void register_reporter(reporter_t* reporter) {
        if (reporter == nullptr) {
          throw std::runtime_error("Invalid null reporter passed to "
                                   "bandit::detail::register_reporter");
        }
        get_reporter_address() = reporter;
      }
      static reporter_t& registered_reporter() {
        auto reporter = get_reporter_address();
        if (reporter == nullptr) {
          throw std::runtime_error("No reporter set. Please call "
                                   "bandit::detail::register_reporter with a non-null reporter");
        }
        return *reporter;
      }

    private:
      static reporter_t*& get_reporter_address() {
        static reporter_t* reporter_ = nullptr;
        return reporter_;
      }
    };

    inline void register_reporter(reporter_t* reporter) {
      reporter::register_reporter(reporter);
    }
    inline reporter_t& registered_reporter() {
      return reporter::registered_reporter();
    }
  }
}
#endif
