#include <bandit/bandit.h>

using namespace bandit;

int main()
{
  describe("my first spec", [&]() {
    int a;

    before_each([&](){
      cout << "running before_each" << endl;
      a = 99;
    });

    it("should be initialized", [&](){
      cout << "running it" << endl;
      assert(a == 99);
      a = 102;
    });

    describe("nested spec", [&](){

      before_each([&](){
        cout << "running nested before_each" << std::endl;
        a += 3;
      });

      it("should build on outer spec", [&](){
        cout << "running nested it value == " << a << endl;
        assert(a == 102);
        a = 666;
      });

      it("should build on outer spec yet again", [&](){
        cout << "running second nested it value == " << a << endl;
        assert(a == 102);
        a = 667;
      });

    });

    it("should be initialized before each it", [&](){
      cout << "running second it" << endl;
      assert(a == 99);
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
      cout << "it should be 25" << endl;
      assert(b == 25);
    });
  });
  return 0;
}
