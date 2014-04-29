#ifndef BANDIT_VALUEPROXY_H
#define BANDIT_VALUEPROXY_H

#include "MatchProxy.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class ValueProxy
    {
    private:
        template<typename U>
        ValueProxy(const ValueProxy<U> &);
        template<typename U>
        ValueProxy & operator=(const ValueProxy<U> &);

    public:
        MatchProxy<T> to;
        MatchProxy<T> to_not;

        explicit ValueProxy(const char* filename, int lineNumber, const T& value) : to(*this), to_not(*this, true), _value(value), _filename(filename), _lineNumber(lineNumber) {}

    private:
        friend class MatchProxy<T>;

        const T&    _value;
        std::string _filename;
        int         _lineNumber;
    };
}}

#endif	// BANDIT_VALUEPROXY_H
