#include <specs/specs.h>

using namespace bandit::specs::util;
namespace bd = bandit::detail;

go_bandit([](){

    describe("options:", [&](){

      it("parses the '--help' option", [&](){
        const char* args[] = {"executable", "--help"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.help(), IsTrue());
      });

      it("parses the '--version' option", [&](){
        const char* args[] = {"executable", "--version"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.version(), IsTrue());
      });

      it("parses the '--no-color' option", [&](){
        const char* args[] = {"executable", "--no-color"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.no_color(), IsTrue());
      });

      it("parser the '--formatter=vs' option", [&](){
        const char* args[] = {"executable", "--formatter=vs"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_VS));
      });

      it("parser the '--formatter=default' option", [&](){
        const char* args[] = {"executable", "--formatter=default"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_DEFAULT));
      });

      it("parses the '--skip=\"substring\"' option", [&](){
        const char* args[] = {"executable", "--skip=substring"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.skip(), Equals("substring"));
      });

      it("parses skip as empty string if not present", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.skip(), Equals(""));
      });

      it("parses the '--only=\"substring\"' option", [&](){
        const char* args[] = {"executable", "--only=substring"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.only(), Equals("substring"));
      });

      it("parses only as empty string if not present", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.only(), Equals(""));
      });

      it("parses the '--break-on-failure' oprtion", [&](){
        const char* args[] = {"executable", "--break-on-failure"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.break_on_failure(), IsTrue());
      });

      describe("with no arguments", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);
        bd::options opt(argv.argc(), argv.argv());

        it("cannot find '--help'", [&](){
          AssertThat(opt.help(), IsFalse());
        });

        it("cannot find '--version'", [&](){
          AssertThat(opt.version(), IsFalse());
        });

        it("cannot find '--no-color'", [&](){
          AssertThat(opt.no_color(), IsFalse());
        });

        it("cannot fine '--break-on-failure'", [&](){
          AssertThat(opt.break_on_failure(), IsFalse())
        });

        it("uses default formatter for '--formatter'", [&](){
          AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_DEFAULT));
        });
      });
    });

});
