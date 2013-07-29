#ifndef BANDIT_REPORTERS_COLORIZER_H
#define BANDIT_REPORTERS_COLORIZER_H

namespace bandit { namespace detail {

  struct colorizer
  {
    colorizer(bool colors_enabled = true)
      : colors_enabled_(colors_enabled)
    {}

    const char* green() const
    {
      return colors_enabled_ ? "\033[1;32m" : "";
    }

    const char* red() const
    {
      return colors_enabled_ ? "\033[1;31m" : "";
    }

    const char* reset() const
    {
      return colors_enabled_ ? "\033[0m" : "";
    }

    private:
    bool colors_enabled_;
  };

}}

#endif
