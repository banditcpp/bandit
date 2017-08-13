#ifndef BANDIT_COLORIZERS_COLORIZER_H
#define BANDIT_COLORIZERS_COLORIZER_H

#include <bandit/colorizers/backend.h>

namespace bandit {
  namespace colorizer {
    struct light : private backend {
      light(bool colors_enabled = true) : backend(colors_enabled) {}

      const std::string good() const {
        return backend::green();
      }

      const std::string neutral() const {
        return backend::yellow();
      }

      const std::string info() const {
        return backend::blue();
      }

      const std::string bad() const {
        return backend::red();
      }

      const std::string emphasize() const {
        return backend::white();
      }

      const std::string reset() const {
        return backend::reset();
      }
    };
  }

  namespace detail {
    using colorizer_t = ::bandit::colorizer::light;
  }
}
#endif
