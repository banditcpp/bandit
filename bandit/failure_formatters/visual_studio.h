#ifndef BANDIT_FAILURE_FORMATTERS_VISUAL_STUDIO_H
#define BANDIT_FAILURE_FORMATTERS_VISUAL_STUDIO_H

#include <sstream>
#include <bandit/failure_formatters/interface.h>

namespace bandit {
  namespace failure_formatter {
    struct visual_studio : public interface {
      std::string format(const detail::assertion_exception& err) const override {
        std::stringstream ss;
        if (err.file_name().size()) {
          ss << err.file_name();

          if (err.line_number()) {
            ss << "(" << err.line_number() << ")";
          }

          ss << ": ";
        } else {
          ss << "bandit: ";
        }

        ss << err.what();

        return ss.str();
      }
    };
  }
}
#endif
