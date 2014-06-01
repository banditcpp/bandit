#ifndef BANDIT_BECLOSETO_H
#define BANDIT_BECLOSETO_H

#include "Matcher.h"

namespace bandit { namespace Matchers {

    template<typename T>
    class BeCloseTo : public Matcher
    {
    public:
        explicit BeCloseTo(const T& expectedValue): Matcher(), _expectedValue(expectedValue), _threshold(0.01) {}

        BeCloseTo<T>& within(float threshold)
	{
	    _threshold = threshold;
	    return *this;
	}

        template<typename U>
        bool matches(const U& actualValue) const
	{
	    return this->subtractable_types_match(actualValue, _expectedValue);
	}


    protected:
        virtual std::string failure_message_end() const
	{
	    std::ostringstream ss;
	    ss << "be close to <" << _expectedValue << ">" << " (within " << _threshold << ")";
	    return ss.str();
	}

    private:
        template<typename U, typename V>
        bool subtractable_types_match(const U& actualValue, const V& expectedValue) const
	{
	    return (actualValue > (expectedValue - _threshold)) && (actualValue < (expectedValue + _threshold));
	}


    private:
        const T& _expectedValue;
        float _threshold;
    };

    template<typename T>
    BeCloseTo<T> be_close_to(const T& expectedValue)
    {
        return BeCloseTo<T>(expectedValue);
    }
}}

#endif	// BANDIT_BECLOSETO_H
