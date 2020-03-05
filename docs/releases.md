# Releases

For a list of releases including release notes, go to
[bandit's releases page on GitHub](//github.com/banditcpp/bandit/releases).

Bandit will be using [Semantic Versioning 2.0.0](http://semver.org/spec/v2.0.0.html)
beginning from the next major release v3.0.0.
In the following, we list breaking changes per major release.

## Next major release (v3.0.0)

* The `default` formatter is renamed to `posix`.

* The `--no-color` option is replaced by `--colorizer=off`. It now can be
  understood as choosing the `off` color theme instead of turning colors off.

* In prior versions, an error was raised in case no tests were available
  or all tests were skipped. Now the exit code is `0` in these cases because
  no tests are failing.

* The signatures of the grammar functions (`it()`, `describe()`,
  `before_each()`, etc.) have changed.
  For `it()` and `describe()` (and their skipping counterparts),
  the first parameter (the description) is now a `const std::string&`
  instead of `const char*`.
  `it()` and `describe()` also got an optional third parameter that says
  if it should be skipped or not.
  Both changes should not break any sane existing code but the former
  allows to write description-generating code more nicely and
  the latter eases skipping (or not) based on the program flow.

* The grammar and runner now use `bandit::detail::controller_t` as optional parameters
  instead of reporters, run policies, etc. This is a change of the public
  API that is probably only of internal interest.

* The behavior of combined `--only` and `--skip` has changed.
  You can now use these switches multiple times and they always act as a filter,
  that is, you can only reduce the list of tests by adding a `--only` or `--skip`
  switch.
  In prior versions, contexts matching `--skip` were ignored when they contained
  a matching `--only`.

* If you use `after_each()` in nested `describe()`s, the order of processing them
  is now reversed, i.e., the inner `after_each()` is run before the outer one.
  Also, inner `after_each()`s are only called if their corresponding `before_each()`s
  did not fail (or do not exist).

* The option handling became stricter.
  Illegal option arguments like `--reporter=invalid` result in an error now.

* The assertion framework Snowhouse v5.0.0 is used including all its breaking
  changes.

* The `snowhouse` namespace is not used by default.
  To make your old code compile, it may be necessary to add
  ```c++
  using namespace snowhouse;
  ```
