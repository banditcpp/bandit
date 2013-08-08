#include <specs/specs.h>

go_bandit([](){

  describe("name contains skip policy:", [&](){
  
    describe("with an empty pattern", [&](){
      name_contains_skip_policy policy("");
    
      it("includes everything if pattern is empty", [&](){
        AssertThat(policy.should_skip("whatever"), IsFalse());
      });
    
    });

    describe("with a pattern", [&](){
    
      name_contains_skip_policy policy("substring");

      it("skips names containing substring", [&](){
        AssertThat(policy.should_skip("this contains substring"), IsTrue());
      });

      it("includes names not containing substring", [&](){
        AssertThat(policy.should_skip("this doesn't contain it"), IsFalse());
      });
    
    });
  
  });

});
