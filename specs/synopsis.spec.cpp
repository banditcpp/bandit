#include <specs/specs.h>

go_bandit([](){
  describe("my first spec", [&]() {
    int a;

    before_each([&](){
      a = 99;
    });

    it("should be initialized", [&](){
      a must equal(99);
      a = 102;
    });

    describe("nested spec", [&](){

      before_each([&](){
        a += 3;
      });

      it("should build on outer spec", [&](){
        a must equal(102);
        a = 666;
      });

      it("should build on outer spec yet again", [&](){
        a must equal(102);
        a = 667;
      });

    });

    it("should be initialized before each it", [&](){
      a must equal(99);
    });
  });

  describe("my second spec", [&](){
    int b;

    before_each([&](){
      b = 22;
    });

    before_each([&](){
      b += 3;
    });

    it("should be 25", [&](){
      b must equal(25);
    });
  });
});
