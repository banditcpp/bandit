bandit
======
[![MIT License](https://img.shields.io/badge/license-MIT%20License-blue.svg)](LICENSE.txt)
[![Conan Center](https://img.shields.io/conan/v/bandit)](https://conan.io/center/recipes/bandit)
[![Codacy quality grade](https://app.codacy.com/project/badge/Grade/d86badcd05184ce79d715cf7f22722ed)](https://app.codacy.com/gh/banditcpp/bandit/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Contributors](https://img.shields.io/github/contributors/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/graphs/contributors)
[![Watchers](https://img.shields.io/github/watchers/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/watchers)
[![Stars](https://img.shields.io/github/stars/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/stargazers)
[![Forks](https://img.shields.io/github/forks/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/network/members)
[![Commits per year](https://img.shields.io/github/commit-activity/y/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/graphs/code-frequency)
[![Last commit](https://img.shields.io/github/last-commit/banditcpp/bandit/main.svg)](https://github.com/banditcpp/bandit/commits/main)
[![Issues](https://img.shields.io/github/issues/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/banditcpp/bandit.svg)](https://github.com/banditcpp/bandit/pulls)

Human-friendly unit testing for C++11

Bandit is a framework for C++11 that wants to make working with unit tests a
pleasant experience.

## An example

This is a complete test application written in bandit:

```c++
#include <bandit/bandit.h>

using namespace snowhouse;
using namespace bandit;

// Tell bandit there are tests here.
go_bandit([]() {
  // We're describing how a fuzzbox works.
  describe("fuzzbox", []() {
    guitar_ptr guitar;
    fuzzbox_ptr fuzzbox;

    // Make sure each test has a fresh setup with
    // a guitar with a fuzzbox connected to it.
    before_each([&]() {
      guitar = guitar_ptr(new struct guitar());
      fuzzbox = fuzzbox_ptr(new struct fuzzbox());
      guitar->add_effect(fuzzbox.get());
    });

    it("starts in clean mode", [&]() {
      AssertThat(guitar->sound(), Equals(sounds::clean));
    });

    // Describe what happens when we turn on the fuzzbox.
    describe("in distorted mode", [&]() {
      // Turn on the fuzzbox.
      before_each([&]() {
        fuzzbox->flip();
      });

      it("sounds distorted", [&]() {
        AssertThat(guitar->sound(), Equals(sounds::distorted));
      });
    });
  });
});

int main(int argc, char* argv[]) {
  // Run the tests.
  return bandit::run(argc, argv);
}
```

## Download

Clone the Git repository:

```sh
git clone --recursive https://github.com/banditcpp/bandit.git
```

or if you have already cloned the repository without `--recursive`, do

```sh
cd bandit
git submodule update --init --recursive
```

The Git repository contains several directories besides bandit itself,
that is, the specifications (or tests) for bandit (in `specs`),
the bandit website and documentation (in `docs`),
and auxiliary tools for CMake (in `cmake`) and continuous integration (in `util`).

## Installation

Bandit is header-only, so there is no need for additional compilation before you
can start using it. (CMake is only necessary to compile the specs for bandit itself.)

If you want to use bandit inside your projects,
add the parent directory of the `bandit` directory to your project's
include directories and you are ready to go.

Note that you can remove all directories except the `bandit` directory.
Also keep license and copyright information intact.
You can also simply use the `headers-only` branch as a submodule:

```sh
git submodule add -b headers-only https://github.com/banditcpp/bandit bandit
git submodule update --init --recursive
```

## Compilers

Bandit has been tested with the following compilers:

 * GCC ≥ 4.5
 * Clang ≥ 3.2
 * MSVC ≥ 2012

If you want to see if bandit works for your compiler, bandit is shipped with a
cmake project for generating bandit's self tests. Let us know how it goes.

If your compiler does not support the C++11 features required by Bandit, we
suggest that you take a look at [Igloo](//iglootesting.wordpress.com/), which is
built on the same philosophy but works without C++11.

## Online resources

 * [The bandit website](//banditcpp.github.io/bandit)
 * [Bandit on GitHub](//github.com/banditcpp/bandit)
 * [Issues with bandit](//github.com/banditcpp/bandit/issues?state=open)
