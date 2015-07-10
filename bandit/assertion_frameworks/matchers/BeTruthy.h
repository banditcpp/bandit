#ifndef BANDIT_BETRUTHY_H
#define BANDIT_BETRUTHY_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    class BeTruthy : public Matcher
    {
    public:
        BeTruthy() : Matcher() {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return !!actualValue;
	}

        bool matches(const std::nullptr_t&) const
	{
	    return false;
	}


    protected:
        virtual std::string failure_message_end() const
	{
	    return std::string("evaluate to true");
	}
    };

    static const BeTruthy be_truthy = BeTruthy();
}}

#endif	// BANDIT_BETRUTHY_H
