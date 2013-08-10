---
layout: index
---
Bandit is a framework for C++11 that wants to make working with unit tests a 
pleasant experience.

Bandit is released under the 
[MIT license](https://github.com/joakimkarlsson/bandit/blob/master/LICENSE.md)

#An example

This is a complete test application written in bandit:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){
    describe("fuzzbox:", [](){
      guitar_ptr guitar;
      fuzzbox_ptr fuzzbox;

      before_each([&](){
        guitar = guitar_ptr(new struct guitar());
        fuzzbox = fuzzbox_ptr(new struct fuzzbox());
        guitar->add_effect(*fuzzbox);
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
{% endhighlight %}

#Installing

Bandit is header only so there is no need for additional compilation before you
can start using it. Download bandit and add its root directory to your project's
include directories and you're ready to go.

#Compilers

Bandit has been tested with GCC 4.7.3, Clang 3.2.0, and Visual Studio 2012. It 
doesn't work with Visual Studio 2010. 

We'll add more compilers to the list as we test them. If you want to see if 
bandit works for your compiler, bandit is shipped with a cmake project for 
generating bandit's self tests. 

If your compiler doesn't support the C++11 features required by Bandit, we 
suggest that you take a look at [Igloo](http://igloo-testing.org), which is 
built on the same philosophy but works without C++11.
