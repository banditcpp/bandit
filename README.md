bandit
======

Human friendly unit testing for C++11

Bandit is a framework for C++11 that wants to make working with unit tests a pleasant
experience.

For more information, go to [the bandit website](http://banditcpp.org).

Bandit is released under the [MIT license](LICENSE.md)

#An example

This is a complete test application written in bandit:

```cpp
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

    describe("fuzzbox:", [](){
      guitar_ptr guitar;
      fuzzbox_ptr fuzzbox;

      before_each([&](){
        guitar = guitar_ptr(new struct guitar());
        fuzzbox = fuzzbox_ptr(new struct fuzzbox());
      });

      before_each([&](){
        guitar->add_effect(fuzzbox.get());
      });

      it("starts in clean mode", [&](){
        AssertThat(guitar->sound(), Equals(sounds::clean));
      });

      describe("in distorted mode", [&](){

        before_each([&](){
          fuzzbox->flip();
        });

        it("sounds distorted", [&](){
          AssertThat(guitar->sound(), Equals(sounds::distorted));
        });
      });
    });

});

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
```
