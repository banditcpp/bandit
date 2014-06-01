#ifndef BANDIT_BEGREATERTHANOREQUAL_H
#define BANDIT_BEGREATERTHANOREQUAL_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class BeGTE : public Matcher
    {
    private:
        BeGTE<T>& operator=(const BeGTE<T>&);

    public:
        explicit BeGTE(const T& expectedValue) : Matcher(), _expectedValue(expectedValue) {}
        ~BeGTE() {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return actualValue >= _expectedValue;
	}

    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "be greater than or equal to <" << _expectedValue << ">";
	    return ss.str();
	}

    private:
        const T& _expectedValue;
    };

    template<typename T>
    BeGTE<T> be_gte(const T& expectedValue)
    {
        return BeGTE<T>(expectedValue);
    }

    template<typename T>
    BeGTE<T> be_greater_than_or_equal_to(const T& expectedValue)
    {
        return be_gte(expectedValue);
    }
}}

#endif	// BANDIT_BEGREATERTHANOREQUAL_H
