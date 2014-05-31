#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeCloseTo)

describe("be_close_to matcher", []{
    describe("when the actual value is declared as a float", [&]{
        float actualValue = 2.0 / 3.0;

        describe("and the expected value is also a float", [&]{
            float expectedValue;

            describe("with an explicit threshold", [&]{
                float threshold = 0.1;

                describe("and the values are within the given threshold", [&]{
                    before_each([&]{
                        expectedValue = 2.0 / 3.0 + 0.01;
                    });

		    it("must accept a positive match", [&]{
			actualValue must be_close_to(expectedValue).within(threshold);
		    });

		    it("must reject a negative match", [&]{
			AssertThrows(std::exception, [&]{ actualValue must_not be_close_to(expectedValue).within(threshold); }());
		    });
                });

                describe("and the values are not within the given threshold", [&]{
                    before_each([&]{
                        expectedValue = 2.0 / 3.0 + 0.2;
                    });

		    it("must accept a negative match", [&]{
			actualValue must_not be_close_to(expectedValue).within(threshold);
		    });

		    it("must reject a positive match", [&]{
			AssertThrows(std::exception, [&]{ actualValue must be_close_to(expectedValue).within(threshold); }());
		    });
                });
            });

            describe("without an explicit threshold", [&]{
                describe("and the values are within the default threshold", [&]{
                    before_each([&]{
                        expectedValue = 2.0 / 3.0 + 0.000001;
                    });

		    it("must accept a positive match", [&]{
			actualValue must be_close_to(expectedValue);
		    });

		    it("must reject a negative match", [&]{
			AssertThrows(std::exception, [&]{ actualValue must_not be_close_to(expectedValue); }());
		    });
                });

                describe("and the values are not within the default threshold", [&]{
                    before_each([&]{
                        expectedValue = 2.0 / 3.0 + 0.1;
                    });

		    it("must accept a negative match", [&]{
			actualValue must_not be_close_to(expectedValue);
		    });

		    it("must reject a positive match", [&]{
			AssertThrows(std::exception, [&]{ actualValue must be_close_to(expectedValue); }());
		    });
                });
            });
        });

        describe("and the expected value is a compatible non-float type", [&]{
            int expectedValue;
            float threshold = 1;

            describe("and the values are within the given threshold", [&]{
                before_each([&]{
                    expectedValue = 1;
                });

		it("must accept a positive match", [&]{
		    actualValue must be_close_to(expectedValue).within(threshold);
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must_not be_close_to(expectedValue).within(threshold); }());
		});
            });

            describe("and the values are not within the given threshold", [&]{
                before_each([&]{
                    expectedValue = 5;
                });

		it("must accept a negative match", [&]{
		    actualValue must_not be_close_to(expectedValue).within(threshold);
		});

		it("must reject a positive match", [&]{
		    AssertThrows(std::exception, [&]{ actualValue must be_close_to(expectedValue).within(threshold); }());
		});
            });
        });
    });
});

SPEC_END
