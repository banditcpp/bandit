#include <specs/specs.h>

go_bandit([](){

  describe("dots_reporter:", [&](){
    unique_ptr<std::stringstream> stm;
    unique_ptr<bandit::dots_reporter> reporter;
  
    before_each([&](){
      stm = unique_ptr<std::stringstream>(new std::stringstream());
      reporter = unique_ptr<bandit::dots_reporter>(new dots_reporter(*(stm.get())));
    });

    auto output = [&](){ return stm->str(); };

    describe("an empty test run", [&](){
    
      before_each([&](){
        reporter->test_run_starting();
        reporter->test_run_complete();
      });

      it("reports no tests where run", [&](){
        AssertThat(output(), Equals("\nCould not find any tests.\n"));
      });
    
    });
  
  });

});
