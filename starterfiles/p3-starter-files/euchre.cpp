#include <iostream>
#include <fstream>
#include <string>
#include "Card.hpp"
#include "Pack.hpp"
#include "Player.hpp"
#include <cstring>

class Game {
    public:
     Game(/* ... */);
     void play();
   
    private:
     std::vector<Player*> players;
     Pack pack;
     // ...
   
     void shuffle();
     void deal(/* ... */);
     void make_trump(/* ... */);
     void play_hand(/* ... */);
     // ...
   };
   