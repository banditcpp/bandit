#ifndef BANDIT_CONTEXT_H
#define BANDIT_CONTEXT_H

namespace bandit {
  namespace detail {

    using namespace std;

    class context
    {
      public:
        virtual void execution_is_starting() = 0;
        virtual void register_before_each(voidfunc_t func) = 0;
        virtual void register_after_each(voidfunc_t func) = 0;
        virtual void run_before_eaches() = 0;
        virtual void run_after_eaches() = 0;
    };

    class bandit_context : public context
    {
      public:
        bandit_context()
          : is_executing_(false)
        {}

        void execution_is_starting()
        {
          is_executing_ = true;
        }

        void register_before_each(voidfunc_t func)
        {
          if(is_executing_)
          {
            throw test_run_error("before_each was called after 'describe' or 'it'");
          }

          before_eaches_.push_back(func);
        }

        void register_after_each(voidfunc_t func)
        {
          if(is_executing_)
          {
            throw test_run_error("after_each was called after 'describe' or 'it'");
          }

          after_eaches_.push_back(func);
        }

        void run_before_eaches()
        {
          run_all(before_eaches_);
        }

        void run_after_eaches()
        {
          run_all(after_eaches_);
        }

      private:
        void run_all(const list<voidfunc_t>& funcs)
        {
          auto call_func = [](voidfunc_t f){ f(); };

          for_each(funcs.begin(), funcs.end(), call_func);
        }

      private:
        bool is_executing_;
        list<voidfunc_t> before_eaches_;
        list<voidfunc_t> after_eaches_;
    };
    typedef deque<context*> contextstack_t;

    inline contextstack_t& context_stack()
    {
      static contextstack_t contexts;
      return contexts;
    }
  }
}

#endif
