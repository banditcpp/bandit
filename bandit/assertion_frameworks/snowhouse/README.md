snowhouse
=========

An assertion library for C++

Snowhouse is a stand alone assertion framework for C++. It was originally developed as part of [Igloo](http://github.com/joakimkarlsson/igloo) and has been extracted to be usable in other contexts.

## Usage

```C++
#include <snowhouse/snowhouse.h>
using namespace snowhouse;

int main()
{
  std::cout << "Testing that 23 is 23" << std::endl;
  Assert::That(23, Is().EqualTo(23));

  try
  {
    AssertThat(12, Is().LessThan(11).And().GreaterThan(99));
  }
  catch(const AssertionException& ex)
  {
    std::cout << "Apparently this failed:" << std::endl;
    std::cout << ex.GetMessage() << std::endl;
  }

  return 0;
}
```
