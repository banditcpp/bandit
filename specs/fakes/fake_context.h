#ifndef BANDIT_FAKE_CONTEXT_H
#define BANDIT_FAKE_CONTEXT_H

namespace bandit { namespace fakes {

  struct fake_context : public bandit::detail::context, public bandit::specs::logging_fake
  {
    fake_context() : is_skipped_(false)
    {}

    void execution_is_starting()
    {
      log() << "execution_is_starting" << std::endl;
    }

    void register_before_each(voidfunc_t)
    {
      log() << "register_before_each" << std::endl;
    }

    void register_after_each(voidfunc_t)
    {
      log() << "register_after_each" << std::endl;
    }

    void run_before_eaches()
    {
      log() << "run_before_eaches" << std::endl;
    }

    void run_after_eaches()
    {
      log() << "run_after_eaches" << std::endl;
    }

    void set_is_skipped(bool skip)
    {
      log() << "set_is_skipped: " << skip << std::endl;
      is_skipped_ = skip;
    }

    bool is_skipped()
    {
      log() << "is_skipped: returning false" << std::endl;
      return is_skipped_;
    }

    private:
    bool is_skipped_;
  };
}}

#endif
