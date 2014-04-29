#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeNull)

describe("be_null matcher", [&]{
    describe("when the value is a pointer to a built-in type", [&]{
        int* value;

        describe("which is NULL", [&]{
            before_each([&]{
                value = NULL;
            });

	    it("must pass a positive match", [&]{
		value must be_null;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ value must_not be_null; }());
	    });
        });

        describe("which is not NULL", [&]{
            int i = 7;

            before_each([&]{
                value = &i;
            });

	    it("must pass a negative match", [&]{
		value must_not be_null;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ value must be_null; }());
	    });
        });
    });
});

SPEC_END
