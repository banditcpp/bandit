#include <specs/specs.h>

go_bandit([](){

  describe("default failure formatter", [&](){
    default_failure_formatter formatter;
  
    it("formats assertions with file and line number", [&](){
      assertion_exception exception("message", "file", 321);
      AssertThat(formatter.format(exception), Equals("file:321: message"));
    });

    it("formats assertions without file and line number", [&](){
      assertion_exception exception("message");
      AssertThat(formatter.format(exception), Equals("message"));
    });
  
  });

});
