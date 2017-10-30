#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

#include <bandit/settings.h>

namespace bandit {
  inline void describe(const std::string& desc, const std::function<void()>& func,
      bool hard_skip = false,
      detail::settings_t& settings = detail::registered_settings()) {
    settings.describe(desc, func, hard_skip);
  }

  inline void describe_skip(const std::string& desc, const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    describe(desc, func, true, settings);
  }

  inline void xdescribe(const std::string& desc, const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    describe(desc, func, true, settings);
  }

  inline void before_each(const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    settings.before_each(func);
  }

  inline void after_each(const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    settings.after_each(func);
  }

  inline void it(const std::string& desc, const std::function<void()>& func,
      bool hard_skip = false, detail::settings_t& settings = detail::registered_settings()) {
    settings.it(desc, func, hard_skip);
  }

  inline void it_skip(const std::string& desc, const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    it(desc, func, true, settings);
  }

  inline void xit(const std::string& desc, const std::function<void()>& func,
      detail::settings_t& settings = detail::registered_settings()) {
    it(desc, func, true, settings);
  }
}
#endif
