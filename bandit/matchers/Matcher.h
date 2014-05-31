#ifndef BANDIT_MATCHER_H
#define BANDIT_MATCHER_H

#include <sstream>

//#import "CedarStringifiers.h"

namespace bandit { namespace Matchers {
    class Matcher
    {
    private:
        Matcher & operator=(const Matcher &);

    public:
        Matcher() {}
        virtual ~Matcher() {};

        template<typename U>
	std::string failure_message(const U& value, bool negate) const
	{
	    std::ostringstream ss;
	    ss << "Expected <" << value << "> " << (negate ? "to not " : "to ") << failure_message_end();
	    return ss.str();
	}

	std::string failure_message(char *const value, bool negate) const
	{
	    return failure_message((value ? value : "NULL"), negate);
	}

        template<typename U>
	std::string failure_message(const std::unique_ptr<U>& value, bool negate) const
	{
	    return failure_message(value.get(), negate);
	}

	std::string failure_message(const std::nullptr_t pointer, bool negate) const
	{
	    (void)pointer;
	    return failure_message("nullptr", negate);
	}

	template<typename U>
	std::string failure_message(std::function<U>& value, bool negate) const
	{
	    return failure_message(typeid(value).name(), negate);
	}

	template<typename U>
	std::string failure_message(const std::function<U>& value, bool negate) const
	{
	    return failure_message(typeid(value).name(), negate);
	}

	template<typename U, template <class T, class = std::allocator<T> > class container >
	std::string failure_message(const container<U>& value, bool negate) const
	{
	    return failure_message(typeid(value).name(), negate);
	}

	template<typename U, template <class T, class = std::less<T>, class = std::allocator<T> > class container >
	std::string failure_message(const container<U>& value, bool negate) const
	{
	    return failure_message(typeid(value).name(), negate);
	}

	template<typename U, typename V, template <class K, class T, class = std::less<K>, class = std::allocator<std::pair<const K,T>> > class container >
	std::string failure_message(const container<U,V>& value, bool negate) const
	{
	    return failure_message(typeid(value).name(), negate);
	}

    protected:
        virtual std::string failure_message_end() const = 0;
    };
}}

#endif	// BANDIT_MATCHER_H
