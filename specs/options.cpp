#include <specs/util/argv_helper.h>
#include <specs/specs.h>

using namespace specs::util;
namespace bd = bandit::detail;

struct error_collector {
  error_collector() : origbuf(std::cerr.rdbuf(str.rdbuf())) {}

  ~error_collector() {
    std::cerr.rdbuf(origbuf);
  }

  std::string get() {
    return str.str();
  }

private:
  std::stringstream str;
  std::streambuf* origbuf;
};

static void all_ok(const bd::options& opt) {
  AssertThat(opt.parsed_ok(), IsTrue());
  AssertThat(opt.has_further_arguments(), IsFalse());
  AssertThat(opt.has_unknown_options(), IsFalse());
}

struct options : private argv_helper, public bd::options {
  options(std::list<std::string>&& args)
      : argv_helper(std::move(args)), bd::options(argc(), argv()) {}
};

go_bandit([]() {
  describe("options", [&]() {
    describe("with valid options", [&] {
      it("parses the '--help' option", [&]() {
        options opt({"--help"});
        AssertThat(opt.help(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--version' option", [&]() {
        options opt({"--version"});
        AssertThat(opt.version(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--skip=\"substring\"' option once", [&]() {
        options opt({"--skip=substring"});
        AssertThat(opt.filter_chain(), HasLength(1));
        AssertThat(opt.filter_chain().front().pattern, Equals("substring"));
        AssertThat(opt.filter_chain().front().skip, IsTrue());
        all_ok(opt);
      });

      it("parses the '--only=\"substring\"' option", [&]() {
        options opt({"--only=substring"});
        AssertThat(opt.filter_chain(), HasLength(1));
        AssertThat(opt.filter_chain().front().pattern, Equals("substring"));
        AssertThat(opt.filter_chain().front().skip, IsFalse());
        all_ok(opt);
      });

      it("parses multiple --skip and --only options correctly", [&]() {
        options opt({"--skip=s1", "--only=o2", "--skip=s3", "--only=o4"});
        AssertThat(opt.filter_chain(), HasLength(4));
        AssertThat(opt.filter_chain()[0].pattern, Equals("s1"));
        AssertThat(opt.filter_chain()[0].skip, IsTrue());
        AssertThat(opt.filter_chain()[1].pattern, Equals("o2"));
        AssertThat(opt.filter_chain()[1].skip, IsFalse());
        AssertThat(opt.filter_chain()[2].pattern, Equals("s3"));
        AssertThat(opt.filter_chain()[2].skip, IsTrue());
        AssertThat(opt.filter_chain()[3].pattern, Equals("o4"));
        AssertThat(opt.filter_chain()[3].skip, IsFalse());
        all_ok(opt);
      });

      it("has an empty filter chain if neither --skip nor --only are present", [&]() {
        options opt({});
        AssertThat(opt.filter_chain(), IsEmpty());
        all_ok(opt);
      });

      it("parses the '--break-on-failure' option", [&]() {
        options opt({"--break-on-failure"});
        AssertThat(opt.break_on_failure(), IsTrue());
        all_ok(opt);
      });

      it("parses the '--dry-run' option", [&]() {
        options opt({"--dry-run"});
        AssertThat(opt.dry_run(), IsTrue());
        all_ok(opt);
      });
    });

    describe("with no arguments", [&]() {
      options opt({});

      it("is valid", [&] {
        all_ok(opt);
      });

      it("cannot find '--help'", [&]() {
        AssertThat(opt.help(), IsFalse());
      });

      it("cannot find '--version'", [&]() {
        AssertThat(opt.version(), IsFalse());
      });

      it("cannot find '--break-on-failure'", [&]() {
        AssertThat(opt.break_on_failure(), IsFalse());
      });

      it("cannot find '--dry-run'", [&]() {
        AssertThat(opt.dry_run(), IsFalse());
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
        options opt({"--unknown-option", "--formatter=vs", "--reporter", "xunit",
            "unknown-argument", "--dry-run"});
        AssertThat(opt.parsed_ok(), IsTrue());
        AssertThat(opt.dry_run(), IsFalse());
        AssertThat(opt.has_further_arguments(), IsTrue());
        AssertThat(opt.has_unknown_options(), IsTrue());
      });
    });

    describe("with missing option arguments", [&] {
      for (std::string name : {"skip", "only", "formatter", "reporter"}) {
        it("is not ok with missing --" + name + " argument", [&] {
          error_collector cerr;
          options opt({"--" + name});
          AssertThat(opt.parsed_ok(), IsFalse());
          AssertThat(cerr.get(), !IsEmpty());
        });
      }
    });
  });
});
