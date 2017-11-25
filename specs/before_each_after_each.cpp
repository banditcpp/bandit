#include <specs/fakes/fake_context.h>
#include <specs/specs.h>

go_bandit([]() {
  describe("before_each/after_each", [&]() {
    std::unique_ptr<bandit::detail::controller_t> controller;
    std::unique_ptr<fakes::fake_context> context;

    before_each([&]() {
      controller.reset(new bandit::detail::controller_t());
      context.reset(new fakes::fake_context());
      controller->get_contexts().push_back(context.get());
    });

    describe("before_each", [&]() {
      std::function<void()> before_each_fn;

      before_each([&]() {
        before_each_fn = []() {};
      });

      it("registers itself for the current context in the stack", [&]() {
        before_each(before_each_fn, *controller);
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("register_before_each"));
      });

      it("does not work without context", [&] {
        controller->get_contexts().pop_back();
        AssertThrows(bandit::detail::test_run_error, before_each(before_each_fn, *controller));
        AssertThat(LastException<bandit::detail::test_run_error>().what(),
            Equals("'before_each' was called without surrounding 'describe'"));
      });
    });

    describe("after_each", [&]() {
      std::function<void()> after_each_fn;

      before_each([&]() {
        after_each_fn = []() {};
      });

      it("registers itself for the current context in the stack", [&]() {
        after_each(after_each_fn, *controller);
        AssertThat(context->call_log(), Has().Exactly(1).EqualTo("register_after_each"));
      });

      it("does not work without context", [&] {
        controller->get_contexts().pop_back();
        AssertThrows(bandit::detail::test_run_error, after_each(after_each_fn, *controller));
        AssertThat(LastException<bandit::detail::test_run_error>().what(),
            Equals("'after_each' was called without surrounding 'describe'"));
      });
    });
  });

  describe("before_each/after_each integration", [&]() {
    describe("multiple before_each/after_each on the same level", [&] {
      specs::logging_fake logger;

      before_each([&]() {
        logger.log() << "first before_each called" << std::endl;
      });

      before_each([&]() {
        logger.log() << "second before_each called" << std::endl;
      });

      after_each([&]() {
        logger.log() << "first after_each called" << std::endl;
      });

      after_each([&]() {
        logger.log() << "second after_each called" << std::endl;
      });

      it("should only have called the before_each functions for the first test", [&]() {
        AssertThat(logger.call_log(), Has().Exactly(1).EqualTo("first before_each called"));
        AssertThat(logger.call_log(), Has().Exactly(1).EqualTo("second before_each called"));
        AssertThat(logger.call_log(), Has().None().Containing("after_each"));
      });

      it("should have called 'before_each' function twice, and 'after_each' functions once for the second test", [&]() {
        AssertThat(logger.call_log(), Has().Exactly(2).EqualTo("first before_each called"));
        AssertThat(logger.call_log(), Has().Exactly(2).EqualTo("second before_each called"));
        AssertThat(logger.call_log(), Has().Exactly(1).EqualTo("first after_each called"));
        AssertThat(logger.call_log(), Has().Exactly(1).EqualTo("second after_each called"));
      });
    });

    describe("multiple before_each/after_each in different levels", [&] {
      specs::logging_fake logger;

      before_each([&] {
        logger.log() << "1b";
      });

      after_each([&] {
        logger.log() << "1a";
      });

      describe("first context level", [&] {
        before_each([&] {
          logger.log() << "2b";
        });

        after_each([&] {
          logger.log() << "2a";
        });

        describe("second context level", [&] {
          before_each([&] {
            logger.log() << "3b";
          });

          after_each([&] {
            logger.log() << "3a";
          });

          it("should only have called the before_each functions for the first test", [&]() {
            AssertThat(logger.call_log(), Equals("1b2b3b"));
            logger.log() << " ";
          });

          it("should have called 'before_each' function twice, and 'after_each' functions once for the second test", [&]() {
            AssertThat(logger.call_log(), Equals("1b2b3b 3a2a1a1b2b3b"));
          });
        });
      });
    });
  });
});
