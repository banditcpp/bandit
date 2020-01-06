#ifndef BANDIT_REPORTERS_INFO_H
#define BANDIT_REPORTERS_INFO_H

#include <iostream>
#include <stack>
#include <bandit/reporters/colored_base.h>

namespace bandit {
  namespace reporter {
    struct info : public colored_base {
      struct context_info {
        context_info(const std::string& d) : desc(d), total(0), skipped(0), failed(0) {}

        void merge(const context_info& ci) {
          total += ci.total;
          skipped += ci.skipped;
          failed += ci.failed;
        }

        const std::string desc; // copy
        int total;
        int skipped;
        int failed;
      };

      info(std::ostream& stm, const detail::failure_formatter_t& formatter, const detail::colorizer_t& colorizer)
          : colored_base(stm, formatter, colorizer),
            indentation_(0), not_yet_shown_(0), context_stack_() {}

      info(const detail::failure_formatter_t& formatter, const detail::colorizer_t& colorizer)
          : info(std::cout, formatter, colorizer) {}

      info& operator=(const info&) {
        return *this;
      }

      void list_failures_and_errors() {
        if (specs_failed_ > 0) {
          stm_
              << colorizer_.bad()
              << "List of failures:"
              << std::endl;
          for (const auto& failure : failures_) {
            stm_
                << colorizer_.emphasize()
                << " (*) "
                << colorizer_.bad()
                << failure << std::endl;
          }
        }
        if (test_run_errors_.size() > 0) {
          stm_
              << colorizer_.bad()
              << "List of run errors:"
              << std::endl;
          for (const auto& error : test_run_errors_) {
            stm_
                << colorizer_.emphasize()
                << " (*) "
                << colorizer_.bad()
                << error << std::endl;
          }
        }
      }

      void summary() {
        stm_
            << colorizer_.emphasize()
            << "Tests run: " << specs_run_
            << std::endl;
        if (specs_skipped_ > 0) {
          stm_
              << colorizer_.neutral()
              << "Skipped: " << specs_skipped_
              << std::endl;
        }
        if (specs_succeeded_ > 0) {
          stm_
              << colorizer_.good()
              << "Passed: " << specs_succeeded_
              << std::endl;
        }
        if (specs_failed_ > 0) {
          stm_
              << colorizer_.bad()
              << "Failed: " << specs_failed_
              << std::endl;
        }
        if (test_run_errors_.size() > 0) {
          stm_
              << colorizer_.bad()
              << "Errors: " << test_run_errors_.size()
              << std::endl;
        }
        stm_
            << colorizer_.reset()
            << std::endl;
      }

      void test_run_complete() override {
        colored_base::test_run_complete();
        stm_ << std::endl;
        list_failures_and_errors();
        summary();
        stm_.flush();
      }

      void test_run_error(const std::string& desc, const detail::test_run_error& err) override {
        colored_base::test_run_error(desc, err);

        std::stringstream ss;
        ss << "Failed to run \"" << current_context_name() << "\": error \"" << err.what() << "\"" << std::endl;
        test_run_errors_.push_back(ss.str());
      }

      void context_starting(const std::string& desc) override {
        colored_base::context_starting(desc);
        context_stack_.emplace(desc);
        if (context_stack_.size() == 1) {
          output_context_start_message();
        } else {
          ++not_yet_shown_;
        }
      }

      void output_context_start_message() {
        stm_
            << indent()
            << colorizer_.info()
            << "begin "
            << colorizer_.emphasize()
            << context_stack_.top().desc
            << colorizer_.reset()
            << std::endl;
        ++indentation_;
        stm_.flush();
      }

      void output_not_yet_shown_context_start_messages() {
        std::stack<context_info> temp_stack;
        for (int i = 0; i < not_yet_shown_; ++i) {
          temp_stack.push(context_stack_.top());
          context_stack_.pop();
        }
        for (int i = 0; i < not_yet_shown_; ++i) {
          context_stack_.push(temp_stack.top());
          output_context_start_message();
          temp_stack.pop();
        }
        not_yet_shown_ = 0;
      }

      void context_ended(const std::string& desc) override {
        colored_base::context_ended(desc);
        if (context_stack_.size() == 1 || context_stack_.top().total > context_stack_.top().skipped) {
          output_context_end_message();
        }
        const context_info context = context_stack_.top(); // copy
        context_stack_.pop();
        if (!context_stack_.empty()) {
          context_stack_.top().merge(context);
        }
        if (not_yet_shown_ > 0) {
          --not_yet_shown_;
        }
      }

      void output_context_end_message() {
        const context_info& context = context_stack_.top();
        --indentation_;
        stm_
            << indent()
            << colorizer_.info()
            << "end "
            << colorizer_.reset()
            << context.desc;
        if (context.total > 0) {
          stm_
              << colorizer_.emphasize()
              << " " << context.total << " total";
        }
        if (context.skipped > 0) {
          stm_
              << colorizer_.neutral()
              << " " << context.skipped << " skipped";
        }
        if (context.failed > 0) {
          stm_
              << colorizer_.bad()
              << " " << context.failed << " failed";
        }
        stm_ << colorizer_.reset() << std::endl;
      }

      void it_skip(const std::string& desc) override {
        colored_base::it_skip(desc);
        ++context_stack_.top().total;
        ++context_stack_.top().skipped;
      }

      void it_starting(const std::string& desc) override {
        if (context_stack_.size() > 1 && context_stack_.top().total == context_stack_.top().skipped) {
          output_not_yet_shown_context_start_messages();
        }

        colored_base::it_starting(desc);
        stm_
            << indent()
            << colorizer_.neutral()
            << "[ TEST ]"
            << colorizer_.reset()
            << " it " << desc;
        ++indentation_;
        stm_.flush();
      }

      void it_succeeded(const std::string& desc) override {
        colored_base::it_succeeded(desc);
        ++context_stack_.top().total;
        --indentation_;
        stm_
            << "\r" << indent()
            << colorizer_.good()
            << "[ PASS ]"
            << colorizer_.reset()
            << " it " << desc
            << std::endl;
        stm_.flush();
      }

      void it_failed(const std::string& desc, const detail::assertion_exception& ex) override {
        colored_base::it_failed(desc, ex);

        ++context_stack_.top().total;
        ++context_stack_.top().failed;
        --indentation_;
        stm_
            << "\r" << indent()
            << colorizer_.bad()
            << "[ FAIL ]"
            << colorizer_.reset()
            << " it " << desc
            << std::endl;
        stm_.flush();
      }

      void it_unknown_error(const std::string& desc) override {
        colored_base::it_unknown_error(desc);

        ++context_stack_.top().total;
        ++context_stack_.top().failed;
        --indentation_;
        stm_
            << "\r" << indent()
            << colorizer_.bad()
            << "-ERROR->"
            << colorizer_.reset()
            << " it " << desc
            << std::endl;
        stm_.flush();
      }

    private:
      std::string indent() {
        return std::string(2 * indentation_, ' ');
      }

      int indentation_;
      int not_yet_shown_; // number of elements in stack that are not yet shown
      std::stack<context_info> context_stack_;
    };
  }
}
#endif
