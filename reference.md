---
layout: index
---
#Reference

##Entry point

This is the entry point for a bandit application:

{% highlight cpp %}
#include <bandit/bandit.h>

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
{% endhighlight %}

##Assertions
Bandit uses the [snowhouse](https://github.com/joakimkarlsson/snowhouse#assertions) 
assertion library. Go there for references on how to write assertions.

##go_bandit()

The `go_bandit()` construct registers a function that contains tests. There
can be one `go_bandit()` construct per .cpp file.

The boilerplate for each .cpp file containing tests should look like this:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){
  describe("a thing", [&](){
    it("should do something", [&](){
      AssertThat(something, IsTrue());
    });
  });
});
{% endhighlight %}

##describe()

This function is setting up the context for a test. That is, declaring variables
for holding the objects that we need for our tests. You can have several levels
of nested `describe()` where each level specifies specific cases of its parent
level.

##describe_skip()
TBD

##it()
TBD

##it_skip()
TBD

##before_each()
TBD

##after_each()
TBD
