#include "Player.hpp"

class SimplePlayer : public Player {
    public:
     //EFFECTS: Creates a SimplePlayer with the given name
     SimplePlayer(const std::string &name);
   
     //EFFECTS: Returns player's name
     const std::string & get_name() const;
   
     //REQUIRES player has less than MAX_HAND_SIZE cards
     //EFFECTS  adds Card c to Player's hand
     void add_card(const Card &c);
   
     //REQUIRES round is 1 or 2
     //EFFECTS If Player wishes to order up a trump suit then return true and
     //  change order_up_suit to desired suit.  If Player wishes to pass, then do
     //  not modify order_up_suit and return false.
     // In making trump, a Simple Player considers the upcard, which player dealt, and whether it is the first or second round of making trump. A more comprehensive strategy would consider the other players’ responses, but we will keep it simple.
     // During round one, a Simple Player considers ordering up the suit of the upcard, which would make that suit trump. They will order up if that would mean they have two or more cards that are either face or ace cards of the trump suit (the right and left bowers, and Q, K, A of the trump suit, which is the suit proposed by the upcard). (A Simple Player does not consider whether they are the dealer and could gain an additional trump by picking up the upcard.)
     // During round two, a Simple Player considers ordering up the suit with the same color as the upcard, which would make that suit trump. They will order up if that would mean they have one or more cards that are either face or ace cards of the trump suit in their hand (the right and left bowers, and Q, K, A of the order-up suit). For example, if the upcard is a Heart and the player has the King of Diamonds in their hand, they will order up Diamonds. The Simple Player will not order up any other suit. If making reaches the dealer during the second round, we invoke screw the dealer, where the dealer is forced to order up. In the case of screw the dealer, the dealer will always order up the suit with the same color as the upcard.
     bool make_trump(const Card &upcard, bool is_dealer,
                     int round, Suit &order_up_suit) const;
   
     //REQUIRES Player has at least one card
     //EFFECTS  If the trump suit is ordered up during round one, the dealer picks up the upcard. The dealer then discards the lowest card in their hand, even if this is the upcard, for a final total of five cards. (Note that at this point, the trump suit is the suit of the upcard.)
     void add_and_discard(const Card &upcard);
   
     //REQUIRES Player has at least one card
     //EFFECTS  Leads one Card from Player's hand according to their strategy
     //  "Lead" means to play the first Card in a trick.  The card
     //  is removed the player's hand.
     //  When a Simple Player leads a trick, they play the highest non-trump card in their hand. If they have only trump cards, they play the highest trump card in their hand.
     Card lead_card(Suit trump);
   
     //REQUIRES Player has at least one card
     //EFFECTS  Plays one Card from Player's hand according to their strategy.
     //  The card is removed from the player's hand.
     //  When playing a card, Simple Players use a simple strategy that considers only the suit that was led. A more complex strategy would also consider the cards on the table.
     //  If a Simple Player can follow suit, they play the highest card that follows suit. Otherwise, they play the lowest card in their hand.
     Card play_card(const Card &led_card, Suit trump);
   
    private:
     std::string name;
     std::vector<Card> hand;
   };

// The Human Player reads input from the human user. You may assume all user input is correctly formatted and has correct values. You may also assume the user will follow the rules of the game and not try to cheat.
class HumanPlayer: public Player {
    public:
    //EFFECTS: Creates a SimplePlayer with the given name
    HumanPlayer(const std::string &name);
  
    //EFFECTS: Returns player's name
    const std::string & get_name() const;
  
    //REQUIRES player has less than MAX_HAND_SIZE cards
    //EFFECTS  adds Card c to Player's hand
    void add_card(const Card &c);
  
    //REQUIRES round is 1 or 2
    //EFFECTS  If Player wishes to order up a trump suit then return true and
  //  change order_up_suit to desired suit.  If Player wishes to pass, then do
  //  not modify order_up_suit and return false.
  //  When making trump reaches a Human Player, first print the Player’s hand. Then, prompt the user for their decision to pass or order up. The user will then enter one of the following: “Spades”, “Hearts”, “Clubs”, “Diamonds”, or “pass” to either order up the specified suit or pass. This procedure is the same for both rounds of making trump.
    bool make_trump(const Card &upcard, bool is_dealer,
                    int round, Suit &order_up_suit) const;
  
    //REQUIRES Player has at least one card
    //EFFECTS  Player adds one card to hand and removes one card from hand.
    //  If a Human Player is the dealer and someone orders up during the first round of making, the Human Player will pick up the upcard and discard a card of their choice. Print the Player’s hand and an option to discard the upcard. Then, prompt the user to select a card to discard. The user will then enter the number corresponding to the card they want to discard (or -1 if they want to discard the upcard).
    void add_and_discard(const Card &upcard);
  
    //REQUIRES Player has at least one card
    //EFFECTS  Leads one Card from Player's hand according to their strategy
    //  "Lead" means to play the first Card in a trick.  The card
    //  is removed the player's hand.
    //  When it is the Human Player’s turn to lead or play a trick, first print the Player’s hand. Then, prompt the user to select a card. The user will then enter the number corresponding to the card they want to play.
    Card lead_card(Suit trump);
  
    //REQUIRES Player has at least one card
    //EFFECTS  Plays one Card from Player's hand according to their strategy.
    //  The card is removed from the player's hand.
    //   When it is the Human Player’s turn to lead or play a trick, first print the Player’s hand. Then, prompt the user to select a card. The user will then enter the number corresponding to the card they want to play.
    Card play_card(const Card &led_card, Suit trump);
  
   private:
    std::string name;
    std::vector<Card> hand;
   };