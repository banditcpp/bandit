#ifndef BANDIT_BEGREATERTHAN_H
#define BANDIT_BEGREATERTHAN_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class BeGreaterThan : public Matcher
    {
    public:
        explicit BeGreaterThan(const T& expectedValue) : Matcher(), _expectedValue(expectedValue) {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return actualValue > _expectedValue;
	}

    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "be greater than <" << _expectedValue << ">";
	    return ss.str();
	}

    private:
        const T& _expectedValue;
    };

    template<typename T>
    BeGreaterThan<T> be_greater_than(const T& expectedValue)
    {
        return BeGreaterThan<T>(expectedValue);
    }
}}

#endif	// BANDIT_BEGREATERTHAN_H
