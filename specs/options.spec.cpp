#include <specs/specs.h>

using namespace bandit::specs::util;

go_bandit([](){

    describe("options:", [&](){
    
      it("parses the '--help' option", [&](){
        const char* args[] = {"executable", "--help"};
        argv_helper argv(2, args);

        options opt(argv.argc(), argv.argv());

        AssertThat(opt.help(), IsTrue());
      });

      it("parses the '--version' option", [&](){
        const char* args[] = {"executable", "--version"};
        argv_helper argv(2, args);

        options opt(argv.argc(), argv.argv());

        AssertThat(opt.version(), IsTrue());
      });

      it("parses the '--no-color' option", [&](){
        const char* args[] {"executable", "--no-color"};
        argv_helper argv(2, args);

        options opt(argv.argc(), argv.argv());

        AssertThat(opt.no_color(), IsTrue());
      });
    
      describe("with no arguments", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);
        options opt(argv.argc(), argv.argv());

      
        it("cannot find '--help'", [&](){
          AssertThat(opt.help(), IsFalse());
        });

        it("cannot find '--version'", [&](){
          AssertThat(opt.version(), IsFalse());
        });
      
      });
    });

});
