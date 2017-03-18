#include <specs/util/argv_helper.h>
#include <specs/specs.h>

using namespace bandit::specs::util;
namespace bd = bandit::detail;

static void all_ok(const bd::options &opt) {
  AssertThat(opt.parsed_ok(), IsTrue());
  AssertThat(opt.has_further_arguments(), IsFalse());
  AssertThat(opt.has_unknown_options(), IsFalse());
}

struct options : private argv_helper, public bd::options {
  options(std::list<std::string>&& args)
    : argv_helper(std::move(args))
    , bd::options(argc(), argv())
  {}
};

go_bandit([](){
  describe("options:", [&](){
    describe("with valid options", [&] {
      it("parses the '--help' option", [&](){
        options opt({"--help"});
        AssertThat(opt.help(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--version' option", [&](){
        options opt({"--version"});
        AssertThat(opt.version(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--no-color' option", [&](){
        options opt({"--no-color"});
        AssertThat(opt.no_color(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--formatter=vs' option", [&](){
        options opt({"--formatter=vs"});
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_VS));
        all_ok(opt);
      });

      it("parses the '--formatter=vs' option when used without =", [&](){
        options opt({"--formatter", "vs"});
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_VS));
        all_ok(opt);
      });

      it("parses the '--formatter=default' option", [&](){
        options opt({"--formatter=default"});
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_DEFAULT));
        all_ok(opt);
      });

      it("parses the '--skip=\"substring\"' option", [&](){
        options opt({"--skip=substring"});
        AssertThat(opt.skip(), Equals("substring"));
        all_ok(opt);
      });

      it("parses skip as empty string if not present", [&](){
        options opt({});
        AssertThat(opt.skip(), Equals(""));
        all_ok(opt);
      });

      it("parses the '--only=\"substring\"' option", [&](){
        options opt({"--only=substring"});
        AssertThat(opt.only(), Equals("substring"));
        all_ok(opt);
      });

      it("parses only as empty string if not present", [&](){
        options opt({});
        AssertThat(opt.only(), Equals(""));
        all_ok(opt);
      });

      it("parses the '--break-on-failure' option", [&](){
        options opt({"--break-on-failure"});
        AssertThat(opt.break_on_failure(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--dry-run' option", [&](){
        options opt({"--dry-run"});
        AssertThat(opt.dry_run(), IsTrue());
        all_ok(opt);
      });
    });

    describe("with no arguments", [&](){
      options opt({});

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

    describe("with unknown arguments", [&] {
      it("recognizes unknown arguments", [&] {
        options opt({"unknown-argument"});
        AssertThat(opt.parsed_ok(), IsTrue());
        AssertThat(opt.has_further_arguments(), IsTrue());
        AssertThat(opt.has_unknown_options(), IsFalse());
      });

      it("recognizes unknown options", [&] {
        options opt({"--unknown-option"});
        AssertThat(opt.parsed_ok(), IsTrue());
        AssertThat(opt.has_further_arguments(), IsFalse());
        AssertThat(opt.has_unknown_options(), IsTrue());
      });

      it("recognizes unknown options and arguments", [&] {
        options opt({"--unknown-option", "unknown-argument"});
        AssertThat(opt.parsed_ok(), IsTrue());
        AssertThat(opt.has_further_arguments(), IsTrue());
        AssertThat(opt.has_unknown_options(), IsTrue());
      });

      it("ignores unknown options and arguments", [&] {
        options opt({"--unknown-option",
                     "--formatter=vs",
                     "--reporter", "xunit",
                     "--no-color",
                     "unknown-argument",
                     "--dry-run"});
        AssertThat(opt.parsed_ok(), IsTrue());
        AssertThat(opt.formatter(), Equals(bd::options::formatters::FORMATTER_VS));
        AssertThat(opt.reporter(), Equals("xunit"));
        AssertThat(opt.no_color(), IsTrue());
        AssertThat(opt.dry_run(), IsFalse())
        AssertThat(opt.has_further_arguments(), IsTrue());
        AssertThat(opt.has_unknown_options(), IsTrue());
      });
    });

    describe("with missing option arguments", [&] {
      for (std::string name : {"skip", "only", "formatter", "reporter"}) {
        it((std::string("is not ok with missing --") + name + " argument").c_str(), [&] {
          options opt({"--" + name});
          AssertThat(opt.parsed_ok(), IsFalse());
        });
      }
    });

    describe("with unknown option arguments", [&] {
      for (std::string name : {"formatter", "reporter"}) {
        it((std::string("is not ok with unknown ") + name).c_str(), [&] {
          options opt({"--" + name + "=__unknown__"});
          AssertThat(opt.parsed_ok(), IsFalse());
        });
      }
    });
  });
});
