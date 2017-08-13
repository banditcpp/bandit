#ifndef BANDIT_COLORIZERS_COLORIZER_H
#define BANDIT_COLORIZERS_COLORIZER_H

#include <bandit/colorizers/backend.h>

namespace bandit {
  namespace detail {
    struct colorizer : private bandit::colorizer::backend {
      colorizer(bool colors_enabled = true) : backend(colors_enabled) {}

      const std::string green() const {
        return backend::green();
      }

      const std::string yellow() const {
        return backend::yellow();
      }

      const std::string blue() const {
        return backend::blue();
      }

      const std::string red() const {
        return backend::red();
      }

      const std::string white() const {
        return backend::white();
      }

      const std::string reset() const {
        return backend::reset();
      }
    };
  }
}
#endif
