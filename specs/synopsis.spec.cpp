#include <specs/specs.h>

go_bandit([](){
  describe("my first spec", [&]() {
    int a;

    before_each([&](){
      a = 99;
    });

    it("should be initialized", [&](){
      AssertThat(a, Equals(99));
      a = 102;
    });

    describe("nested spec", [&](){

      before_each([&](){
        a += 3;
      });

      it("should build on outer spec", [&](){
        AssertThat(a, Equals(102));
        a = 666;
      });

      it("should build on outer spec yet again", [&](){
        AssertThat(a, Equals(102));
        a = 667;
      });

    });

    it("should be initialized before each it", [&](){
      AssertThat(a, Equals(99));
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
      AssertThat(b, Equals(25));
    });
  });
});
