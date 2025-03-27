---
layout: spec
mermaid: true
---

EECS 280 Project 3: Euchre
==========================
{: .primer-spec-toc-ignore }

Winter 2025 release.

Checkpoint due 8:00pm EST Monday February 24, 2025.  See [Submission and Grading / Checkpoint](#checkpoint).

Full project due 8:00pm EST Wednesday March 12, 2025.

You may work alone or with a partner ([partnership guidelines](https://eecs280.org/syllabus.html#project-partnerships)). If you work alone, you must work alone on both the checkpoint and the full project. If you work with a partner, you must work with the same partner on the checkpoint and the full project. You may not work alone on the checkpoint and then add a partner for the full project.

<!--
## Change Log

-->

## Introduction
Euchre (pronounced "YOO-kur") is a card game popular in Michigan.

The learning goals of this project include Abstract Data Types in C++, Derived Classes, Inheritance, and Polymorphism.  You'll gain practice with C++-style Object Oriented Programming (OOP) with classes and virtual functions.

When you're done, you'll have a program that simulates a game of Euchre, supporting a AI player and a Human player.
```console
$ ./euchre.exe pack.in noshuffle 3 Ivan Human Judea Simple Kunle Simple Liskov Simple
Hand 0
Ivan deals
Jack of Diamonds turned up
Judea passes
Kunle passes
Liskov passes
Human player Ivan's hand: [0] Nine of Diamonds
Human player Ivan's hand: [1] Ten of Diamonds
Human player Ivan's hand: [2] Jack of Hearts
Human player Ivan's hand: [3] Queen of Hearts
Human player Ivan's hand: [4] Ace of Clubs
Human player Ivan, please enter a suit, or "pass":
Diamonds
Ivan orders up Diamonds
...
```
{: data-variant="legacy" }


## Setup
Set up your visual debugger and version control, then submit to the autograder.

### Visual debugger
During setup, name your project `p3-euchre`. Use this starter files link: `https://eecs280staff.github.io/euchre/starter-files.tar.gz`

| [VS Code](https://eecs280staff.github.io/tutorials/setup_vscode.html)| [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html) | [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html) |

If you created a `main.cpp` while following the setup tutorial, rename it to `euchre.cpp`. Otherwise, create a new file `euchre.cpp`.  You should end up with a folder with starter files that looks like this.  You may have already renamed files like `Card.cpp.starter` to `Card.cpp`.
```console
$ ls
Card.cpp.starter        Pack_public_tests.cpp     euchre_test00.out.correct
Card.hpp                Pack_tests.cpp.starter    euchre_test01.out.correct
Card_public_tests.cpp   Player.hpp                euchre_test50.in
Card_tests.cpp.starter  Player_public_tests.cpp   euchre_test50.out.correct
Makefile                Player_tests.cpp.starter  pack.in
Pack.hpp                euchre.cpp                unit_test_framework.hpp
```
{: data-variant="no-line-numbers" }

Here's a short description of each starter file.

| File(s) | Description |
| ------- | ----------- |
| `Card.hpp` | Abstraction representing a playing card. |
| `Card.cpp.starter` | Starter code for the `Card`. |
| `Card_tests.cpp` | Your `Card` unit tests. |
| `Card_public_tests.cpp` | Compile check test for `Card.cpp`. |
| `Pack.hpp` | Abstraction representing a pack cards. |
| `Pack_tests.cpp` | Add your `Pack` unit tests to this file. |
| `Pack_public_tests.cpp` | Compile check test for `Pack.cpp`. |
| `Player.hpp` | Abstraction representing a euchre player. |
| `Player_tests.cpp` | Your `Player` unit tests. |
| `Player_public_tests.cpp` | Compile check test for `Player.cpp`. |
| `pack.in` | Input file containing a Euchre deck. |
| `Makefile` | Helper commands for building. |
| `euchre_test00.out.correct` <br>`euchre_test01.out.correct` | Correct output for system tests with Simple player.  |
| `euchre_test50.in` <br>`euchre_test50.out.correct` | Input and correct output for system tests with Human player. |
| `unit_test_framework.hpp` | A simple unit-testing framework. |

### Version control
Set up version control using the [Version control tutorial](https://eecs280staff.github.io/tutorials/setup_git.html).

After you're done, you should have a local repository with a "clean" status and your local repository should be connected to a remote GitHub repository.
```console
$ git status
On branch main
Your branch is up-to-date with 'origin/main'.

nothing to commit, working tree clean
$ git remote -v
origin	https://github.com/awdeorio/p3-euchre.git (fetch)
origin	https://githubcom/awdeorio/p3-euchre.git (push)
```

You should have a `.gitignore` file ([instructions](https://eecs280staff.github.io/tutorials/setup_git.html#add-a-gitignore-file)).
```console
$ head .gitignore
# This is a sample .gitignore file that's useful for C++ projects.
...
```

### Group registration
Register your partnership (or working alone) on the  [Autograder](https://autograder.io/).  Then, submit the code you have.

## EECS 280 Euchre Rules

There are many variants of Euchre. Our particular version is based on
a variety commonly played in Michigan with a few changes to make it
feasible as a coding project.

Our step-by-step explanation of a game of "EECS 280 Euchre" can be found in a [YouTube video](https://www.youtube.com/watch?v=M0jGJ0NRcrc) and a [PDF](https://drive.google.com/file/d/14MSt1WdtnRbfgpPiMp9YuKn2wQ4Z83Ir/view?usp=sharing).

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Skim this section the first time through.  Refer back to it while you're coding.
</div>

### Players
{:.primer-spec-toc-ignore}

There are four players numbered 0-3. If the players sat around the
table, it would look like this:

![](images/image2.png){: .invert-colors-in-dark-mode }

There are two teams: players 0 and 2 are *partners*, as are 1 and 3.
Each player has left and right *neighbors*. For example, 1 is to the
left of 0, and 3 is to the right of 0. That means 1 is 0's left
*neighbor*, and 3 is 0's right *neighbor*.

### The Cards
{:.primer-spec-toc-ignore}

Euchre uses a deck of 24 *playing cards*, each of which has two
properties: a *rank* and a *suit*. The ranks are 9, 10, *Jack*,
*Queen*, *King*, and *Ace*, and the suits are *Spades*, *Hearts*,
*Clubs*, and *Diamonds*. Each card is unique &mdash; there are no duplicates.
Throughout this document, we sometimes refer to ranks or suits using
only the first letter of their name. Farther below, we describe how to
determine the [ordering of the cards](#value-of-cards).

### Playing the Game

At a high level, a game of Euchre involves several rounds, which are
called *hands*. Each hand consists of the following phases.

Each hand:

1.  [Setup table](#setup-table)
    1.  [Shuffle](#shuffle)
    2.  [Deal](#deal)
2.  [Making Trump](#making-trump)
    1.  [Round One](#round-one)
    2.  [Round Two](#round-two)
3.  [Trick Taking](#trick-taking)
4.  [Scoring](#scoring)

We describe each in more detail below.

### Setup table

#### Shuffle
{:.primer-spec-toc-ignore}

The dealer shuffles the deck at the beginning of each hand. The
algorithm you will implement for shuffling is a variant of a riffle
shuffle called an "in shuffle"
([https://en.wikipedia.org/wiki/In\_shuffle](https://en.wikipedia.org/wiki/In_shuffle)).
Cut the deck exactly in half and then interleave the two halves,
starting with the second half. Thus, the card originally at position
12 goes to position 0, the one originally at position 0 goes to
position 1, the one originally at position 13 goes to position 2, and
so on. Do this in-shuffle process 7 times.

You will also implement an option to run the game with shuffling
disabled - when this option is chosen, just reset the pack any time
shuffling would be called for. This may make for easier testing and
debugging.

#### Deal
{:.primer-spec-toc-ignore}

In each hand, one player is designated as the *dealer* (if humans were
playing the game, the one who passes out the cards). In our game,
player 0 deals during the first hand. Each subsequent hand, the role
of dealer moves one player to the left.

Each player receives five cards, dealt in alternating batches of 3 and 2.
That is, deal 3-2-3-2 cards then 2-3-2-3 cards, for a total of 5
cards each. The player to the left of the dealer receives the first
batch, and dealing continues to the left until 8 batches have been
dealt.

Four cards remain in the deck after the deal. The next card in the
pack is called the upcard (it is turned face up, while the other cards
are all face down). It plays a special role in the next phase. The
three remaining cards are not used for the current hand.

### Making Trump

During this phase, the trump suit is determined by whichever player
chooses to *order up*.

#### Round One
{:.primer-spec-toc-ignore}

The suit of the *upcard* is used to propose a *trump* suit whose cards
become more valuable during the upcoming hand. Players are given the
opportunity to *order up* (i.e. select the suit of the upcard to be
the trump suit) or *pass*, starting with the player to the dealer's
left (also known as the *eldest hand*) and progressing once around the
circle to the left. If any player orders up, the upcard's suit becomes
trump and the dealer is given the option to replace one of their cards
with the upcard.

#### Round Two
{:.primer-spec-toc-ignore}

If all players *pass* during the first round, there is a second round
of *making*, again beginning with the eldest hand. The upcard's suit
is rejected and cannot be ordered up. Instead, the players may *order
up* any suit other than the upcard's suit. The dealer does not have
the opportunity to pick up the upcard during round two.

If *making* reaches the dealer during the second round, a variant
called *screw the dealer* is invoked: the dealer must order up a suit
other than the rejected suit.

(Note for pro Euchre players: for simplicity, we have omitted "going
alone" in this version.)

### Trick Taking

Once the trump has been determined, five *tricks* are played. For each
trick, players take turns laying down cards, and whoever played the
highest card *takes* the trick.

During each trick, the player who plays first is called the *leader*.
For the first trick, the eldest hand leads.

At the beginning of each trick, the leader *leads* a card, which
affects which cards other players are allowed to play, as well as the
value of each card played (see below). Each other player must *follow
suit* (play a card with the same suit as the led card) if they are
able, and otherwise may play any card (it is removed from their hand).
Play moves to the left around the table, with each player playing one
card.

A trick is won by the player who played the highest valued card (see
below to determine comparative values). The winner of the trick
*leads* the next one.

### Scoring

The team that takes the majority of tricks receives points for that hand.
* If the team that ordered up the trump suit takes 3 or 4 tricks, they get 1 point.
* If the team that ordered up the trump suit takes all 5 tricks, they get 2 points. This is called a *march*.
* If the team that did not order up takes 3, 4, or 5 tricks, they receive 2 points. This is called *euchred*.

Traditionally, the first side to reach 10 points wins the game. In
this project, the number of points needed to win is specified when the
program is run.

### Value of cards

In order to determine which of two cards is better, you must pay
attention to the context in which they are being compared. There are
three separate contexts, which depend on whether or not a trump or led
suit is present.

In the simplest case, cards are ordered by rank (A \> K \> Q \> J \>
10 \> 9), with ties broken by suit (D \> C \> H \> S).

If a *trump suit* is present, all trump cards are more valuable than
non-trump cards. That means a 9 of the trump suit will beat an Ace of
a non-trump suit. Additionally, two special cards called *bowers* take
on different values than normal.

  - *Right Bower*: The Jack of the trump suit. This is the most
    valuable card in the game.
  - *Left Bower*: The Jack of the "same color" suit as trump is
    **considered to be a trump** (regardless of the suit printed on
    the card) and is the second most valuable card.

The suit of the left bower is called *next*, while the two suits of the opposite color are called *cross* suits.

If a *led suit* is present as well as a trump suit, the ordering is
the same except that all cards of the led suit are considered more
valuable than all non-trump-suit, non-led-suit cards. Note that it is
possible for the trump suit and led suit to be the same.

<div class="primer-spec-callout warning" markdown="1">
**Pitfall:** The left bower is always considered the trump suit.  For example, if Diamonds is trump, the Jack of Hearts is also considered a Diamond, not a Heart.
</div>

![](images/image1.png)

The above shows card orderings in the possible contexts. Cards in
higher rows are greater than those in lower rows. Within rows, cards
farther to the left are greater. Note the right bower (blue outline)
and left bower (red outline).

### Euchre Glossary

**Trump:** A suit whose cards are elevated above their normal rank
during play.

**Right Bower:** The Jack card of the *Trump* suit, which is
considered the highest-valued card in Euchre.

**Left Bower:** The Jack from the other suit of the same color as the
*Trump* suit, considered the second highest-valued card in Euchre. The
*Left Bower* is also considered a *Trump* card.

**Face or Ace:** The Jack, Queen, and King cards are generally pictured with people on them and are called "face" cards. These and the ace cards are more powerful than the 9 or 10 and factor into the simple player's strategy.

**Next Suit:** The suit of the same color as trump.

**Cross Suits:** The two suits of the opposite color as trump.

**Making:** The process in which a trump card is chosen, consists of
two rounds.

**Eldest:** Player to the left of the dealer.

**Upcard:** The up-facing card in front of the dealer that proposes
the trump suit.

**Order Up:** Accepts the *Upcard* suit.

**Pass:** Player rejects the suit and passes on the decision to the
next player.

**Screw the Dealer:** When making* reaches the dealer on round two,
*the dealer must *order up* a suit other than the rejected one.

**Lead:** The first card played by the eldest* hand, regardless of who
*is the maker.

**Leader:** Person playing the lead* card in a trick, allowed to lead*
*any card.

**March:** When the side that made trump* wins all 5 tricks.

**Euchred:** When the side that didn't make trump* wins 3, 4, or 5
*tricks.


## Card

Write an abstract data type (ADT) for a playing card.  Refer back to the [Value of cards section](#value-of-cards) while you're working on the card ADT.

Before getting started, take a look at the [Operator Overloading Tutorial](operator_overloading.html).  That will help you understand functions like:
```c++
bool operator<(const Card &lhs, const Card &rhs);
```

Next, see the short [Enumerated Type Tutorial](enum.html).  It explains code like this from `Card.hpp`.
```c++
enum Suit {
  SPADES   = 0,
  HEARTS   = 1,
  CLUBS    = 2,
  DIAMONDS = 3,
};
```

Write implementations in `Card.cpp` for the functions declared in
`Card.hpp`.

Run the public Card tests.
```console
$ make Card_public_tests.exe
$ ./Card_public_tests.exe
```

Write tests for `Card` in `Card_tests.cpp` using the [Unit Test Framework](https://eecs280staff.github.io/unit_test_framework/).  You'll submit these tests to the autograder.  See the [Unit Test Grading](#unit-test-grading) section.
```console
$ make Card_tests.exe
$ ./Card_tests.exe
```

Submit `Card.cpp` and `Card_tests.cpp` to the [autograder](https://autograder.io).

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Compare the underlying numeric representation of an `enum`.
```c++
Rank r = JACK;
r == JACK; // true

Suit s1 = SPADES;
Suit s2 = DIAMONDS;
s1 < s2;  // true
```
</div>

### Setup
Rename these files ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#rename-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#rename-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#rename-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#rename-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#mv)):
-  `Card.cpp.starter` -> `Card.cpp`
-  `Card_tests.cpp.starter` -> `Card_tests.cpp`

Edit `Card.cpp`, adding a function stub for each of the Card member functions in `Card.hpp`.
```c++
Card::Card() {
  assert(false);
}
```
{: data-title="Card.cpp" }

Edit `Card.cpp`, adding  function stubs for each of the non-member non-operator functions in `Card.hpp`.  Here are a few examples.
```c++
Suit Suit_next(Suit suit) {
  assert(false);
}

bool Card_less(const Card &a, const Card &b, Suit trump) {
  assert(false);
}
```
{: data-title="Card.cpp" }

Edit `Card.cpp`, adding  function stubs for each of the overloaded operators in `Card.hpp`.  Here are a few examples.
```c++
std::ostream & operator<<(std::ostream &os, const Card &card) {
  assert(false);
}

bool operator<(const Card &lhs, const Card &rhs) {
  assert(false);
}

```
{: data-title="Card.cpp" }

The Card tests should compile and run.  Expect them to fail at this point because the `Card.cpp` starter code contains function stubs.
```console
$ make Card_public_tests.exe
$ ./Card_public_tests.exe
$ make Card_tests.exe
$ ./Card_tests.exe
```

Configure your IDE to debug either the public tests or your own tests.

<table>
<thead>
<tr>
  <th></th>
  <th>
  Public tests
  </th>
  <th>
  Your own tests
  </th>
</tr>
</thead>
<tbody>
<tr>
  <td>
  <b>VS Code (macOS)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Card_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Card_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>VS Code (Windows)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Card_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Card_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>Xcode</b>
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Card_public_tests.cpp`, `Card.cpp`
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Card_tests.cpp`, `Card.cpp`
  </td>
</tr>
<tr>
  <td>
  <b>Visual Studio</b>
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Card_public_tests.cpp`
  - Exclude `Card_tests.cpp`, `Pack_public_tests.cpp`, `Pack_tests.cpp`, `Player_public_tests.cpp`, `Player_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Card_tests.cpp`
  - Exclude `Card_public_tests.cpp`, `Pack_public_tests.cpp`, `Pack_tests.cpp`, `Player_public_tests.cpp`, `Player_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
</tr>
</tbody>
</table>

### Testing
This section contains a few tips for writing your own tests in `Card_tests.cpp`.

Here's how to create a Card and check its rank and suit.
```c++
Card ace_hearts(ACE, HEARTS);
ASSERT_EQUAL(ace_hearts.get_rank(), ACE);
ASSERT_EQUAL(ace_hearts.get_suit(), HEARTS);
```

In this example, the overloaded less-than operator runs: `bool operator<(const Card &lhs, const Card &rhs);`.  Notice that the suits are ordered to break ties (see the [Value of Cards](#value-of-cards) section).
```c++
Card ace_hearts(ACE, HEARTS);
Card ace_diamonds(ACE, DIAMONDS);
ASSERT_TRUE(ace_hearts < ace_diamonds);
```

## Pack

Write an abstract data type (ADT) for a pack of playing cards.

Write implementations in `Pack.cpp` for the functions declared in
`Pack.hpp`.

Run the public Pack tests.
```console
$ make Pack_public_tests.exe
$ ./Pack_public_tests.exe
```

Write tests for `Pack` in `Pack_tests.cpp` using the unit test framework. While you should write your own tests for `Pack` to ensure that your implementation is correct, you do not have to submit your tests to the autograder.
```console
$ make Pack_tests.exe
$ ./Pack_tests.exe
```

Submit `Pack.cpp` to the [autograder](https://autograder.io).  Don't forget to include the code you finished earlier, `Card.cpp` and `Card_tests.cpp`.

### Default constructor
{: .primer-spec-toc-ignore }

The default `Pack` constructor initializes a full pack with cards in order from lowest to highest.
```c++
Pack();
```

You can take advantage of an `enum`'s underlying integer representation to loop over all its values.
```c++
for (int s = SPADES; s <= DIAMONDS; ++s) {
  Suit suit = static_cast<Suit>(s);
}
```

### Stream input constructor
{: .primer-spec-toc-ignore }

A second `Pack` constructor reads a pack from stream input. [Later](#command-line-arguments), your `main` function will open an input file and then call this constructor on the input stream.
### Setup
Rename these files ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#rename-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#rename-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#rename-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#rename-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#mv)):
-  `Pack_tests.cpp.starter` -> `Pack_tests.cpp`

Create a new file `Pack.cpp`.  Add a function stub for every function prototype in `Pack.hpp`.  Remember to `#include "Pack.hpp"`.

For example:
```c++
#include "Pack.hpp"
void Pack::reset() {
  assert(false);
  assert(next); // DELETEME: avoid error "private field is not used"
}
```
{: data-title="Pack.cpp" }

The Pack tests should compile and run.  Expect them to fail at this point because the `Pack.cpp` contains function stubs.
```console
$ make Pack_public_tests.exe
$ ./Pack_public_tests.exe
$ make Pack_tests.exe
$ ./Pack_tests.exe
```

Configure your IDE to debug either the public tests or your own tests.

<table>
<thead>
<tr>
  <th></th>
  <th>
  Public tests
  </th>
  <th>
  Your own tests
  </th>
</tr>
</thead>
<tbody>
<tr>
  <td>
  <b>VS Code (macOS)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Pack_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Pack_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>VS Code (Windows)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Pack_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Pack_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>Xcode</b>
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Pack_public_tests.cpp`, `Pack.cpp`, `Card.cpp`
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Pack_tests.cpp`, `Pack.cpp`, `Card.cpp`
  </td>
</tr>
<tr>
  <td>
  <b>Visual Studio</b>
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Pack_public_tests.cpp`
  - Exclude `Pack_tests.cpp`, `Card_public_tests.cpp`, `Card_tests.cpp`, `Player_public_tests.cpp`, `Player_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Pack_tests.cpp`
  - Exclude `Pack_public_tests.cpp`, `Card_public_tests.cpp`, `Card_tests.cpp`, `Player_public_tests.cpp`, `Player_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
</tr>
</tbody>
</table>

### Reading the Pack

One of the `Pack` constructors reads a pack from stream input. [Later](#interface), your `main` function will open an input file and then call this constructor on the input stream.
```c++
Pack(std::istream& pack_input);
```
{: data-title="Pack.hpp" }

The `pack.in` file provided with the project contains an example of the input format, with cards listed in "new pack" order:

```
Nine of Spades
Ten of Spades
Jack of Spades
...
Queen of Diamonds
King of Diamonds
Ace of Diamonds
```
{: data-title="pack.in" data-variant="no-line-numbers" }

Your implementation may assume any pack input is formatted correctly, with exactly 24
unique and correctly formatted cards.

## Player

We'll implement two players: a simple AI player and a human-controlled player that reads instructions from standard input (`cin`).  We have provided an abstract base class in `Player.hpp`.

### Setup
Rename these files ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#rename-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#rename-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#rename-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#rename-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#mv)):
-  `Player_tests.cpp.starter` -> `Player_tests.cpp`

Create a new file `Player.cpp`.  Add function stubs for the two "vanilla" functions in `Player.hpp`. Remember to `#include "Player.hpp"`. Later, you'll add new classes and member functions using derived classes, which you can ignore for now.
```c++
#include "Player.hpp"
Player * Player_factory(const std::string &name, const std::string &strategy) {
  assert(false);
}

std::ostream & operator<<(std::ostream &os, const Player &p) {
  assert(false);
}
```
{: data-title="Player.cpp" }

The Player tests should compile and run.  Expect them to fail at this point because the `Player.cpp` contains function stubs.
```console
$ make Player_public_tests.exe
$ ./Player_public_tests.exe
$ make Player_tests.exe
$ ./Player_tests.exe
```

Configure your IDE to debug either the public tests or your own tests.

<table>
<thead>
<tr>
  <th></th>
  <th>
  Public tests
  </th>
  <th>
  Your own tests
  </th>
</tr>
</thead>
<tbody>
<tr>
  <td>
  <b>VS Code (macOS)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Player_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Player_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>VS Code (Windows)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Player_public_tests.exe`
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/Player_tests.exe`
  </td>
</tr>
<tr>
  <td>
  <b>Xcode</b>
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Player_public_tests.cpp`, `Player.cpp`, `Pack.cpp`, `Card.cpp`
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `Player_tests.cpp`, `Player.cpp`, `Pack.cpp`, `Card.cpp`
  </td>
</tr>
<tr>
  <td>
  <b>Visual Studio</b>
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Player_public_tests.cpp`
  - Exclude `Player_tests.cpp`, `Card_public_tests.cpp`, `Card_tests.cpp`, `Pack_public_tests.cpp`, `Pack_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `Player_tests.cpp`
  - Exclude `Player_public_tests.cpp`, `Card_public_tests.cpp`, `Card_tests.cpp`, `Pack_public_tests.cpp`, `Pack_tests.cpp`, `euchre.cpp` (if present), `main.cpp` (if present)
  </td>
</tr>
</tbody>
</table>

### Player factory

Since the specific types of Players are hidden inside `Player.cpp`, we
need to write a *factory function* that returns a pointer to a
`Player` with the correct dynamic type. We also need the pointed-to
objects to stick around after the factory function finishes, so we'll
create the players using *dynamically allocated memory*. The prototype
for `Player_factory` can be found in `Player.hpp`, and the
implementation will go in `Player.cpp`.

```c++
Player * Player_factory(const std::string &name, 
                        const std::string &strategy) {
  // We need to check the value of strategy and return 
  // the corresponding player type.
  if (strategy == "Simple") {
    // The "new" keyword dynamically allocates an object.
    return new SimplePlayer(name);
  }
  // Repeat for each other type of Player
  ...
  // Invalid strategy if we get here
  assert(false);
  return nullptr;
}
```
{: data-title="Player.cpp" }

### Simple Player

Implement the Simple Player in `Player.cpp` by creating a class that derives from our `Player` abstract base class.  Override each Player function in `Player.cpp` for the functions declared in `Player.hpp`. 

Much of the strategy for our Simple Player can be implemented using
the comparison functions provided by the Card interface.

The Simple Player should not print any output.

Run the public Simple Player tests.
```console
$ make Player_public_tests.exe
$ ./Player_public_tests.exe
```

Write tests for the Simple Player in `Player_tests.cpp` using the [Unit Test Framework](https://eecs280staff.github.io/unit_test_framework/).  You'll submit these tests to the autograder.  See the [Unit Test Grading](#unit-test-grading) section.
```console
$ make Player_tests.exe
$ ./Player_tests.exe
```

Submit `Player.cpp` and `Player_tests.cpp` to the [autograder](https://autograder.io).  Don't forget to include the code you finished earlier, `Card.cpp`, `Card_tests.cpp`, and `Pack.cpp`.

#### Making
{:.primer-spec-toc-ignore}

In making trump, a Simple Player considers the upcard, which player
dealt, and whether it is the first or second round of making trump. A
more comprehensive strategy would consider the other players'
responses, but we will keep it simple.

During round one, a Simple Player considers ordering up the suit
of the upcard, which would make that suit trump. They will order up if
that would mean they have two or more cards that are either face or ace
cards of the trump suit (the right and left bowers, and Q, K, A of
the trump suit, which is the suit proposed by the upcard). (A Simple
Player does not consider whether they are the dealer and could gain an
additional trump by picking up the upcard.)

During round two, a Simple Player considers ordering up the suit with
the same color as the upcard, which would make that suit trump. They
will order up if that would mean they have one or more cards that are either
face or ace cards of the trump suit in their hand (the right and left bowers, and Q, K, A of the order-up
suit). For example, if the upcard is a Heart and the player has the
King of Diamonds in their hand, they will order up Diamonds. The
Simple Player will not order up any other suit. If making reaches the
dealer during the second round, we invoke *screw the dealer*, where
the dealer is forced to order up. In the case of screw the dealer, the
dealer will always order up the suit with the same color as the
upcard.

#### Adding the Upcard and Discarding a Card
{:.primer-spec-toc-ignore}

If the trump suit is ordered up during round one, the dealer picks up
the upcard. The dealer then discards the lowest card in their hand,
even if this is the upcard, for a final total of five cards. (Note that
at this point, the trump suit is the suit of the upcard.)

#### Leading Tricks
{:.primer-spec-toc-ignore}

When a Simple Player leads a trick, they play the highest non-trump
card in their hand. If they have only trump cards, they play the
highest trump card in their hand.

#### Playing Tricks
{:.primer-spec-toc-ignore}

When playing a card, Simple Players use a simple strategy that
considers only the suit that was led. A more complex strategy would
also consider the cards on the table.

If a Simple Player can follow suit, they play the highest card that
follows suit. Otherwise, they play the lowest card in their hand.

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Here's a nice way to remove a Card from a vector using [`vector::erase`](https://cplusplus.com/reference/vector/vector/erase/).
```c++
vector<Card> hand;  // Assume hand is full of cards
hand.erase(hand.begin() + i);  // Remove card i
```
</div>

### Human Player

The Human Player reads input from the human user. You may assume all
user input is correctly formatted and has correct values. You may also
assume the user will follow the rules of the game and not try to
cheat. See [Example With Human Players](#example-with-human-players) for
exact output for a game with a human player.

Implement the Human Player in `Player.cpp` by creating a class that derives from our `Player` abstract base class.  Override each Player function in `Player.cpp` for the functions declared in `Player.hpp`. 

The tests in `Player_tests.cpp` and `Player_public_tests.cpp` test only the Simple Player.  You'll have to wait until you have a working Euchre game driver to test the Human player.

Submit `Player.cpp` again to the [autograder](https://autograder.io).  Don't forget to include the code you finished earlier, `Card.cpp`, `Card_tests.cpp`, `Pack.cpp`, and `Player_tests.cpp`.

#### Making Trump
{:.primer-spec-toc-ignore}

When making trump reaches a Human Player, first print the Player's
hand. Then, prompt the user for their decision to pass or order up. The user will
then enter one of the following: "Spades", "Hearts", "Clubs",
"Diamonds", or "pass" to either order up the specified suit or pass.
This procedure is the same for both rounds of making trump.

Sample output code:
```c++
print_hand();
cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";
```

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Add a private member function that prints out the human player's hand.
```c++
void print_hand() const {
  for (size_t i=0; i < hand.size(); ++i)
    cout << "Human player " << name << "'s hand: "
         << "[" << i << "] " << hand[i] << "\n";
}
```
</div>

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Use the `string_to_suit()` function provided with the starter code.
```c++
string decision;
cin >> decision;

if (decision != "pass") {
  Suit ordered_up = string_to_suit(decision);
  ...
}
else {
  ...
}
```
</div>

#### Adding the Upcard and Discarding
{:.primer-spec-toc-ignore}

If a Human Player is the dealer and someone orders up during the first
round of making, the Human Player will pick up the upcard and discard
a card of their choice. Print the Player's hand and an option to
discard the upcard. Then, prompt the user to select a card to discard.
The user will then enter the number corresponding to the card they
want to discard (or -1 if they want to discard the upcard).

Sample output code:
```c++
print_hand();
cout << "Discard upcard: [-1]\n";
cout << "Human player " << name << ", please select a card to discard:\n";
```

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Make "sorted card ordering" a _representation invariant_ of your Human Player class and ensure that the hand is always kept in sorted order.
For consistency with autograder test cases, the cards in a human player's hand must always be printed in ascending order, as defined by the `<` operator in `Card.hpp`.

Use the STL to sort a `vector<Card> hand`:
```c++
#include <algorithm>
// ...
std::sort(hand.begin(), hand.end());
```
{: data-variant="no-line-numbers" }

**Pitfall:** Using `sort` on a member variable in a `const` member function leads to a confusing error, `no matching function for call to 'swap'`.  Instead, call `sort` when adding a card to the `hand`.
```c++
void Human::print_hand() const {
  std::sort(hand.begin(), hand.end()); // Error!
  // ...
}
```
</div>

#### Playing and Leading Tricks
{:.primer-spec-toc-ignore}

When it is the Human Player's turn to lead or play a trick, first
print the Player's hand. Then, prompt the user to select a card. The user
will then enter the number corresponding to the card they want to
play.

Sample output code:
```c++
print_hand();
cout << "Human player " << name << ", please select a card:\n";
```


## Euchre Game

Write the Euchre Game in `euchre.cpp` following the [EECS 280 Euchre Rules](#eecs-280-euchre-rules).

Run a Euchre game with four players.
```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple
```

### Setup
Make sure you have created `euchre.cpp` ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#add-new-files), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#add-new-files), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#add-new-files),  [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#add-new-files), [CLI](https://eecs280staff.github.io/tutorials/cli.html#touch)).

Add "hello world" code if you haven't already.
```c++
#include <iostream>
using namespace std;

int main() {
  cout << "Hello World!\n";
}
```

The euchre program should compile and run.
```console
$ make euchre.exe
$ ./euchre.exe
Hello World!
```

Configure your IDE to debug the euchre program.

<table>
<tr>
  <td>
  <b>VS Code (macOS)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/euchre.exe`
  </td>
</tr>
<tr>
  <td>
  <b>VS Code (Windows)</b>
  </td>
  <td markdown="1">
  Set [program name](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#edit-launchjson-program) to: <br>
  `${workspaceFolder}/euchre.exe`
  </td>
</tr>
<tr>
  <td>
  <b>Xcode</b>
  </td>
  <td markdown="1">
  Include [compile sources](https://eecs280staff.github.io/tutorials/setup_xcode.html#compile-sources): <br>
  `euchre.cpp`, `Card.cpp`, `Pack.cpp`, `Player.cpp`
  </td>
</tr>
<tr>
  <td>
  <b>Visual Studio</b>
  </td>
  <td markdown="1">
  [Exclude files](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#exclude-files-from-build) from the build: <br>
  - Include `euchre.cpp`
  - Exclude `Card_public_tests.cpp`, `Card_tests.cpp`, `Pack_public_tests.cpp`, `Pack_tests.cpp`, `Player_public_tests.cpp`, `Player_tests.cpp`, `main.cpp` (if present)
  </td>
</tr>
</table>

Configure command line arguments ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#arguments-and-options), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#arguments-and-options), [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#arguments-and-options), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#arguments-and-options)).  We recommend starting with the euchre test 00: `pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple`

To compile and run a Euchre game with four players.
```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple
```

### Interface

The Euchre game driver takes several command line arguments, for example:

```console
$ ./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple
```
{: data-variant="no-line-numbers" }

Here's a short description of each command line argument.

| `./euchre.exe` | Name of the executable |
| `pack.in` | Filename of the pack |
| `noshuffle` | Don't shuffle the deck, or use `shuffle` to turn on shuffling |
| `1` | Points to win the game |
| `Adi` | Name of player 0 |
| `Simple` | Type of player 0 |
| `Barbara` | Name of player 1 |
| `Simple` | Type of player 1 |
| `Chi-Chih` | Name of player 2 |
| `Simple` | Type of player 2 |
| `Dabbala` | Name of player 3 |
| `Simple` | Type of player 3 |

Check for each of these errors:
  - There are exactly 12 arguments, including the executable name.
  - Points to win the game is between 1 and 100, inclusive.
  - The shuffle argument is either `shuffle` or `noshuffle`.
  - The types of each of the players are either `Simple` or `Human`.

If there is an error print this message and no other output.  Exit returning a non-zero value from `main`.

```c++
cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
     << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
     << "NAME4 TYPE4" << endl;
```

You must also verify the pack input file opens successfully. If it does not,
print this error message and exit returning a non-zero value from `main`.

```c++
// Assume pack_filename is a variable containing
// the specified pack filename from argv
cout << "Error opening " << pack_filename << endl;
```
{: data-variant="no-line-numbers" }

If the file opens successfully, you may assume it is formatted correctly.

### Design

The Euchre game driver coordinates all the [actions in the game](#playing-the-game).  *Take some time* to design this complex piece of code before beginning.

We recommend writing a `Game` ADT. It should have a constructor that takes in details like the players, points to win, etc. and a public `play()` function.  The `Game` and its helper functions do the hard work, and the `main()` function is simple.

```c++
class Game {
 public:
  // primer-spec-highlight-start
  Game(/* game details */);
  void play();
  // primer-spec-highlight-end
};

int main(int argc, char **argv) {
  // Read command line args and check for errors
  // primer-spec-highlight-start
  Game game(/* game details */);
  game.play();
  // primer-spec-highlight-end
}
```

Next, consider which member data you'll need.  Examples include player pointers, pack of cards, etc.
```c++
class Game {
 public:
  Game(/* ... */);
  void play();

 private:
  // primer-spec-highlight-start
  std::vector<Player*> players;
  Pack pack;
  // ...
  // primer-spec-highlight-end
};
```

Which helper functions would help simplify the `play()` function?  Add these as private member functions.  Functions that shuffle, deal, make trump, and play a hand are a good starting point. 
```c++
class Game {
 public:
  Game(/* ... */);
  void play();

 private:
  std::vector<Player*> players;
  Pack pack;
  // ...

  // primer-spec-highlight-start
  void shuffle();
  void deal(/* ... */);
  void make_trump(/* ... */);
  void play_hand(/* ... */);
  // ...
  // primer-spec-highlight-end
};
```

#### End of game
{: .primer-spec-toc-ignore }

At the end of a game, delete the `Player` objects created by the [`Player_factory`](#player-factory).

```c++
for (size_t i = 0; i < players.size(); ++i) {
  delete players[i];
}
```

### Example With Simple Players

We've provided full example output for a game with four simple players in `euchre_test00.out.correct`.  This section explains the output line by line.

As a reminder, all the output in this example is produced by the Euchre Game driver, not by the simple player code in `Player.cpp`.

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Debug differences in game output using `diff -y -B`, which shows differences side-by-side and ignores whitespace.  We'll use the `less` pager so we can scroll through the long terminal output.  Press `q` to quit.
```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple > euchre_test00.out
$ diff -y -B euchre_test00.out.correct euchre_test00.out | less  # q to quit
```
</div>

Run the example.

```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple
```

First, print the executable and all arguments on the first line. Print
a single space at the end, which makes it easier to print an array.

```
./euchre.exe pack.in noshuffle 1 Adi Simple Barbara Simple Chi-Chih Simple Dabbala Simple 
```
{: data-variant="legacy" }

At the beginning of each hand, announce the hand, starting at zero,
followed by the dealer and the upcard.

```
Hand 0
Adi deals
Jack of Diamonds turned up
```
{: data-variant="legacy" }

Print the decision of each player during the making procedure. Print
an extra newline when making, adding, and discarding is complete.

```
Barbara passes
Chi-Chih passes
Dabbala passes
Adi passes
Barbara orders up Hearts
```
{: data-variant="legacy" }

Each of the five tricks is announced, including the lead, cards played
and the player that took the trick. Print an extra newline at the end
of each trick.

```
Jack of Spades led by Barbara
King of Spades played by Chi-Chih
Ace of Spades played by Dabbala
Nine of Diamonds played by Adi
Dabbala takes the trick

```
{: data-variant="legacy" }

At the end of the hand, print the winners of the hand. When printing
the names of a partnership, print the player with the lower index
first. For example, Adi was specified on the command line before
Chi-Chih, so he goes first.

```
Adi and Chi-Chih win the hand
```
{: data-variant="legacy" }

If a march occurs, print `march!` followed by a newline. If euchre
occurs, print `euchred!` followed by a newline. If neither occurs,
print nothing.

```
euchred!
```
{: data-variant="legacy" }

Print the score, followed by an extra newline.

```
Adi and Chi-Chih have 2 points
Barbara and Dabbala have 0 points
```
{: data-variant="legacy" }

When the game is over, print the winners of the game.

```
Adi and Chi-Chih win!
```
{: data-variant="legacy" }

The output for a much longer game with four simple players is in `euchre_test01.out.correct`. See the [Euchre Test 01 Explanation](euchre_test01.html) for an explanation of the output.

### Example With Human Players

We’ve provided full example output for a game with four human players in `euchre_test50.out.correct`. The input is in `euchre_test50.in`.  This section explains the output line by line.

Output that is specific to the Human Player should be printed by the Human Player code in (`Player.cpp`). All other output that is common to both Simple and Human Players should be printed by the Euchre Game driver.

Set up input redirection ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#input-redirection), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#input-redirection), [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#input-redirection), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#input-redirection)) to read `euchre_test50.in`. 

Configure command line arguments ([VS Code (macOS)](https://eecs280staff.github.io/tutorials/setup_vscode_macos.html#arguments-and-options), [VS Code (Windows)](https://eecs280staff.github.io/tutorials/setup_vscode_wsl.html#arguments-and-options), [Xcode](https://eecs280staff.github.io/tutorials/setup_xcode.html#arguments-and-options), [Visual Studio](https://eecs280staff.github.io/tutorials/setup_visualstudio.html#arguments-and-options)) for euchre test 50: `noshuffle 3 Ivan Human Judea Human Kunle Human Liskov Human`

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Similar to the simple player, you can debug differences in game output using `diff -y -B`.  Press `q` to quit.
```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 3 Ivan Human Judea Human Kunle Human Liskov Human < euchre_test50.in > euchre_test50.out
$ diff -y -B euchre_test50.out.correct euchre_test50.out | less  # q to quit
```
</div>

Run the example, redirecting the input file `euchre_test50.in` to stdin.

```console
$ make euchre.exe
$ ./euchre.exe pack.in noshuffle 3 Ivan Human Judea Human Kunle Human Liskov Human < euchre_test50.in
```

First, print the executable and all arguments on the first line. Print
a single space at the end, which makes it easier to print an array.

```
./euchre.exe pack.in noshuffle 3 Ivan Human Judea Human Kunle Human Liskov Human 
```
{: data-variant="legacy" }

At the beginning of each hand, announce the hand, starting at zero,
followed by the dealer and the upcard.

```
Hand 0
Ivan deals
Jack of Diamonds turned up
```
{: data-variant="legacy" }

Print the hand of each player during the making procedure, followed by
a prompt for their making decision. End the prompt with a newline
immediately after the colon.

```
Human player Judea's hand: [0] Nine of Spades
Human player Judea's hand: [1] Ten of Spades
Human player Judea's hand: [2] Jack of Spades
Human player Judea's hand: [3] King of Hearts
Human player Judea's hand: [4] Ace of Hearts
Human player Judea, please enter a suit, or "pass":
```
{: data-variant="legacy" }

Print the decision of each player during the making procedure.

```
Judea passes
...
Judea orders up Hearts
```
{: data-variant="legacy" }

Print the dealer's hand if a player orders up during the first round,
as well as an option to discard the upcard. Prompt the dealer to
select a card to discard, ending the prompt with a newline immediately
after the colon. Print an extra newline when making, adding, and
discarding is done.

```
Human player Ivan's hand: [0] Nine of Diamonds
...
Human player Ivan's hand: [4] Ace of Clubs
Discard upcard: [-1]
Human player Ivan, please select a card to discard:
```
{: data-variant="legacy" }

For each trick, print the Human Player's hand and prompt them to
select a card.

```
Human player Judea's hand: [0] Nine of Spades
...
Human player Judea's hand: [4] Ace of Hearts
Human player Judea, please select a card:
```
{: data-variant="legacy" }

Then print the card played or lead.

```
Nine of Spades led by Judea
```
{: data-variant="legacy" }

At the end of each trick, print the player who took the trick as well
as an extra newline.

```
Liskov takes the trick
```
{: data-variant="legacy" }

At the end of the hand, print the winners of the hand. When printing
the names of a partnership, print the player with the lower index
first. For example, Ivan was specified on the command line before
Kunle, so he goes first.

```
Ivan and Kunle win the hand
```
{: data-variant="legacy" }

If a march occurs, print `march!` followed by a newline. If euchre
occurs, print `euchred!` followed by a newline. If neither occurs, print
nothing.

```
euchred!
```
{: data-variant="legacy" }

Print the score, followed by an extra newline.

```
Ivan and Kunle have 2 points
Judea and Liskov have 0 points
```
{: data-variant="legacy" }

When the game is over, print the winners of the game.

```
Ivan and Kunle win!
```
{: data-variant="legacy" }


## Submission and Grading

Submit these files to the [autograder](https://autograder.io).
  - `Card.cpp`
  - `Card_tests.cpp`
  - `Pack.cpp`
  - `Player.cpp`
  - `Player_tests.cpp`
  - `euchre.cpp`

You do not have to submit `Pack_tests.cpp` to the autograder.

This project will be autograded for correctness, comprehensiveness of
your test cases, and programming style. See the [style checking
tutorial](https://eecs280staff.github.io/tutorials/setup_style.html)
for the criteria and how to check your style automatically on CAEN.

### Checkpoint

This project is graded on an early checkpoint as well as the full submission. The checkpoint submission is worth 0.5% of your overall course grade. The full submission is worth 9.5% of your overall course grade.

The checkpoint consists of everything except the [Euchre Game](#euchre-game) driver.

Submit these files to the [autograder](https://autograder.io).
  - `Card.cpp`
  - `Card_tests.cpp`
  - `Pack.cpp`
  - `Player.cpp`
  - `Player_tests.cpp`

There are no private tests and no style grading at the checkpoint.

Your checkpoint score does not change after the checkpoint deadline, even though we run the same public tests and [mutation tests](#unit-test-grading) at both the checkpoint and full submission.

After the checkpoint autograder closes, the full submission autograder will open.

### Testing

Run all the unit tests and system tests.  This includes the public tests we provided and the unit tests that you wrote.

```console
$ make test
```

<div class="primer-spec-callout info" markdown="1">
**Pro-tip:** Run commands in parallel with `make -j`.
```console
$ make -j4 test
```
{: data-variant="no-line-numbers" }
</div>

### Unit Test Grading

We will autograde your `Card_tests.cpp` and `Player_tests.cpp` unit tests. Note that your player tests should only test the "simple player", not the "human player".

Your unit tests must use the [unit test framework](https://eecs280staff.github.io/unit_test_framework/).

A test suite must run in less than 5 seconds and contain 50 or fewer `TEST()` functions.  One test suite is one `_tests.cpp` file.

To grade your unit tests, we use a set of intentionally buggy instructor solutions.  You get points for catching the bugs.

1.  We compile and run your unit tests with a **correct solution**.
    - Tests that pass are **valid**.
    - Tests that fail are **invalid**, they falsely report a bug.
3.  We compile and run all of your **valid** tests against each **buggy solution**.
    - If any of your tests fail, you caught the bug.
    - You earn points for each bug that you catch.

### Requirements and Restrictions

It is our goal for you to gain practice with good C++ code, classes,
and polymorphism.


| DO | DO NOT |
| -- | ------ |
| Modify `.cpp` files | Modify `.hpp` files |
| Write helper functions in `Card.cpp` and `Pack.cpp` as non-member functions in the `.cpp` files and declare them `static` | Modify `.hpp` files |
| Use these libraries: `<iostream>`, `<fstream>`, `<cstdlib>`, `<cassert>`, `<cstring>`, `<string>`, `<array>`, `<vector>`, `<sstream>` <br>Use the `<algorithm>` library, but only for the `sort()` function. | Use other libraries. <br><br>Use `<algorithm>` library for anything other than the `sort()` function. |
| `#include` a library to use its functions | Assume that the compiler will find the library for you (some do, some don't) |
| | `#include` an unnecessary `.hpp` file, such as `#include Pack.hpp` in either `Card.cpp` or in `Player.cpp` (This introduces undesirable dependencies, e.g. that the `Card`/`Player` ADTs require the `Pack` ADT to exist and be properly implemented.) |
| Use C++ strings | Use C-strings other than when checking `argv` |
| Send all output to standard out (AKA stdout) by using `cout` | Send any output to standard error (AKA stderr) by using `cerr` |
| `const` global variables | Global or static variables |
| Pass large structs or classes by reference | Pass large structs or classes by value |
| Pass by `const` reference when appropriate | "I don't think I'll modify it ..." |
| Variables on the stack | Dynamic memory (`new`, `malloc()`, etc.) outside of the `Player_factory`. |


## Acknowledgments

The original project was written by Andrew DeOrio, Fall 2013. The
project was modified to use C++ style object oriented programming and
the specification updated by the Fall 2015 staff.
