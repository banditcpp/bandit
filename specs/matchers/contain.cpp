#include <set>

#include <specs/specs.h>

using namespace bandit::Matchers;

SPEC_BEGIN(Matchers::Contain)

describe("contain matcher", [&]{
    std::string element0("element0");
    std::string element1("element1");

    describe("when the container is an STL vector", [&]{
        describe("which contains the element", [&]{
            std::vector<std::string> container {element0, element1};

	    it("must pass a positive match", [&]{
		container must contain(element1);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not contain(element1); }());
	    });
        });

        describe("which does not contain the element", [&]{
            std::vector<int> container;

	    it("must pass a negative match", [&]{
		container must_not contain(4);
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain(4); }());
	    });
        });
    });

    describe("when the container is an STL map", [&]{
        describe("which contains the expected key", [&]{
            std::map<int, int> container {{5, 6}, {7,10}};

	    it("must pass a positive match", [&]{
		container must contain(5);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not contain(5); }());
	    });
        });

        describe("which does not contain the expected value", [&]{
            std::map<int, int> container;

	    it("must pass a negative match", [&]{
		container must_not contain(6);
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain(6); }());
	    });
        });
    });

    describe("when the container is an STL set", [&]{
        describe("which contains the element", [&]{
            std::set<int> container {5, 7};

	    it("must pass a positive match", [&]{
		container must contain(7);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not contain(7); }());
	    });
        });

        describe("which does not contain the element", [&]{
            std::set<int> container;

	    it("must pass a negative match", [&]{
		container must_not contain(7);
	    });

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain(7); }());
	    });
        });
    });

    describe("when the container is a C string", [&]{
        describe("which is null", [&]{
            char* container = NULL;

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain("foo"); }());
	    });
        });

        describe("which contains the substring", [&]{
            char* container = (char*)"jack and jill";
            char* element = (char*)"jack";

	    it("must pass a positive match", [&]{
		container must contain(element);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not contain(element); }());
	    });
        });

        describe("which does not contain the substring", [&]{
            char* container = (char*)"batman and robin";
            char* element = (char*)"catwoman";

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain(element); }());
	    });

	    it("must pass a negative match", [&]{
		container must_not contain(element);
	    });
        });
    });

    describe("when the container is a const C string", [&]{
        describe("which contains the substring", [&]{
            const char* container = (char*)"jack and jill";
            const char* element = (char*)"jack";

	    it("must pass a positive match", [&]{
		container must contain(element);
	    });

	    it("must reject a negative match", [&]{
		AssertThrows(std::exception, [&]{ container must_not contain(element); }());
	    });
        });

        describe("which does not contain the substring", [&]{
            const char* container = (char*)"batman and robin";
            const char* element = (char*)"catwoman";

	    it("must reject a positive match", [&]{
		AssertThrows(std::exception, [&]{ container must contain(element); }());
	    });

	    it("must pass a negative match", [&]{
		container must_not contain(element);
	    });
        });
    });
});

SPEC_END
