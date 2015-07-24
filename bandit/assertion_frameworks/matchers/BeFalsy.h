#ifndef BANDIT_BEFALSY_H
#define BANDIT_BEFALSY_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    class BeFalsy : public Matcher
    {
    private:
        // BeFalsy& operator=(const BeFalsy&);

    public:
        explicit BeFalsy() : Matcher() {}
        // ~BeFalsy() {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return !actualValue;
	}

        bool matches(const std::nullptr_t&) const
	{
	    return true;
	}


    protected:
        virtual std::string failure_message_end() const
	{
	    return std::string("evaluate to false");
	}
    };

    static const BeFalsy be_falsy = BeFalsy();
}}

#endif	// BANDIT_BEFALSY_H
