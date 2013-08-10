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

This function is similar to the concept of a fixture in xUnit based frameworks.

{% highlight cpp %}
describe("a calculator", [&](){
  calculator calc;

  describe("in scientific mode", [&](){
    // Additional setup and tests.

    describe("in hex mode", [&](){
      // Additional setup and tests.
    });
  });

  describe("in basic mode", [&](){
    // Additional setup and tests.
  });
});
{% endhighlight %}

##describe_skip()
This tells bandit to skip running all `it()` functions in this `describe()` and
all its nested describes. Bandit will report the number of skipped `it()` functions
after the test run has completed.

##it()
This function describes something that should hold true for the component we're
describing.

`it()` is similar to a test method in xUnit based frameworks.

##it_skip()
This causes the function to be skipped during a test run. The number of skipped
functions will be reported by the bandit executable after a test run.

##before_each()
This function is used to set up all prerequisites needed before we can start 
testing. `before_each()` is called before each `it()` during a test run. This 
way you know that each `it()` function has access to a fresh new context that
hasn't been contaminated by other tests.

You can have several `before_each()` methods in a `describe()` function. They
will be called in the order they are declared before each `it()` function.

{% highlight cpp %}
describe("a calculator", [&](){
  calculator_ptr calc;

  before_each([&](){
    // Make sure each 'it()' gets a fresh new
    // calculator to work with.
    calc = calculator_ptr(new calculator());
  });

  it("can add", [&](){
    AssertThat(calculator->add(3,2), Equals(5));
  });

  // More tests...
});
{% endhighlight %}

When you have nested `describe()` functions, bandit will call all `before_each()`
for the outermost context first, then all for it child context until each reaches
the context containing the current `it()`.

**NOTE:** *All `before_each()` must be declared before any `it()` or nested `describe()`.
Otherwise bandit cannot guarantee that everything is set up correctly and will
complain with an error message*

##after_each()
Sometimes (more rarely than you might think) you need to clean up after each
test. `after_each()` gets called after each `it()` function.

**NOTE:** *All `after_each()` must be declared before any `it()` or nested `describe()`.
Otherwise bandit cannot guarantee that everything is set up correctly and will
complain with an error message*
