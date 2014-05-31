#include <set>

#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::BeEmpty)

describe("be_empty matcher", [&]{
    describe("when the value is an STL vector", [&]{
        describe("which is empty", [&]{
            std::vector<int> container;

	    it("must pass a positive match", [&]{
		container must be_empty;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not be_empty; }());
	    });
        });

        describe("which is not empty", [&]{
            std::vector<int> container {2, 7};

	    it("must pass a negative match", [&]{
		container must_not be_empty;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must be_empty; }());
	    });
        });
    });

    describe("when the value is an STL map", [&]{
        describe("which is empty", [&]{
            std::map<int, int> container;

	    it("must pass a positive match", [&]{
		container must be_empty;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not be_empty; }());
	    });
        });

        describe("which is not empty", [&]{
            std::map<int, int> container {{5, 6}, {7,10}};

	    it("must pass a negative match", [&]{
		container must_not be_empty;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must be_empty; }());
	    });
        });
    });

    describe("when the value is an STL set", [&]{
        describe("which is empty", [&]{
            std::set<int> container;

	    it("must pass a positive match", [&]{
		container must be_empty;
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not be_empty; }());
	    });
        });

        describe("which is not empty", [&]{
            std::set<int> container {5, 7};

	    it("must pass a negative match", [&]{
		container must_not be_empty;
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must be_empty; }());
	    });
        });
    });
});

SPEC_END
