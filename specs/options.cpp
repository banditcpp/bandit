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

  options(std::list<std::string>&& args, const bd::choice_options& choices)
      : argv_helper(std::move(args)), bd::options(argc(), argv(), choices) {}
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

    describe("update_settings()", [&] {
      std::unique_ptr<bd::settings_t> settings;

      before_each([&] {
        settings.reset(new bd::settings_t);
      });

      it("throws exception if no choice_options given", [&] {
        options opt({});
        AssertThrows(std::runtime_error, opt.update_settings(*settings));
      });

      describe("with default choice_options", [&] {
        std::unique_ptr<bd::choice_options> copts;

        before_each([&] {
          copts.reset(new bd::choice_options);
          use_defaults(*copts);
        });

        it("works without any options default choice_options given", [&] {
          options opt({}, *copts);
          AssertThat(opt.update_settings(*settings), IsTrue());
          all_ok(opt);
        });

        using slpair = std::pair<std::string, std::vector<std::string>>;
        for (auto pair : {
                 slpair{"colorizer", {"off", "dark", "light"}},
                 slpair{"formatter", {"posix", "vs"}},
                 slpair{"reporter", {"singleline", "xunit", "info", "spec", "crash", "dots"}}}) {
          for (std::string name : pair.second) {
            it("works with known " + pair.first + " '" + name + "'", [&] {
              error_collector cerr;
              options opt({"--" + pair.first, name}, *copts);
              AssertThat(opt.update_settings(*settings), IsTrue());
              all_ok(opt);
              AssertThat(cerr.get(), IsEmpty());
            });
          }

          it("fails with unknown " + pair.first, [&] {
            error_collector cerr;
            options opt({"--" + pair.first + "=__unknown__"}, *copts);
            all_ok(opt);
            AssertThat(cerr.get(), IsEmpty());
            AssertThat(opt.update_settings(*settings), IsFalse());
            AssertThat(opt.parsed_ok(), IsFalse());
            AssertThat(cerr.get(), Contains("Unknown"));
            AssertThat(cerr.get(), Contains(pair.first));
          });
        }
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
        options opt({"--unknown-option", "--formatter=something", "--reporter", "something",
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
