#ifndef BANDIT_BENULL_H
#define BANDIT_BENULL_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    class BeNull : public Matcher
    {
    private:
        BeNull& operator=(const BeNull&);

    public:
        BeNull() : Matcher() {}
        ~BeNull() {}

        template<typename U>
        bool matches(U *const & actualValue) const
	{
	    return !actualValue;
	}

    protected:
	std::string failure_message_end() const
	{
	    return std::string("be nil");
	}
    };

    static const BeNull be_null = BeNull();
}}

#endif	// BANDIT_BENULL_H
