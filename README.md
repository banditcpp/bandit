bandit
======

A BDD style unit testing framework for C++11

This is still an experiment in using C++11. I ended up with a framework similar
to [RSpec](https://github.com/joakimkarlsson/snowhouse.git), [Mocha](http://visionmedia.github.io/mocha/) or [Jasmine](http://pivotal.github.io/jasmine/) from other environments.

bandit is a test framework. It is (going to be) agnostic of the assertion 
framework to use. Right now it uses [snowhouse](https://github.com/joakimkarlsson/snowhouse) for its internal testing. [Snowhouse](https://github.com/joakimkarlsson/snowhouse) is the assertion framework used by [igloo](https://github.com/joakimkarlsson/igloo).

As bandit makes heavy use of lambdas it does not have to do a whole lot of preprocessor magic to get things done. The only macro
right now is the run_bandit() construct which is used to register which specs to run.

Synopsis:

```cpp
#include <bandit/bandit.h>
using namespace bandit;

#include <snowhouse/snowhouse.h>
using namespace snowhouse;

run_bandit([](){

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
```
