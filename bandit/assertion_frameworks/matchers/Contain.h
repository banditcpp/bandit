#ifndef BANDIT_CONTAIN_H
#define BANDIT_CONTAIN_H

#include <cstring>
#include <vector>

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class Contain : public Matcher
    {
    public:
        explicit Contain(const T& element) : Matcher(), _element(element) {}

        template<typename U>
        bool matches(const U& container) const
	{
	    return container.find(_element) != container.end();
	}

        template<typename U>
        bool matches(const std::vector<U>& container) const
	{
	    return std::find(container.begin(), container.end(), _element) != container.end();
	}

	bool matches(const char *const container) const
	{
	    return (_element != NULL) && (container != NULL) && (strstr(container, _element) != NULL);
	}

	bool matches(char *const container) const
	{
	    return (_element != NULL) && (container != NULL) && (strstr(container, _element) != NULL);
	}

    protected:
	std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "contain <" << _element << ">";
	    return ss.str();
	}

    private:
        const T& _element;
    };

    template<typename T>
    Contain<T> contain(const T& element)
    {
        return Contain<T>(element);
    }
}}

#endif	// BANDIT_CONTAIN_H
