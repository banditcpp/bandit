#ifndef BANDIT_MATCHER_EXCEPTION_H
#define BANDIT_MATCHER_EXCEPTION_H

#include <bandit/assertion_exception.h>

namespace bandit { namespace Matchers {
    class MatcherException : public detail::assertion_exception
    {
    public:
      MatcherException(const std::string& filename, const unsigned linenumber, const std::string& message) : detail::assertion_exception(message, filename, linenumber) {}
      MatcherException(const MatcherException&) = default;
      virtual ~MatcherException() noexcept {}
    };
}}

#endif	// BANDIT_MATCHER_EXCEPTION_H
