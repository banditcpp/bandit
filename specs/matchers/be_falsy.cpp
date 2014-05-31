#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeFalsy)

describe("be_falsy matcher", [&]{
    describe("when the value is a built-in type", [&]{
        bool value;

        describe("which evaluates to false", [&]{
            before_each([&]{
                value = false;
            });

	    it("must accept a positive match", [&]{
		value must be_falsy;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ value must_not be_falsy; }());
            });
        });

        describe("which evaluates to true", [&]{
            before_each([&]{
                value = true;
            });

	    it("must accept a negative match", [&]{
		value must_not be_falsy;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ value must be_falsy; }());
            });
        });
    });

    describe("when the value is nullptr", [&]{
	auto value = nullptr;

	it("must accept a positive match", [&]{
	    value must be_falsy;
	});

	it("must reject a negative match", [&]{
	    AssertThrows(std::exception, [&]{ value must_not be_falsy; }());
	});
    });

    describe("when the value is a pointer", [&]{
        char* value;

        describe("which evaluates to false", [&]{
            before_each([&]{
                value = NULL;
            });

	    it("must accept a positive match", [&]{
		value must be_falsy;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ value must_not be_falsy; }());
	    });
        });

        describe("which evaluates to true", [&]{
            before_each([&]{
                value = (char*)"cat";
            });

	    it("must accept a negative match", [&]{
		value must_not be_falsy;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ value must be_falsy; }());
            });
        });
    });
});

SPEC_END
