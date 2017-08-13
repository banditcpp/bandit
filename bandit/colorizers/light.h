#ifndef BANDIT_COLORIZERS_LIGHT_H
#define BANDIT_COLORIZERS_LIGHT_H

#include <bandit/colorizers/backend.h>
#include <bandit/colorizers/interface.h>

namespace bandit {
  namespace colorizer {
    struct light : interface, private backend {
      light(bool colors_enabled = true) : backend(colors_enabled) {}

      const std::string good() const override {
        return backend::green();
      }

      const std::string neutral() const override {
        return backend::yellow();
      }

      const std::string info() const override {
        return backend::blue();
      }

      const std::string bad() const override {
        return backend::red();
      }

      const std::string emphasize() const override {
        return backend::white();
      }

      const std::string reset() const override {
        return backend::reset();
      }
    };
  }
}
#endif
