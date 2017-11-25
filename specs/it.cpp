#include <specs/fakes/fake_context.h>
#include <specs/fakes/fake_reporter.h>
#include <specs/specs.h>

using namespace bandit::fakes;
namespace bd = bandit::detail;

go_bandit([]() {
  describe("it", [&]() {
    std::function<void()> it_func;
    fake_reporter* reporter;
    std::unique_ptr<bandit::detail::controller_t> controller;
    std::unique_ptr<fake_context> context;

    before_each([&]() {
      reporter = new fake_reporter();
      controller.reset(new bandit::detail::controller_t());
      controller->set_reporter(reporter);
      context.reset(new fake_context());
      controller->get_contexts().push_back(context.get());

      controller->set_policy(new run_policy::always());
    });

    auto call_it = [&]() {
      it("my it", it_func, false, *controller);
    };

    it("tells the current context that execution has started", [&]() {
      // This is important as once execution has started,
      // before_each and after_each calls cannot be guaranteed to
      // be run before any 'it' method.

      call_it();
      AssertThat(context->call_log(), Has().AtLeast(1).EqualTo("execution_is_starting"));
    });

    it("does not work without context", [&] {
      controller->get_contexts().pop_back();
      AssertThrows(bandit::detail::test_run_error, call_it());
      AssertThat(LastException<bandit::detail::test_run_error>().what(),
          Equals("'it' was called without surrounding 'describe'"));
    });

    describe("with succeeding test", [&]() {
      before_each([&]() {
        it_func = []() {};
      });

      it("tells reporter it's starting", [&]() {
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_starting: my it"));
      });

      it("tells reporter it's succeeded", [&]() {
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_succeeded: my it"));
      });

      it("calls before_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
      });

      describe("but with a failing after_each", [&]() {
        before_each([&]() {
          context->with_after_each([]() { AssertThat(2, Equals(3)); });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(),
              Has().Exactly(1).EqualTo("it_failed: my it (Expected: equal to 3 Actual: 2 )"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });

      describe("but with a std::exception in after_each", [&]() {
        before_each([&]() {
          context->with_after_each([]() { throw std::logic_error("logic is wrong!"); });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_failed: my it (exception: logic is wrong!)"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });

      describe("but with an unknown error in after_each", [&]() {
        before_each([&]() {
          context->with_after_each([]() { throw 25; });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_unknown_error: my it"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });

      describe("but with a failing before_each", [&]() {
        before_each([&]() {
          context->with_before_each([]() { AssertThat(2, Equals(3)); });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(),
              Has().Exactly(1).EqualTo("it_failed: my it (Expected: equal to 3 Actual: 2 )"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });

      describe("but with a std::exception in before_each", [&]() {
        before_each([&]() {
          context->with_before_each([]() { throw std::logic_error("logic is wrong!"); });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_failed: my it (exception: logic is wrong!)"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });

      describe("but with an unknown error in before_each", [&]() {
        before_each([&]() {
          context->with_before_each([]() { throw 25; });
        });

        it("tells reporter it's failed", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_unknown_error: my it"));
        });

        it("doesn't report a succeeding test", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().None().EqualTo("it_succeeded: my it"));
        });

        it("tells run_policy that we have a failing test", [&]() {
          call_it();
          AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
        });
      });
    });

    describe("with failing test", [&]() {
      before_each([&]() {
        it_func = []() { AssertThat(3, Equals(2)); };
      });

      it("tells reporter it's failed", [&]() {
        call_it();
        AssertThat(reporter->call_log(),
            Has().Exactly(1).EqualTo("it_failed: my it (Expected: equal to 2 Actual: 3 )"));
      });

      it("calls before_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
      });

      it("tells run_policy that we have a failing test", [&]() {
        call_it();
        AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
      });
    });

    describe("with crashing test", [&]() {
      before_each([&]() {
        it_func = []() { throw 44; };
      });

      it("tells reporter it's failed", [&]() {
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_unknown_error: my it"));
      });

      it("calls before_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
      });

      it("tells run_policy that we have a failing test", [&]() {
        call_it();
        AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
      });
    });

    describe("with test throwing exception based on 'std::exception'", [&]() {
      before_each([&]() {
        it_func = []() { throw std::logic_error("logic error"); };
      });

      it("tells reporter it's failed", [&]() {
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_failed: my it (exception: logic error)"));
      });

      it("calls before_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_before_eaches"));
      });

      it("calls after_each in context", [&]() {
        call_it();
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("run_after_eaches"));
      });

      it("tells run_policy that we have a failing test", [&]() {
        call_it();
        AssertThat(controller->get_policy().has_encountered_failure(), IsTrue());
      });
    });

    describe("it_skip", [&]() {
      it("tells reporter it's skipped", [&]() {
        it_skip("my it", []() {}, *controller);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_skip: my it"));
      });

      it("doesn't call function", [&]() {
        bool called = false;
        it_skip("my it", [&]() { called = true; }, *controller);
        AssertThat(called, IsFalse());
      });
    });

    describe("xit", [&]() {
      it("tells reporter it's skipped", [&]() {
        xit("my it", []() {}, *controller);
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_skip: my it"));
      });

      it("doesn't call function", [&]() {
        bool called = false;
        xit("my it", [&]() { called = true; }, *controller);
        AssertThat(called, IsFalse());
      });
    });

    describe("with a run policy that says to skip this 'it'", [&]() {
      bool it_was_called;

      before_each([&]() {
        controller->set_policy(new run_policy::never());
        it_func = [&]() { it_was_called = true; };
        it_was_called = false;
      });

      it("tells reporter it's skipped", [&]() {
        call_it();
        AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_skip: my it"));
      });

      it("doesn't call function", [&]() {
        call_it();
        AssertThat(it_was_called, IsFalse());
      });
    });

    describe("skipping", [&]() {
      bool it_was_called;

      before_each([&]() {
        it_func = [&]() { it_was_called = true; };
        it_was_called = false;
      });

      describe("with a policy that says to skip this it", [&]() {
        before_each([&]() {
          controller->set_policy(new run_policy::never());
        });

        it("tells reporter it's skipped", [&]() {
          call_it();
          AssertThat(reporter->call_log(), Has().Exactly(1).EqualTo("it_skip: my it"));
        });

        it("doesn't call function", [&]() {
          call_it();
          AssertThat(it_was_called, IsFalse());
        });
      });
    });
  });
});
