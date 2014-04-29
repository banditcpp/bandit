#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeLTE)

describe("be_lte matcher", [&]{
    int someInteger = 10;

    describe("when the actual value is a built-in type", [&]{
        int actualValue = someInteger;

        describe("and the expected value is the same built-in type", [&]{
            int expectedValue;

            describe("and the actual value is greater than the expected value", [&]{
                before_each([&]{
                    expectedValue = 1;
                });

		it("must pass a negative match", [&]{
		    actualValue must_not be_lte(expectedValue);
		    actualValue must_not be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a positive match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must be_less_than_or_equal_to(expectedValue); }());
		});
            });

            describe("and the actual value is less than the expected value", [&]{
                before_each([&]{
                    expectedValue = 100;
                });

		it("must pass a positive match", [&]{
		    actualValue must be_lte(expectedValue);
		    actualValue must be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must_not be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must_not be_less_than_or_equal_to(expectedValue); }());
		});
            });

            describe("and the actual value equals the expected value", [&]{
                before_each([&]{
                    expectedValue = actualValue;
                });

		it("must pass a positive match", [&]{
		    actualValue must be_lte(expectedValue);
		    actualValue must be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must_not be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must_not be_less_than_or_equal_to(expectedValue); }());
		});
            });
        });

        describe("and the expected value is a different, but comparable, built-in type", [&]{
            float expectedValue;

            describe("and the actual value is greater than the expected value", [&]{
                before_each([&]{
                    expectedValue = 1.1;
                });

		it("must pass a negative match", [&]{
		    actualValue must_not be_lte(expectedValue);
		    actualValue must_not be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a positive match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must be_less_than_or_equal_to(expectedValue); }());
		});
            });

            describe("and the actual value is less than the expected value", [&]{
                before_each([&]{
                    expectedValue = 100.1;
                });

		it("must pass a positive match", [&]{
		    actualValue must be_lte(expectedValue);
		    actualValue must be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must_not be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must_not be_less_than_or_equal_to(expectedValue); }());
		});
            });

            describe("and the actual value equals the expected value", [&]{
                before_each([&]{
                    expectedValue = someInteger / 1.0;
                });

		it("must pass a positive match", [&]{
		    actualValue must be_lte(expectedValue);
		    actualValue must be_less_than_or_equal_to(expectedValue);
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must_not be_lte(expectedValue); }());
		    AssertThrows(std::exception, [&]{ actualValue must_not be_less_than_or_equal_to(expectedValue); }());
		});
            });
        });
    });
});

SPEC_END
