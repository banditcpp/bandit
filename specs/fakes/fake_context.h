#ifndef BANDIT_FAKE_CONTEXT_H
#define BANDIT_FAKE_CONTEXT_H

namespace bandit { namespace fakes {

  struct fake_context : public bandit::detail::context, public bandit::specs::logging_fake
  {
    void execution_is_starting()
    {
      log("execution_is_starting");
    }

    void register_before_each(voidfunc_t)
    {
      log("register_before_each");
    }

    void register_after_each(voidfunc_t)
    {
      log("register_after_each");
    }

    void run_before_eaches()
    {
      log("run_before_eaches");
    }

    void run_after_eaches()
    {
      log("run_after_eaches");
    }
  };
}}

#endif
