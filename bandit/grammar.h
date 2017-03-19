#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

#include <bandit/adapters.h>
#include <bandit/reporters.h>
#include <bandit/run_policies.h>

namespace bandit {
  inline void describe(const std::string& desc, std::function<void()> func,
      detail::reporter_t& reporter, context::stack_t& context_stack,
      bool hard_skip = false) {
    reporter.context_starting(desc);

    context_stack.back()->execution_is_starting();

    context::bandit ctxt(desc, hard_skip);

    context_stack.push_back(&ctxt);

    if (break_on_failure() == on) {
      func();
    } else {
      try {
        func();
      } catch (const bandit::detail::test_run_error& error) {
        reporter.test_run_error(desc, error);
      }
    }

    context_stack.pop_back();

    reporter.context_ended(desc);
  }

  inline void describe(const std::string& desc, std::function<void()> func, bool hard_skip = false) {
    describe(desc, func, detail::registered_reporter(), context::stack(), hard_skip);
  }

  inline void describe_skip(const std::string& desc, std::function<void()> func,
      detail::reporter_t& reporter, context::stack_t& context_stack) {
    describe(desc, func, reporter, context_stack, true);
  }

  inline void describe_skip(const std::string& desc, std::function<void()> func) {
    describe_skip(desc, func, detail::registered_reporter(),
        context::stack());
  }

  inline void xdescribe(const std::string& desc, std::function<void()> func,
      detail::reporter_t& reporter = detail::registered_reporter(),
      context::stack_t& context_stack = context::stack()) {
    describe_skip(desc, func, reporter, context_stack);
  }

  inline void throw_if_no_context(std::string&& method, const context::stack_t& context_stack) {
    if (context_stack.empty()) {
      throw detail::test_run_error("'" + method + "' was called without surrounding 'describe'");
    }
  }

  inline void before_each(std::function<void()> func,
      context::stack_t& context_stack) {
    throw_if_no_context("before_each", context_stack);
    context_stack.back()->register_before_each(func);
  }

  inline void before_each(std::function<void()> func) {
    before_each(func, context::stack());
  }

  inline void after_each(std::function<void()> func,
      context::stack_t& context_stack) {
    throw_if_no_context("after_each", context_stack);
    context_stack.back()->register_after_each(func);
  }

  inline void after_each(std::function<void()> func) {
    after_each(func, context::stack());
  }

  inline void it_skip(const std::string& desc, std::function<void()>, detail::reporter_t& reporter) {
    reporter.it_skip(desc);
  }

  inline void it_skip(const std::string& desc, std::function<void()> func) {
    it_skip(desc, func, detail::registered_reporter());
  }

  inline void xit(const std::string& desc, std::function<void()> func,
      detail::reporter_t& reporter = detail::registered_reporter()) {
    it_skip(desc, func, reporter);
  }

  inline void it(const std::string& desc, std::function<void()> func, detail::reporter_t& reporter,
      context::stack_t& context_stack,
      detail::assertion_adapter_t& assertion_adapter,
      detail::run_policy_t& run_policy,
      bool hard_skip = false) {
    throw_if_no_context("it", context_stack);
    if (hard_skip || !run_policy.should_run(desc, context_stack)) {
      it_skip(desc, func, reporter);
      return;
    }

    reporter.it_starting(desc);

    context_stack.back()->execution_is_starting();

    auto try_with_adapter = [&](bool allow_fail, std::function<void()> do_it) {
      if (allow_fail) {
        try {
          assertion_adapter.adapt_exceptions([&] { do_it(); });
        } catch (const bandit::detail::assertion_exception& ex) {
          reporter.it_failed(desc, ex);
          run_policy.encountered_failure();
        } catch (const std::exception& ex) {
          std::string err = std::string("exception: ") + ex.what();
          reporter.it_failed(desc, bandit::detail::assertion_exception(err));
          run_policy.encountered_failure();
        } catch (...) {
          reporter.it_unknown_error(desc);
          run_policy.encountered_failure();
        }
      } else {
        try {
          do_it();
        } catch (...) {
          /* ignore */
        }
      }
    };
    if (break_on_failure() == on) {
      try_with_adapter = [&](detail::voidfunc_t do_it) {
        do_it();
      };
    };

    bool success = false;
    context::interface* last_successful_before_each_context = nullptr;
    try_with_adapter(true, [&] {
      for (auto context : context_stack) {
        context->run_before_eaches();
        last_successful_before_each_context = context;
      }

      func();
      success = true;
    });

    try_with_adapter(success, [&] {
      bool do_run_after_each = false;
      std::for_each(context_stack.rbegin(), context_stack.rend(), [&](context::interface* context) {
        if (context == last_successful_before_each_context) {
          do_run_after_each = true;
        }
        if (do_run_after_each) {
          context->run_after_eaches();
        }
      });

      if (success) {
        reporter.it_succeeded(desc);
      }
    });
  }

  inline void it(const std::string& desc, std::function<void()> func, bool hard_skip = false) {
    it(desc, func, detail::registered_reporter(), context::stack(),
        detail::registered_adapter(), detail::registered_run_policy(), hard_skip);
  }
}
#endif
