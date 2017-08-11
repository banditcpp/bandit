#include <specs/specs.h>

go_bandit([]() {
  namespace bd = bandit::detail;

  describe("bandit run policy", [&]() {
    std::unique_ptr<bandit::context::stack_t> contextstack;
    std::unique_ptr<bandit::context::interface> global_context;
    std::unique_ptr<bd::filter_chain_t> filter_chain(new bd::filter_chain_t);
    bool hard_skip;
    bool break_on_failure;
    bool dry_run;

    auto create_policy = [&]() -> run_policy::bandit {
      return run_policy::bandit(*filter_chain, break_on_failure, dry_run);
    };

    before_each([&]() {
      hard_skip = false;
      break_on_failure = false;
      dry_run = false;

      contextstack = std::unique_ptr<bandit::context::stack_t>(new bandit::context::stack_t());
      global_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("", hard_skip));
      contextstack->push_back(global_context.get());
    });

    describe("neither skip nor only specified", [&]() {
      before_each([&]() {
        filter_chain.reset(new bd::filter_chain_t);
      });

      it("always says run", [&]() {
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name", *contextstack), IsTrue());
      });

      describe("with 'break-on-failure' set", [&]() {
        before_each([&]() {
          break_on_failure = true;
        });

        it("says run if no failure has been encountered", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsTrue());
        });

        it("says don't run if a failure has been encountered", [&]() {
          run_policy::bandit policy = create_policy();
          policy.encountered_failure();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });
      });

      describe("has context marked with 'hard_skip' in stack", [&]() {
        std::unique_ptr<bandit::context::interface> hard_skip_context;

        before_each([&]() {
          hard_skip = true;
          hard_skip_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("always ignore", hard_skip));
          contextstack->push_back(hard_skip_context.get());
        });

        it("never runs", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
          AssertThat(policy.should_run("it name matches 'skip'", *contextstack), IsFalse());
          AssertThat(policy.should_run("it name matches 'only'", *contextstack), IsFalse());
        });
      });
    });

    describe("'skip' specified, 'only' unspecified", [&]() {
      before_each([&]() {
        filter_chain.reset(new bd::filter_chain_t{{"skip", true}});
      });

      describe("current context matches 'skip'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'skip'", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("never runs", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });
      });

      describe("current context doesn't match 'skip'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context doesn't match", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("runs if spec's name doesn't match", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsTrue());
        });

        it("doesn't run if spec's name matches", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name matching 'skip'", *contextstack), IsFalse());
        });
      });
    });

    describe("'only' specified, 'skip' unspecified", [&]() {
      before_each([&]() {
        filter_chain.reset(new bd::filter_chain_t{{"only", false}});
      });

      describe("current context matches 'only'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'only'", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("always runs", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsTrue());
        });
      });

      describe("current context doesn't match 'only'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context doesn't match", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("doesn't run if spec's name doesn't match", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });

        it("runs if spec's name matches", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name matching 'only'", *contextstack), IsTrue());
        });
      });
    });

    describe("'skip' specified, 'only' specified", [&]() {
      before_each([&]() {
        filter_chain.reset(new bd::filter_chain_t{{"skip", true}, {"only", false}});
      });

      describe("current context doesn't match anything", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("doesn't run if spec's name doesn't match 'only'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });

        it("runs if spec's name matches 'only'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it matches 'only'", *contextstack), IsTrue());
        });
      });

      describe("current context matches 'skip'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'skip'", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("doesn't run if spec's name doesn't match 'only'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });

        it("doesn't run if spec's name matches 'only'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it matches 'only'", *contextstack), IsFalse());
        });
      });

      describe("current context matches 'only'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'only'", hard_skip));
          contextstack->push_back(current_context.get());
        });

        it("runs if spec's name doesn't match anything", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsTrue());
        });

        it("doesn't run if spec's name matches 'skip'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name matching 'skip'", *contextstack), IsFalse());
        });
      });

      describe("both 'only' and 'skip' in context stack", [&]() {
        std::unique_ptr<bandit::context::interface> current_context;
        std::unique_ptr<bandit::context::interface> parent_context;

        before_each([&]() {
          current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'only'", hard_skip));
          parent_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'skip'", hard_skip));
          contextstack->push_back(parent_context.get());
          contextstack->push_back(current_context.get());
        });

        it("doesn't run if spec's name doesn't match anything", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name", *contextstack), IsFalse());
        });

        it("doesn't run if spec's name matches 'skip'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name matching 'skip'", *contextstack), IsFalse());
        });

        it("doesn't run if spec's name matches 'only'", [&]() {
          run_policy::bandit policy = create_policy();
          AssertThat(policy.should_run("it name matching 'only'", *contextstack), IsFalse());
        });
      });
    });

    describe("multiple 'skip' and 'only' specified", [&]() {
      before_each([&]() {
        filter_chain.reset(new bd::filter_chain_t{
            {"skip1", true},
            {"only1", false},
            {"skip2", true},
            {"only2", false},
        });
      });

      it("runs if spec's name matches all 'only' but no 'skip'", [&]() {
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name only1 only2", *contextstack), IsTrue());
      });

      it("doesn't run if spec's name matches one 'only' only", [&]() {
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name only1", *contextstack), IsFalse());
        AssertThat(policy.should_run("it name only2", *contextstack), IsFalse());
      });

      it("doesn't run if spec's name does not match anything", [&]() {
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name", *contextstack), IsFalse());
      });

      it("doesn't run if spec's name matches any of the 'skip'", [&]() {
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name skip1", *contextstack), IsFalse());
        AssertThat(policy.should_run("it name skip2", *contextstack), IsFalse());
        AssertThat(policy.should_run("it name skip1 skip2", *contextstack), IsFalse());
      });

      it("doesn't run if context contains any 'skip' but spec's name matches all 'only'", [&]() {
        std::unique_ptr<bandit::context::interface> current_context = std::unique_ptr<bandit::context::interface>(new bandit::context::bandit("context matches 'skip1'", hard_skip));
        contextstack->push_back(current_context.get());
        run_policy::bandit policy = create_policy();
        AssertThat(policy.should_run("it name only1 only2", *contextstack), IsFalse());
      });
    });
  });
});
