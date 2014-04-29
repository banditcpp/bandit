#ifndef BANDIT_BELESSTHANOREQUAL_H
#define BANDIT_BELESSTHANOREQUAL_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class BeLTE : public Matcher
    {
    private:
        BeLTE<T>& operator=(const BeLTE<T>&);

    public:
        explicit BeLTE(const T& expectedValue) : Matcher(), _expectedValue(expectedValue) {}
        ~BeLTE() {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return actualValue <= _expectedValue;
	}

    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "be less than or equal to <" << _expectedValue << ">";
	    return ss.str();
	}

    private:
        const T& _expectedValue;
    };

    template<typename T>
    BeLTE<T> be_lte(const T& expectedValue)
    {
        return BeLTE<T>(expectedValue);
    }

    template<typename T>
    BeLTE<T> be_less_than_or_equal_to(const T& expectedValue)
    {
        return be_lte(expectedValue);
    }
}}

#endif	// BANDIT_BELESSTHANOREQUAL_H
