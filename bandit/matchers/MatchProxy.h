#ifndef BANDIT_MATCHPROXY_H
#define BANDIT_MATCHPROXY_H

#include "MatcherException.h"

namespace bandit { namespace Matchers
{
    template<typename T> class ValueProxy;

    template<typename T>
    class MatchProxy
    {
    private:
        template<typename U>
        MatchProxy(const MatchProxy<U>&);

        template<typename U>
        MatchProxy& operator=(const MatchProxy<U>&);

    public:
        explicit MatchProxy(const ValueProxy<T>& value, bool negate_ = false) : _value(value), _negate(negate_) {}

        template<typename MatcherType>
	void operator()(const MatcherType& matcher) const
        {
	    if( matcher.matches(_value._value) == _negate )
	    {
		throw MatcherException(_value._filename, _value._lineNumber, matcher.failure_message(_value._value, _negate));
	    }
        }

        MatchProxy<T> negate() const
        {
            return MatchProxy<T>(_value, !_negate);
        }

    private:
        const ValueProxy<T>& _value;
        bool _negate;
    };
}}

#endif	// BANDIT_MATCHPROXY_H
