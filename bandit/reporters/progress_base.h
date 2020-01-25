#ifndef BANDIT_REPORTERS_PROGRESS_BASE_H
#define BANDIT_REPORTERS_PROGRESS_BASE_H

#include <algorithm>
#include <sstream>
#include <vector>
#include <bandit/failure_formatters/interface.h>
#include <bandit/reporters/interface.h>

namespace bandit {
  namespace reporter {
    struct progress_base : public interface {
      progress_base(const detail::failure_formatter_t& formatter)
          : specs_run_(0), specs_succeeded_(0), specs_failed_(0), specs_skipped_(0),
            failure_formatter_(formatter) {}

      progress_base& operator=(const progress_base&) {
        return *this;
      }

      void test_run_starting() override {
        specs_run_ = 0;
        specs_succeeded_ = 0;
        specs_failed_ = 0;
        specs_skipped_ = 0;
        failures_.clear();
        contexts_.clear();
      }

      void test_run_complete() override {}

      void context_starting(const std::string& desc) override {
        contexts_.push_back(std::string(desc));
      }

      void context_ended(const std::string&) override {
        contexts_.pop_back();
      }

      void test_run_error(const std::string&, const detail::test_run_error&) override {}

      void it_starting(const std::string&) override {
        specs_run_++;
      }

      void it_succeeded(const std::string&) override {
        specs_succeeded_++;
      }

      void it_failed(const std::string& desc, const detail::assertion_exception& ex) override {
        specs_failed_++;

        std::stringstream ss;
        ss << current_context_name() << " " << desc << ":" << std::endl;
        ss << failure_formatter_.format(ex);

        failures_.push_back(ss.str());
      }

      void it_unknown_error(const std::string& desc) override {
        specs_failed_++;

        std::stringstream ss;
        ss << current_context_name() << " " << desc << ":" << std::endl;
        ss << "Unknown exception";
        ss << std::endl;

        failures_.push_back(ss.str());
      }

      void it_skip(const std::string&) override {
        specs_skipped_++;
      }

      bool did_we_pass() const override final {
        return specs_failed_ == 0 && test_run_errors_.size() == 0;
      }

    protected:
      std::string current_context_name() {
        std::string name;

        for (const auto& context : contexts_) {
          if (name.size() > 0) {
            name += " ";
          }

          name += context;
        }

        return name;
      }

    protected:
      int specs_run_;
      int specs_succeeded_;
      int specs_failed_;
      int specs_skipped_;
      const detail::failure_formatter_t& failure_formatter_;
      std::vector<std::string> contexts_;
      std::vector<std::string> failures_;
      std::vector<std::string> test_run_errors_;
    };
  }
}
#endif
