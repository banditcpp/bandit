bandit
======

Human friendly unit testing framework for C++11

Bandit is a framework for C++11 that wants to make working with unit tests a pleasant
experience.

#An example

This is a complete test application written in bandit:

```cpp
#include <bandit/bandit.h>
using namespace bandit;

go_bandit([](){

    describe("fuzzbox:", [](){
      guitar_ptr guitar;
      fuzzbox_ptr fuzzbox;

      before_each([&](){
        guitar = guitar_ptr(new struct guitar());
        fuzzbox = fuzzbox_ptr(new struct fuzzbox());
      });

      before_each([&](){
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

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
```

#Installing

Bandit is header only so there is no need for additional compilation before you can start using it. Download bandit and add its root directory to your project's include directories and you're ready to go.

#Running Tests

With bandit you create a command line application that runs all registered tests, reports the result
on stdout, and then exits with an error level 0 if all tests passed, and an error level greater
than 0 if some tests did not passed.

Every application needs an entry point, and for bandit it should look like this:

```cpp
#include <bandit/bandit.h>

int main(int argc, char* argv[])
{
  return bandit::run(argc, argv);
}
```

The ```run()``` function will run all registered tests and report the result.

##Command line arguments

Bandit enables you to tweak the way tests results are reported by allowing you to specify command
line switches. Running ```[name of your executable] --help``` will list the available options.

#Writing Tests

Once you have a command line application with an entry point that delegates to bandit, it's time to
start writing some tests.

We begin by adding a new .cpp file to our project. We then tell bandit that this file contains tests
by using the ```go_bandit()``` construct:

```cpp
#include <bandit/bandit.h>

go_bandit([](){

  // Our tests goes here...

});
```

##Describing things

Bandit is all about describing how pieces of your code should work. These descriptions can be 
be executed to verify that they're accurate. Each description specifies how a certain piece of
your code behaves under certain conditions. 

An example:

```cpp
#include <bandit/bandit.h>

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
```

```describe()``` in this case describes a game with two players. ```it()``` verifies that when a new
game has just been created, it should be player one's turn to make a move.

##Assertions

The example above uses (snowhouse)[https://github.com/joakimkarlsson/snowhouse] for verifying that
it actually is player one's turn to move.

Bandit comes bundled with snowhouse out of the box, but it is possible to use any assertion framework
that uses exceptions to notify us about errors. We need to create an adapter that translates from
the framework's exception to bandit's exceptions.

##Setting up context

We need to be more specific about what it means to set up a game. This is what ```before_each()```
and ```after_each()``` is for.

```cpp
#include <bandit/bandit.h>

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
```

```before_each()``` will be called before each ```it()``` method is called. This ensures that each 
methods gets a fresh new context that hasn't been polluted by a previous test. If you need to restore
things after a test, you can user ```after_each()```, which will be called after each ```it()```
method.

#Nested descriptions

This is a power feature: you can nest descriptions. This will help you organize the different states
that the thing you're describing can be in. The outer context says "here is this thing" and nested
contexts can then say "in this state". This makes for descriptions that really conveys how a 
component works under different conditions.

An example might be helpful:

```cpp
#include <bandit/bandit.h>

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
  });

});
```

When bandit is about to call ```it("considers the game a tie")```, first calls all registered ```before_each()```
in the outermost context before it calls all ```before_each()``` in all contexts that are parents to
the current ```describe()```. That way nested describes can augment the state set up by its parents,
making the set up for each ```describe()``` pretty self explanatory.


