---
layout: index
---
Bandit is a framework for C++11 that wants to make working with unit tests a 
pleasant experience.

Bandit is released under the 
[MIT license](https://github.com/joakimkarlsson/bandit/blob/master/LICENSE.md)

#An example

This is a complete test application written in bandit:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){
    describe("fuzzbox:", [](){
      guitar_ptr guitar;
      fuzzbox_ptr fuzzbox;

      before_each([&](){
        guitar = guitar_ptr(new struct guitar());
        fuzzbox = fuzzbox_ptr(new struct fuzzbox());
        guitar->add_effect(*fuzzbox);
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

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
{% endhighlight %}

#Installing

Bandit is header only so there is no need for additional compilation before you
can start using it. Download bandit and add its root directory to your project's
include directories and you're ready to go.

#Compilers

Bandit has been tested with GCC 4.7.3, Clang 3.2.0, and Visual Studio 2012. It 
doesn't work with Visual Studio 2010. 

We'll add more compilers to the list as we test them. If you want to see if 
bandit works for your compiler, bandit is shipped with a cmake project for 
generating bandit's self tests. 

If your compiler doesn't support the C++11 features required by Bandit, we 
suggest that you take a look at [Igloo](http://igloo-testing.org), which is 
built on the same philosophy but works without C++11.


#Writing Tests

Once you have a command line application with an entry point that delegates to 
bandit, it's time to start writing some tests.

We begin by adding a new .cpp file to our project. We then tell bandit that this
file contains tests by using the ```go_bandit()``` construct:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

  // Our tests goes here...

});
{% endhighlight %}

##Describing things

Bandit is all about describing how pieces of your code should work. These 
descriptions can be be executed to verify that they're accurate. Each 
description specifies how a certain piece of your code behaves under certain 
conditions. 

An example:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

  describe("a turn based game", [](){
    game_ptr game;
    player_ptr player_one, player_two;

    // Setting up the game. More on this later...

    it("is player one's turn", [&}(){
      AssertThat(game->whos_turn(), Equals(player_one));
    });
  });

});
{% endhighlight %}

```describe()``` in this case describes a game with two players. ```it()``` verifies that when a new
game has just been created, it should be player one's turn to make a move.

##Assertions

Bandit comes bundled with the [snowhouse](https://github.com/joakimkarlsson/snowhouse) assertion library.

##Setting up context

We need to be more specific about what it means to set up a game. This is what ```before_each()```
and ```after_each()``` is for.

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

  describe("a turn based game", [](){
    game_ptr game;
    player_ptr player_one, player_two;

    before_each([&](){
      game = game_ptr(new game());
      player_one = player_ptr(new player("Player 1"));
      player_two = player_ptr(new player("Player 2"));
      game->add_player(*player_one);
      game->add_player(*player_two);
    });

    it("is player one's turn", [&}(){
      AssertThat(game->whos_turn(), Equals(*player_one));
    });
  });

});
{% endhighlight %}

```before_each()``` will be called before each ```it()``` method is called. This ensures that each 
methods gets a fresh new context that hasn't been polluted by a previous test. If you need to restore
things after a test, you can user ```after_each()```, which will be called after each ```it()```
method.

**NOTE:** *You need to declare your ```before_each()``` and ```after_each()``` methods before any 
```it()``` or nested ```describe```. If you don't, bandit cannot guarantee that they will be called 
correctly and will complain with an error message.*

#Nested descriptions

This is a power feature: you can nest descriptions. This will help you organize the different states
that the thing you're describing can be in. The outer context says "here is this thing" and nested
contexts can then say "in this state". This makes for descriptions that really conveys how a 
component works under different conditions.

An example might be helpful:

{% highlight cpp %}
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

  describe("a turn based game", [](){
    game_ptr game;
    player_ptr player_one, player_two;

    before_each([&](){
      game = game_ptr(new game());
      player_one = player_ptr(new player("Player 1"));
      player_two = player_ptr(new player("Player 2"));
      game->add_player(*player_one);
      game->add_player(*player_two);
    });

    it("is player one's turn", [&}(){
      AssertThat(game->whos_turn(), Equals(*player_one));
    });

    describe("with all game positions filled", [&](){

      before_each([&](){
        fill_all_positions(game);
      });

      it("considers the game a tie", [&](){
        AssertThat(game->winner(), Equals(TIE));
      });
    });

    describe("player two has made a bad move", [&](){
    
       before_each([&](){
         make_a_bad_move(*player_two);
       });
       
       it("considers player one the winner", [&](){
         AssertThat(game->winner(), Equals(PLAYER_ONE));
       });
    });
    
  });

});
{% endhighlight %}

When bandit is about to call ```it("considers the game a tie")```, it first calls all registered ```before_each()``` in the outermost ```describe()``` before it calls ```before_each()``` in the current ```describe()```. With several levels of nesting, bandit will start at the outermost ```describe()``` and work its way down the ancestor chain until it reaches the ```describe()``` where the current ```it()``` is found.

```after_each()``` will be called after the call to ```it()``` starting at the describe function of the ```it()``` and working its way up the ancestor chain.

That way nested describes can augment the state set up by its parents, making the set up for each ```describe()``` pretty self explanatory even for components
that have a complex set of states.
