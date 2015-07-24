#ifndef BANDIT_BEEMPTY_H
#define BANDIT_BEEMPTY_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    class BeEmpty : public Matcher
    {
    private:
        // BeEmpty & operator=(const BeEmpty &);

    public:
        explicit BeEmpty() : Matcher() {}

        template<typename U>
        bool matches(const U& container) const
	{
	    return container.empty();
	}

    protected:
	std::string failure_message_end() const
	{
	    return std::string("be empty");
	}
    };

    static const BeEmpty be_empty = BeEmpty();
}}

#endif	// BANDIT_BEEMPTY_H
