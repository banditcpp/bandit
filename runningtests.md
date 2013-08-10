---
layout: index
---
#Running Tests

With bandit you create a command line application that runs all registered 
tests, reports the result on stdout, and then exits with an error level 0 if all
tests passed, and an error level greater than 0 if some tests did not passed.

##Command line arguments

Bandit enables you to tweak the way tests results are reported by allowing you 
to specify command line switches. Running ```[name of your executable] --help```
will list the available options.

{% highlight bash %}
$ my-exe --help
USAGE: <executable> [options]

Options:
  --version,   Print version of bandit
  --help,      Print usage and exit.
  --reporter,  Select reporter (dots, singleline, xunit, spec)
  --no-color,  Suppress colors in output
  --formatter, Select formatting of errors (default, vs)
{% endhighlight %}

##Reporters

You can specify how you want bandit to report the progress and result of a test
run by using the `--reporter` option:

`--reporter=dots` is the default. Each executed test will be printed as a dot or
as an 'F' or 'E' depending on whether it succeeds or not.

`--reporter=singleline` reports the progress on a single line. Updating counters
of executed, failed and skipped tests.

`--reporter=xunit` reports the test results in a xml format supported by most
continuous integration servers.

`--reporter=specs` reports the tests in a format similar to how they are written
along with the status of each test. 

##Formatters

With the `--formatter=<formatter>` option you can specify how bandit reports
failed assertions. Different editors expect errors to be reported in different
ways to be able to quickly navigate to the location of a faile assertion.

The default behavior is to report errors as

```
/file/path:line: <error message>
```
This matches how gcc and clang reports errors.

`--formatter=vs` matches how visual studio wants errors reported.

##Colors

By default, bandit uses colors to highlight the status of the current test run.
In certain IDE's or when running tests in continuous integration environment, this
may look ugly. By specifying `--no-color` you can tell bandit to stop using colors.

##Skipping tests

You can skip tests from within your source code by using the `describe_skip()` and
`it_skip` constructs.

It is also possible to temporarily skip tests by using the `--skip=<substring>`
command line option. By using this, you tell bandit to skip running all `describe`
and `it` whose names contain `<substring>`.

##Running a subset of the tests

It is also possible to tell bandit to only run a subset of the tests by passing
`--only=<substring>`. This will cause bandit to only run those `describe` and `it`
whose names contain `<substring>`.
