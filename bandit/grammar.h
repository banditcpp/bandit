#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

#include <bandit/settings.h>

namespace bandit {
  inline void describe(const std::string& desc, std::function<void()> func,
      detail::settings_t& settings, bool hard_skip = false) {
    settings.describe(desc, func, hard_skip);
  }

  inline void describe(const std::string& desc, std::function<void()> func, bool hard_skip = false) {
    describe(desc, func, detail::registered_settings(), hard_skip);
  }

  inline void describe_skip(const std::string& desc, std::function<void()> func,
      detail::settings_t& settings) {
    describe(desc, func, settings, true);
  }

  inline void describe_skip(const std::string& desc, std::function<void()> func) {
    describe_skip(desc, func, detail::registered_settings());
  }

  inline void xdescribe(const std::string& desc, std::function<void()> func,
      detail::settings_t& settings = detail::registered_settings()) {
    describe_skip(desc, func, settings);
  }

  inline void before_each(std::function<void()> func,
      detail::settings_t& settings = detail::registered_settings()) {
    settings.before_each(func);
  }

  inline void after_each(std::function<void()> func,
      detail::settings_t& settings = detail::registered_settings()) {
    settings.after_each(func);
  }

  inline void it_skip(const std::string& desc, std::function<void()> func, detail::settings_t& settings) {
    settings.it(desc, func, true);
  }

  inline void it_skip(const std::string& desc, std::function<void()> func) {
    it_skip(desc, func, detail::registered_settings());
  }

  inline void xit(const std::string& desc, std::function<void()> func,
      detail::settings_t& settings = detail::registered_settings()) {
    it_skip(desc, func, settings);
  }

  inline void it(const std::string& desc, std::function<void()> func, detail::settings_t& settings,
      bool hard_skip = false) {
    settings.it(desc, func, hard_skip);
  }

  inline void it(const std::string& desc, std::function<void()> func, bool hard_skip = false) {
    it(desc, func, detail::registered_settings(), hard_skip);
  }
}
#endif
