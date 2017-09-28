#ifndef BANDIT_RUN_POLICIES_H
#define BANDIT_RUN_POLICIES_H

#include <bandit/run_policies/always.h>
#include <bandit/run_policies/never.h>
#include <bandit/run_policies/bandit.h>

namespace bandit {
  namespace detail {
    struct policy_runner {
      // A function is required to initialize a static run_policy variable in a header file
      // and this struct aims at encapsulating this function
      static void register_run_policy(run_policy_t* policy) {
        if (policy == nullptr) {
          throw std::runtime_error("Invalid null policy passed to "
                                   "bandit::detail::register_run_policy");
        }
        get_run_policy_address() = policy;
      }

      static run_policy_t& registered_run_policy() {
        auto policy = get_run_policy_address();
        if (policy == nullptr) {
          throw std::runtime_error("No policy set. Please call "
                                   "bandit::detail::register_run_policy with a non-null reporter");
        }
        return *policy;
      }

    private:
      static run_policy_t*& get_run_policy_address() {
        static run_policy_t* policy = nullptr;
        return policy;
      }
    };

    inline void register_run_policy(run_policy_t* policy) {
      policy_runner::register_run_policy(policy);
    }

    inline run_policy_t& registered_run_policy() {
      return policy_runner::registered_run_policy();
    }
  }
}
#endif
