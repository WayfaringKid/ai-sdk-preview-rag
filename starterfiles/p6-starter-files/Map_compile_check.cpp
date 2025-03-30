#include <string>
#include <iostream>
#include "Map.hpp"

using namespace std;

class Duck {
public:
  Duck() : wealth(0) {} // tree requires default-constructible
  Duck(int wealth_in) : wealth(wealth_in) {}
  int getWealth() const { return wealth; }

private:
  int wealth;

};

class DuckWealthLess {
public:
  bool operator() (const Duck &d1, const Duck &d2) const {
    return d1.getWealth() < d2.getWealth();
  }

};

ostream &operator<<(ostream &os, const Duck &duck) {
  return os << "Duck: $" << duck.getWealth();
}

int main() {
  cout << "This test doesn't do anything interesting." << endl;
  cout << "It is just here to check for compilation issues." << endl;

  Map<string, int> map;
  const Map<string, int> &const_map = map;

  Map<Duck, string, DuckWealthLess> duck_map;
  const Map<Duck, string, DuckWealthLess> &const_duck_map = duck_map;

  // Dummy variables
  bool b;
  size_t st;

  // Dummy iterators - should be default constructible as end iterator
  Map<string, int>::Iterator it;
  Map<Duck, string, DuckWealthLess>::Iterator duck_it;

  // Big Three
  auto map_copy(const_map);
  auto duck_map_copy(const_duck_map);

  map_copy = const_map;
  duck_map_copy = const_duck_map;

  // destructor tested implicitly at end of function



  // Functions that can be applied to a const map
  b = const_map.empty();
  b = const_duck_map.empty();

  st = const_map.size();
  st = const_duck_map.size();

  it = const_map.begin();
  duck_it = const_duck_map.begin();

  it = const_map.end();
  duck_it = const_duck_map.end();

  it = const_map.find("");
  duck_it = const_duck_map.find(Duck());



  // Functions that can't be called on a const tree
  it = map.insert({"", 1}).first;
  b = map.insert({"quack", 2}).second;
  duck_it = duck_map.insert({Duck(), "Donald"}).first;
  b = duck_map.insert({Duck(1000000), "Scrooge"}).second;

  int &x = map["wat"];
  cout << x << endl;
  string &x2 = duck_map[Duck(-200)];
  cout << x2 << endl;

  // Using iterators

  it = map.begin();
  duck_it = duck_map.begin();

  (*it).second = 200;
  (*duck_it).second = "another duck name";

  ++++it;
  ++++duck_it;

  it = it++;
  duck_it = duck_it++;

  b = map.end() == map.end();
  b = duck_map.end() == duck_map.end();

  b = map.end() != map.end();
  b = duck_map.end() != duck_map.end();

  const auto &const_it = it;
  const auto &const_duck_it = duck_it;

  Map<string, int>::Iterator it_copy(const_it);
  Map<Duck, string, DuckWealthLess>::Iterator duck_it_copy(const_duck_it);

  cout << (*it_copy).first << (*duck_it_copy).second << endl;

  cout << b << st << endl;

}
