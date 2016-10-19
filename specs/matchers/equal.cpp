#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::Equal)

describe("when the actual value is a built-in type", []{
    int actualValue = 1;

    describe("and the expected value is the same built-in type", [&]{
	int expectedValue;

	describe("and the values are equal", [&]{
	    before_each([&]{
		expectedValue = 1;
	    });

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue); }());
	    });
	});

	describe("and the values are not equal", [&]{
	    before_each([&]{
		expectedValue = 147;
	    });

	    it("must accept a negative match", [&]{
		actualValue must_not equal(expectedValue);
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must equal(expectedValue); }());
	    });
	});
    });

    describe("and the expected value is a different, but comparable, built-in type", [&]{
	long int expectedValue;

	describe("and the values are equal", [&]{
	    before_each([&]{
		expectedValue = 1;
	    });

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue); }());
	    });
	});

	describe("and the values are not equal", [&]{
	    before_each([&]{
		expectedValue = 42;
	    });

	    it("must accept a negative match", [&]{
		actualValue must_not equal(expectedValue);
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must equal(expectedValue); }());
	    });
	});
    });
});

describe("when the actual value is declared as a C string", []{
    char *actualValue = (char*)"actualValue";
    const size_t len = strlen(actualValue);

    describe("and the expected value is declared as a C string", [&]{
	std::unique_ptr<char> expectedValue;

	before_each([&]{
	    expectedValue.reset((char*)calloc(len + 1, sizeof(char)));
	});

	describe("and the values are equal", [&]{
	    before_each([&]{
		std::copy(actualValue, actualValue + len, expectedValue.get());
	    });

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue.get());
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue.get()); }());
	    });
	});

	describe("and the values are not equal", [&]{
	    before_each([&]{
		std::copy(actualValue, actualValue+len, expectedValue.get());
		*(expectedValue.get()+1) = 'b'; // "abtualValue"
	    });

	    it("must accept a negative match", [&]{
		actualValue must_not equal(expectedValue.get());
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must equal(expectedValue.get()); }());
	    });
	});
    });

    describe("and the expected value is declared as a const C string", [&]{
	const char *expectedValue;

	describe("and the values are equal", [&]{
	    before_each([&]{
		expectedValue = "actualValue";
	    });

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue); }());
	    });
	});
    });

    describe("when the expected value is a unique_ptr to a C string", [&]{
	std::unique_ptr<char> expectedValue;

	before_each([&]{
	    expectedValue.reset((char*)calloc(len + 1, sizeof(char)));
	});

	describe("and the values are equal", [&]{
	    before_each([&]{
		std::copy(actualValue, actualValue + len, expectedValue.get());
	    });

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue); }());
	    });
	});
    });
});

describe("when the actual value is a unique_ptr", []{
    std::unique_ptr<char> actualValue;
    auto expectedValue = (char*)"expectedValue";
    const size_t len = strlen(expectedValue);

    before_each([&]{
	actualValue.reset((char*)calloc(len + 1, sizeof(char)));
    });

    describe("when the strings are equal", [&]{
	before_each([&]{
	    std::copy(expectedValue, expectedValue + len, actualValue.get());
	});

	it("must accept a positive match", [&]{
	    actualValue must equal(expectedValue);
	});
    });

    describe("when the strings are not equal", [&]{
	before_each([&]{
	    std::copy(expectedValue, expectedValue + len, actualValue.get());
	    *(actualValue.get() + 4) = 'b'; // "expebtedValue"
	});

	it("must accept a negative match", [&]{
	    actualValue must_not equal(expectedValue);
	});
    });
});

describe("when the actual value is declared as char array", []{
    describe("and the expected value is declared as a C string", []{
	char actualValue[] = "actualValue";

	describe("and the values are equal", [&]{
	    char* expectedValue = (char*)"actualValue";

	    it("must accept a positive match", [&]{
		actualValue must equal(expectedValue);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must_not equal(expectedValue); }());
	    });
	});

	describe("and the values are not equal", [&]{
	    char* expectedValue = (char*)"expectedValue";

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ actualValue must equal(expectedValue); }());
	    });

	    it("must accept a negative match", [&]{
		actualValue must_not equal(expectedValue);
	    });
	});
    });
});

SPEC_END
