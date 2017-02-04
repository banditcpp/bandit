---
layout: index
---
# Writing Tests

With bandit you create a command line application that runs all registered
tests, reports the result on stdout, and then exits with an error level 0 if all
tests passed, and an error level greater than 0 if some tests did not passed.

Every application needs an entry point, and for bandit it should look like this:

```c++
#include <bandit/bandit.h>

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
```

The `run()` function will run all registered tests and report the result.

If you build and run this, bandit will report the following and exit with an
error code:

```bash
$ specs

Could not find any tests.
```

This means bandit is running fine, we just haven't added any tests for it to
run yet.

## Adding Tests

Once you have a command line application with an entry point that delegates to
bandit, it's time to start writing some tests.

To make sure everything is working, we begin by creating a failing test. That
way we know that our tests actually gets executed and can verify that they fail
for the right reasons.

We add a new .cpp file to our project with the following content:

```c++
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

  describe("our first test", [](){
    it("should fail", [&](){
      AssertThat(5, Equals(6));
    });
  });

});
```

If we compile and run this, we get the following response:

```bash
$ specs
F
There were failures!

our first test should fail:
/<path to our project>/first.spec.cpp:8: Excpected equal to 6
Actual: 5

Test run complete. 1 tests run. 0 succeeded. 1 failed.
```

This means bandit was able to find our test and execute it.

## What we did

After including bandit and using its namespace by default, we tell bandit that
this file includes tests that we want to be part of the execution by using the
`go_bandit()` construct.

Inside `go_bandit()` we can now write our tests.

Bandit takes a lot of inspiration from frameworks like RSpec, where our tests
take the form of specifications that describes how a component should work. These
specifications can be executed to verify that your code fulfills them.

This is conveyed by the 'describe/it' syntax.

`describe()` specifies what we're testing, and `it()` specifies what should hold
true for that thing.

A slightly more elaborate example:

```c++
describe("a calculator", [](){
  calculator calc;

  it("can add", [&](){
    AssertThat(calc.add(2, 3), Equals(5));
  });

  it("can subtract", [&](){
    AssertThat(calc.subtract(2, 3), Equals(-1));
  });
});
```

Here we describe a calculator and specify that it can add and subtract.

## Assertions

Bandit uses the [snowhouse](https://github.com/banditcpp/snowhouse#assertions)
assertion framework to verify that things works as expected. Snowhouse contains
a lot of helpers that aims to create readable tests.
