#include <specs/specs.h>

namespace fuzzbox {
  enum class sounds {
    clean,
    distorted
  };

  struct fuzzbox {
    fuzzbox() : sound_(sounds::clean) {}

    void flip() {
      sound_ = sounds::distorted;
    }

    sounds sound() {
      return sound_;
    }

  private:
    sounds sound_;
  };

  typedef std::unique_ptr<fuzzbox> fuzzbox_ptr;

  struct guitar {
    void add_effect(fuzzbox* effect) {
      effect_ = effect;
    }

    sounds sound() {
      return effect_->sound();
    }

  private:
    fuzzbox* effect_;
  };

  typedef std::unique_ptr<guitar> guitar_ptr;
}

using namespace fuzzbox;

// Tell bandit there are tests here.
go_bandit([]() {
  // We're describing how a fuzzbox works.
  describe("fuzzbox:", []() {
    guitar_ptr guitar;
    fuzzbox_ptr fuzzbox;

    // Make sure each test has a fresh setup with
    // a guitar with a fuzzbox connected to it.
    before_each([&]() {
      guitar = guitar_ptr(new struct guitar());
      fuzzbox = fuzzbox_ptr(new struct fuzzbox());
      guitar->add_effect(fuzzbox.get());
    });

    it("starts in clean mode", [&]() {
      AssertThat(guitar->sound(), Equals(sounds::clean));
    });

    // Describe what happens when we turn on the fuzzbox.
    describe("in distorted mode", [&]() {
      // Turn on the fuzzbox.
      before_each([&]() {
        fuzzbox->flip();
      });

      it("sounds distorted", [&]() {
        AssertThat(guitar->sound(), Equals(sounds::distorted));
      });
    });
  });
});
