#ifndef BANDIT_EQUAL_H
#define BANDIT_EQUAL_H

#include <cstring>
#include <memory>

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    std::ostream& operator<<(std::ostream& os, const std::unique_ptr<T>& obj)
    {
	return os << *obj;
    }

    template<typename T>
    class Equal : public Matcher
    {
    private:
        Equal<T>& operator=(const Equal<T>&);

    public:
        explicit Equal(const T& expectedValue) : Matcher(), _expectedValue(expectedValue) {}
        ~Equal() {}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return actualValue == _expectedValue;
	}

	bool matches(char* actualValue) const
	{
	    return strcmp(actualValue, &*_expectedValue) == 0;
	}

	bool matches(const char* actualValue) const
	{
	    return strcmp(actualValue, &*_expectedValue) == 0;
	}

	template<typename U>
	bool matches(const std::unique_ptr<U>& pointer) const
	{
	    return matches(pointer.get());
	}

    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "equal <" << _expectedValue << ">";
	    return ss.str();
	}

    private:
        const T& _expectedValue;
    };

    template<typename T>
    Equal<T> equal(const T& expectedValue)
    {
        return Equal<T>(expectedValue);
    }

#pragma mark equality operators
    template<typename T, typename U>
    bool operator==(const ValueProxy<T>& actualValue, const U& expectedValue)
    {
        return actualValue.to == expectedValue;
    }

    template<typename T, typename U>
    bool operator==(const MatchProxy<T>& matchProxy, const U& expectedValue)
    {
        matchProxy(equal(expectedValue));
        return true;
    }

    template<typename T, typename U>
    bool operator!=(const ValueProxy<T>& actualValue, const U& expectedValue)
    {
        return actualValue.to != expectedValue;
    }

    template<typename T, typename U>
    bool operator!=(const MatchProxy<T>& matchProxy, const U& expectedValue)
    {
        matchProxy.negate()(equal(expectedValue));
        return true;
    }
}}

#endif	// BANDIT_EQUAL_H
