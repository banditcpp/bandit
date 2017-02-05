#include <specs/specs.h>

using namespace bandit::specs::util;
namespace bd = bandit::detail;

go_bandit([](){

    describe("options:", [&](){

      it("parses the '--help' option", [&](){
        const char* args[] = {"executable", "--help"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        opt.help() must be_truthy;
      });

      it("parses the '--version' option", [&](){
        const char* args[] = {"executable", "--version"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        opt.version() must be_truthy;
      });

      it("parses the '--no-color' option", [&](){
        const char* args[] = {"executable", "--no-color"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        opt.no_color() must be_truthy;
      });

      it("parser the '--formatter=vs' option", [&](){
        const char* args[] = {"executable", "--formatter=vs"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        opt.formatter() must equal(bd::options::formatters::FORMATTER_VS);
      });

      it("parser the '--formatter=default' option", [&](){
        const char* args[] = {"executable", "--formatter=default"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        opt.formatter() must equal(bd::options::formatters::FORMATTER_DEFAULT);
      });

      it("parses the '--skip=\"substring\"' option", [&](){
        const char* args[] = {"executable", "--skip=substring"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        opt.skip() must equal("substring");
      });

      it("parses skip as empty string if not present", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);

        bd::options opt(argv.argc(), argv.argv());
        //opt.skip() must equal("");
        opt.skip() must equal("");
      });

      it("parses the '--only=\"substring\"' option", [&](){
        const char* args[] = {"executable", "--only=substring"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());
        opt.only() must equal("substring");
      });

      it("parses only as empty string if not present", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);

        bd::options opt(argv.argc(), argv.argv());
        opt.only() must equal("");
      });

      it("parses the '--break-on-failure' option", [&](){
        const char* args[] = {"executable", "--break-on-failure"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        opt.break_on_failure() must be_truthy;
      });

      it("parses the '--dry-run' option", [&](){
        const char* args[] = {"executable", "--dry-run"};
        argv_helper argv(2, args);

        bd::options opt(argv.argc(), argv.argv());

        opt.dry_run() must be_truthy;
      });

      describe("with no arguments", [&](){
        const char* args[] = {"executable"};
        argv_helper argv(1, args);
        bd::options opt(argv.argc(), argv.argv());

        it("cannot find '--help'", [&](){
          opt.help() must be_falsy;
        });

        it("cannot find '--version'", [&](){
          opt.version() must be_falsy;
        });

        it("cannot find '--no-color'", [&](){
          opt.no_color() must be_falsy;
        });

        it("cannot find '--break-on-failure'", [&](){
          opt.break_on_failure() must be_falsy;
        });

        it("cannot find '--dry-run'", [&](){
          opt.dry_run() must be_falsy;
        });

        it("uses default formatter for '--formatter'", [&](){
          opt.formatter() must equal(bd::options::formatters::FORMATTER_DEFAULT);
        });
      });
    });

});
