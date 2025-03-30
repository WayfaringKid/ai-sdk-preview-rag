#include "Pack.hpp"
#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <cassert>
#include <iostream>
#include <fstream>
using namespace std;

// THESE TEST CASES ARE NOT VERY THOROUGH.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN Pack_tests.cpp

static const int PACK_SIZE = 24;

TEST(test_pack_default_ctor) {
  Pack pack;
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(NINE, SPADES));
}

TEST(test_pack_istream_ctor) {
  const string filename = "pack.in";
  ifstream ifs(filename);
  assert(ifs.is_open());
  Pack pack(ifs);
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(NINE, SPADES));
}

TEST(test_pack_reset) {
  Pack pack;
  pack.deal_one();
  pack.reset();
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(NINE, SPADES));
}

TEST(test_pack_empty) {
  Pack pack;
  for (int i = 0; i < PACK_SIZE - 1; ++i) {
    pack.deal_one();
    ASSERT_FALSE(pack.empty());
  }
  pack.deal_one();
  ASSERT_TRUE(pack.empty());
}

TEST(test_pack_shuffle) {
  Pack pack;
  pack.shuffle();
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(KING, CLUBS));
}

TEST_MAIN()
