#include <specs/specs.h>

namespace fuzzbox {

  typedef enum {
    clean,
    distorted
  } sounds;

  struct fuzzbox
  {
    fuzzbox() : sound_(sounds::clean)
    {}

    void flip()
    {
      sound_ = sounds::distorted;
    }

    sounds sound()
    {
      return sound_;
    }

    private:
    sounds sound_;
  };
  typedef std::unique_ptr<fuzzbox> fuzzbox_ptr;

  struct guitar
  {
    void add_effect(fuzzbox* effect)
    {
      effect_ = effect;
    }

    sounds sound()
    {
      return effect_->sound();
    }

    private:
    fuzzbox* effect_;
  };
  typedef std::unique_ptr<guitar> guitar_ptr;
  
go_bandit([](){

    describe("fuzzbox:", [](){
      guitar_ptr guitar;
      fuzzbox_ptr fuzzbox;

      before_each([&](){
        guitar = guitar_ptr(new struct guitar());
        fuzzbox = fuzzbox_ptr(new struct fuzzbox());
        guitar->add_effect(fuzzbox.get());
      });

      it("starts in clean mode", [&](){
        AssertThat(guitar->sound(), Equals(sounds::clean));
      });

      describe("in distorted mode", [&](){

        before_each([&](){
          fuzzbox->flip();
        });

        it("sounds distorted", [&](){
          AssertThat(guitar->sound(), Equals(sounds::distorted));
        });
      });
    });

});

}
