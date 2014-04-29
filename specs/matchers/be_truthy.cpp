#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeTruthy)

describe("be_truthy matcher", [&]{
    describe("when the value is a built-in type", [&]{
        bool value;

        describe("which evaluates to false", [&]{
            before_each([&]{
                value = false;
            });

	    it("must accept a negative match", [&]{
		value must_not be_truthy;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ value must be_truthy; }());
            });
        });

        describe("which evaluates to true", [&]{
            before_each([&]{
                value = true;
            });

	    it("must accept a positive match", [&]{
		value must be_truthy;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ value must_not be_truthy; }());
            });
        });
    });

    describe("when the value is nullptr", [&]{
	auto value = nullptr;

	it("must accept a negative match", [&]{
	    value must_not be_truthy;
	});

	it("must reject a positive match", [&]{
	    AssertThrows(std::exception, [&]{ value must be_truthy; }());
	});
    });

    describe("when the value is a pointer", [&]{
        char* value;

        describe("which evaluates to false", [&]{
            before_each([&]{
                value = NULL;
            });

	    it("must accept a negative match", [&]{
		value must_not be_truthy;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ value must be_truthy; }());
	    });
        });

        describe("which evaluates to true", [&]{
            before_each([&]{
                value = (char*)"cat";
            });

	    it("must accept a positive match", [&]{
		value must be_truthy;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ value must_not be_truthy; }());
            });
        });
    });
});

SPEC_END
