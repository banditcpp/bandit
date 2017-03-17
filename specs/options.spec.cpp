#include <specs/util/argv_helper.h>
#include <specs/specs.h>

using namespace bandit::specs::util;
namespace bd = bandit::detail;

static void all_ok(const bd::options &opt) {
  AssertThat(opt.parsed_ok(), IsTrue());
  AssertThat(opt.has_further_arguments(), IsFalse());
  AssertThat(opt.has_unknown_options(), IsFalse());
}

go_bandit([](){

    describe("options:", [&](){

      it("parses the '--help' option", [&](){
        argv_helper argv({"--help"});

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.help(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--version' option", [&](){
        argv_helper argv({"--version"});

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.version(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--no-color' option", [&](){
        argv_helper argv({"--no-color"});

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.no_color(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--formatter=vs' option", [&](){
        argv_helper argv({"--formatter=vs"});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_VS));
        all_ok(opt);
      });

      it("parses the '--formatter=default' option", [&](){
        argv_helper argv({"--formatter=default"});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_DEFAULT));
        all_ok(opt);
      });

      it("parses the '--skip=\"substring\"' option", [&](){
        argv_helper argv({"--skip=substring"});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.skip(), Equals("substring"));
        all_ok(opt);
      });

      it("parses skip as empty string if not present", [&](){
        argv_helper argv({});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.skip(), Equals(""));
        all_ok(opt);
      });

      it("parses the '--only=\"substring\"' option", [&](){
        argv_helper argv({"--only=substring"});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.only(), Equals("substring"));
        all_ok(opt);
      });

      it("parses only as empty string if not present", [&](){
        argv_helper argv({});

        bd::options opt(argv.argc(), argv.argv());
        AssertThat(opt.only(), Equals(""));
        all_ok(opt);
      });

      it("parses the '--break-on-failure' option", [&](){
        argv_helper argv({"--break-on-failure"});

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.break_on_failure(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--dry-run' option", [&](){
        argv_helper argv({"--dry-run"});

        bd::options opt(argv.argc(), argv.argv());

        AssertThat(opt.dry_run(), IsTrue());
        all_ok(opt);
      });

      describe("with no arguments", [&](){
        argv_helper argv({});
        bd::options opt(argv.argc(), argv.argv());

        it("is valid", [&] {
          all_ok(opt);
        });

        it("cannot find '--help'", [&](){
          AssertThat(opt.help(), IsFalse());
        });

        it("cannot find '--version'", [&](){
          AssertThat(opt.version(), IsFalse());
        });

        it("cannot find '--no-color'", [&](){
          AssertThat(opt.no_color(), IsFalse());
        });

        it("cannot find '--break-on-failure'", [&](){
          AssertThat(opt.break_on_failure(), IsFalse())
        });

        it("cannot find '--dry-run'", [&](){
          AssertThat(opt.dry_run(), IsFalse())
        });

        it("uses default formatter for '--formatter'", [&](){
          AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_DEFAULT));
        });
      });
    });

});
