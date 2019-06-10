#ifndef BANDIT_FAILURE_FORMATTERS_QT_CREATOR_H
#define BANDIT_FAILURE_FORMATTERS_QT_CREATOR_H

#include <sstream>
#include <bandit/failure_formatters/interface.h>

namespace bandit {
  namespace failure_formatter {
    struct qt_creator : public interface {
      std::string format(const detail::assertion_exception& err) const override {
        std::stringstream ss;
        if (err.file_name().size()) {
          ss << "file://" << err.file_name();

          if (err.line_number()) {
            ss << ":" << err.line_number();
          }

          ss << ": ";
        }

        ss << err.what();

        return ss.str();
      }
    };
  }
}
#endif
