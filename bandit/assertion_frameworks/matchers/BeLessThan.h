#ifndef BANDIT_BELESSTHAN_H
#define BANDIT_BELESSTHAN_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class BeLessThan : public Matcher
    {
    public:
        explicit BeLessThan(const T& expectedValue) : Matcher(), _expectedValue(expectedValue) {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return actualValue < _expectedValue;
	}

    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "be less than <" << _expectedValue << ">";
	    return ss.str();
	}

    private:
        const T& _expectedValue;
    };

    template<typename T>
    BeLessThan<T> be_less_than(const T& expectedValue)
    {
        return BeLessThan<T>(expectedValue);
    }
}}

#endif	// BANDIT_BELESSTHAN_H
