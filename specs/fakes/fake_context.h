#ifndef BANDIT_FAKE_CONTEXT_H
#define BANDIT_FAKE_CONTEXT_H

namespace bandit { namespace fakes {

  struct fake_context : public bandit::detail::context, public bandit::specs::logging_fake
  {
    fake_context() : hard_skip_(false), name_("fake_context"),
      custom_after_each_([](){}), custom_before_each_([](){})
    {}

    const std::string& name()
    {
      log() << "name" << std::endl;
      return name_;
    }

    void execution_is_starting()
    {
      log() << "execution_is_starting" << std::endl;
    }

    void register_before_each(detail::voidfunc_t)
    {
      log() << "register_before_each" << std::endl;
    }

    void register_after_each(detail::voidfunc_t)
    {
      log() << "register_after_each" << std::endl;
    }

    void run_before_eaches()
    {
      log() << "run_before_eaches" << std::endl;
      custom_before_each_();
    }

    void run_after_eaches()
    {
      log() << "run_after_eaches" << std::endl;
      custom_after_each_();
    }

    bool hard_skip() 
    {
      log() << "hard_skip: returning " << hard_skip_ << std::endl;
      return hard_skip_;
    }

    void with_after_each(detail::voidfunc_t call)
    {
      custom_after_each_ = call;
    }

    void with_before_each(detail::voidfunc_t call)
    {
      custom_before_each_ = call;
    }

    private:
    bool hard_skip_;
    std::string name_;
    detail::voidfunc_t custom_after_each_;
    detail::voidfunc_t custom_before_each_;
  };
}}

#endif
