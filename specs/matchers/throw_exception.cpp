#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::ThrowException)

describe("throw_exception", []{
    describe("when no exception is specified", [&]{
	std::exception exception;

	std::function<void()> exception_block = [&]{ throw exception; };

        describe("when the block throws an exception", [&]{
	    it("must pass a positive match", [&]{
		exception_block must throw_exception;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(MatcherException, [&]{ exception_block must_not throw_exception; }());
	    });
        });

        describe("when the block does not throw an exception", [&]{
	    std::function<void()> quiet_block = [&]{};

	    it("must pass a negative match", [&]{
		quiet_block must_not throw_exception;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ quiet_block must throw_exception; }());
	    });
        });
    });

    describe("with an exception class specified", [&]{
	std::logic_error expected_exception("logic_error");
	using exp_type = decltype(expected_exception);

        describe("when the block throws the expected exception", [&]{
	    std::function<void()> exception_block = [&]{ throw expected_exception; };

	    it("must pass a positive match", [&]{
		exception_block must throw_exception.operator()<exp_type>();
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ exception_block must_not throw_exception.operator()<exp_type>(); }());
	    });
        });

	// TODO: Because C++ lacks reflection, there's no way to implement
	//  subclass-checking. I'm leaving these tests here for when the
	//  language has evolved sufficiently.
        xdescribe("when the block throws a sublass of the specified exception", [&]{
	    std::function<void()> subclass_block = [&]{ throw std::invalid_argument("invalid argument"); };

            describe("when subclasses are expected", [&]{
		it("must pass a positive match", [&]{
		    subclass_block must throw_exception.operator()<exp_type>().or_subclass();
		});

		it("must reject a negative match", [&]{
		    AssertThrows(std::exception, [&]{ subclass_block must_not throw_exception.operator()<exp_type>().or_subclass(); }());
		});
            });

            describe("when subclasses are not expected", [&]{
		it("must pass a negative match", [&]{
		    subclass_block must_not throw_exception.operator()<exp_type>();
		});

		it("must reject a positive match", [&]{
		    AssertThrows(std::exception, [&]{ subclass_block must throw_exception.operator()<exp_type>(); }());
		});
            });
        });

        describe("when the block throws an unrelated exception", [&]{
	    std::function<void()> unrelated_block = [&]{ throw std::range_error("range error"); };

	    it("must pass a negative match", [&]{
		unrelated_block must_not throw_exception.operator()<exp_type>();
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ unrelated_block must throw_exception.operator()<exp_type>(); }());
	    });
        });

        describe("when the block does not throw an exception", [&]{
	    std::function<void()> quiet_block = [&]{};

	    it("must pass a negative match", [&]{
		quiet_block must_not throw_exception.operator()<exp_type>();
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ quiet_block must throw_exception.operator()<exp_type>(); }());
	    });
        });
    });
});

SPEC_END
