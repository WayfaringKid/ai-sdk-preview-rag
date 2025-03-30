#include <sstream>
#include "unit_test_framework.hpp"
#include "Card.hpp"

using namespace std;

// THESE TEST CASES WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN Card_tests.cpp

TEST(test_card_suit_and_rank) {
    Card two_spades = Card();
    ASSERT_EQUAL(two_spades.get_rank(), TWO);
    ASSERT_EQUAL(two_spades.get_suit(), SPADES);

    Card three_spades = Card(THREE, SPADES);
    ASSERT_EQUAL(three_spades.get_rank(), THREE);
    ASSERT_EQUAL(three_spades.get_suit(), SPADES);
    ASSERT_EQUAL(three_spades.get_suit(CLUBS), SPADES);
}

TEST(test_card_type) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades.is_face_or_ace());
    ASSERT_FALSE(three_spades.is_right_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_left_bower(CLUBS));
    ASSERT_FALSE(three_spades.is_trump(CLUBS));
}

TEST(test_card_self_comparison) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(three_spades < three_spades);
    ASSERT_TRUE(three_spades <= three_spades);
    ASSERT_FALSE(three_spades > three_spades);
    ASSERT_TRUE(three_spades >= three_spades);
    ASSERT_TRUE(three_spades == three_spades);
    ASSERT_FALSE(three_spades != three_spades);
}

TEST(test_Suit_next) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
}

TEST(test_Card_less_self) {
    Card three_spades = Card(THREE, SPADES);
    ASSERT_FALSE(Card_less(three_spades, three_spades, CLUBS));
    ASSERT_FALSE(Card_less(three_spades, three_spades, three_spades,
                           CLUBS));
}

TEST(test_card_insertion) {
    Card three_spades = Card(THREE, SPADES);
    ostringstream oss;
    oss << three_spades;
    ASSERT_EQUAL(oss.str(), "Three of Spades");
}

TEST(test_card_extraction) {
    istringstream iss("Three of Spades");
    Card c;
    iss >> c;
    ASSERT_EQUAL(c, Card(THREE, SPADES));
}

TEST_MAIN()
