# Running tests

With bandit, you create a command line application that runs all registered
tests, reports the result on stdout, and then returns with exit code `0` if all
tests passed, or exit code greater than `0` if some tests did not pass.

## Command line arguments

Bandit enables you to tweak the way tests are performed and reported by
specifying command line options.
Running `[name of your executable] --help` will list the available options.

```bash
$ bin/bandit-specs --help
USAGE: <executable> [options]

Options:
  --version,               Print version of bandit
  --help,                  Print usage and exit.
  --skip=<substring>,      Skip all 'describe' and 'it' containing substring
  --only=<substring>,      Run only 'describe' and 'it' containing substring
  --break-on-failure,      Stop test run on first failing test
  --dry-run,               Skip all tests. Use to list available tests
  --report-timing,         Instruct reporter to report timing information
  --reporter=<reporter>,   Select reporter: crash, dots, info, singleline, spec,
                           xunit
  --colorizer=<colorizer>, Select color theme: dark, light, off
  --formatter=<formatter>, Select error formatter: posix, qt, vs
```

## Running a subset of the tests

You can skip tests from within your source code by using the `describe_skip()`,
`xdescribe()`, `it_skip()`, and `xit()` constructs, or by setting the last
(optional) argument of `it()` and `describe()` to `true`.

You can skip tests from the command line by using the `--skip=<substring>`
option. By using this, you tell bandit to skip running all `describe()`
and `it()` whose names contain `<substring>`.

It is also possible to tell bandit to only run a subset of the tests by passing
`--only=<substring>`. This will cause bandit to only run those `describe()` and
`it()` whose names contain `<substring>`.

You can use `--only` and `--skip` in combination and multiple times to
filter the running tests according to your needs.

## Tweaking the output

### Reporters

You can specify the way bandit reports the progress and results of a test
by using the `--reporter` option:

#### `--reporter=dots`

Reports each executed test as a dot (`.`) or as an 'F' or 'E'
depending on whether it succeeds or not.

This is the default.

#### `--reporter=singleline`

Reports the progress on a single line.
Updating counters of executed, failed and skipped tests.

Very useful if you do not want to be bothered a lot by output
of the testing framework.

#### `--reporter=xunit`

Reports the test results in a XML format supported by most
continuous integration servers.

#### `--reporter=spec`

Reports the tests in a format similar to how they are
written along with the status of each test.

#### `--reporter=info`

Reports the tests in an appealing form with a marker
for every relevant `it`. The marker says `[ TEST ]` when the test starts,
`[ PASS ]` when the test passed and `[ FAIL ]` when the test failed.
`describe()`s are declared by `begin` and `end` and every `end` contains a
summary.
Skipped `describe()`s are not shown at all.
This reporter is somewhat inspired by the
[Google Test](https://github.com/google/googletest) framework.

#### `--reporter=crash`

Reports one `it()` per line containing the whole `describe()` stack.
Very useful, for example, if you have a huge amount of tests and you are
testing code that may crash, in a continuous integration system that sends
you a mail with the last few lines of output.

### Formatters

With the `--formatter=<formatter>` option you can specify how bandit reports
failed assertions. Different IDEs expect errors to be reported in different
ways to be able to quickly navigate to the location of a failed assertion.

#### `--formatter=posix`

The POSIX formatter reports errors as

```
<filename>:<line>: <error message>
```

This matches how gcc and clang reports errors.
This formatter is the default.

#### `--formatter=qt`

The Qt Creator formatter reports errors as

```
file://<filename>:<line>: <error message>
```

It enables a hyperlink from the console output to jump directly to the error.

#### `--formatter=vs`

The Visual Studio formatter reports errors the way Visual Studio wants
errors to be reported, as

```
<filename>(<line>): <error message>
```

### Colors

Bandit uses colors to highlight the status of the current test run.

#### `--colorizer=off`

Turns colors off.
In certain IDEs or when running tests in continuous integration environment,
colors might be ugly or are not supported and you just see the underlying escape
sequences.

#### `--colorizer=light`

Uses a color theme with light colors for dark backgrounds.
This is the default.

#### `--colorizer=dark`

Uses a color theme with dark colors for light backgrounds.

### Timing

#### `--report-timing`

Instruct reporter to report timing information for each test (only for the
`info` and `xunit` reporters).

## Tweaking the behavior

### Exit on first error

The option `--break-on-failure` exits bandit when the first error or
failed test occurs.

### Dry run

The `--dry-run` option skips every test.
In combination with `--reporter=spec`, this can be used to get a list
of the available tests.
