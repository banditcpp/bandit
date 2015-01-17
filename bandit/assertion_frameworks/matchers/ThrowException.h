#ifndef BANDIT_THROWEXCEPTION_H
#define BANDIT_THROWEXCEPTION_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template <typename T>
    class ThrowException : public Matcher
    {
    public:
        ThrowException() : Matcher(), _allow_subclasses(false) {}
        explicit ThrowException(bool allow_subclasses) : Matcher(), _allow_subclasses(allow_subclasses) {}

	template <typename U = std::exception>
        ThrowException<U> operator()() const
	{
	    return ThrowException<U>();
	}

        ThrowException& or_subclass()
	{
	    _allow_subclasses = true;
	    return *this;
	}

	template <typename U>
        bool matches(const U& block) const
	{
	    try
	    {
		block();
	    }
	    catch(const T& e)
	    {
		return true;
	    }
	    catch(...)	// Wrong exception
	    {
		_exception = std::current_exception();
	    }

	    return false;
	}

    protected:
        std::string failure_message_end() const
	{
	    return std::string("throw an exception");
	}

    private:
        bool	_allow_subclasses;
	mutable std::exception_ptr   _exception;
    };

    static const ThrowException<std::exception> throw_exception;
}}

#endif	// BANDIT_THROWEXCEPTION_H
