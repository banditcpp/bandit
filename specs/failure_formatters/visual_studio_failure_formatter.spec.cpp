#include <specs/specs.h>
namespace bd = bandit::detail;

go_bandit([](){

  describe("Visual Studio failure formatter:", [&](){
  
    bd::visual_studio_failure_formatter formatter;

    it("formats assertions with file and line number", [&](){
      bd::assertion_exception exception("message", "file", 321);
      formatter.format(exception) must equal("file(321): message");
    });

    it("formats assertions without file and line number", [&](){
      bd::assertion_exception exception("message");
      formatter.format(exception) must equal("bandit: message");
    });
  
  });

});
