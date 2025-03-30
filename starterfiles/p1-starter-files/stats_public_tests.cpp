// THIS TEST CASE WILL ONLY TELL YOU IF YOUR CODE COMPILES.
// YOU NEED TO WRITE YOUR OWN COMPREHENSIVE TEST CASES IN stats_tests.cpp

#include "stats.hpp"
#include <iostream>
#include <cassert>
#include <cmath>
#include <vector>
using namespace std;

// Precision for floating point comparison
const double epsilon = 0.00001;

static bool almost_equal(double x, double y) {
  return abs(x - y) < epsilon;
}

int main() {
  vector<double> data;
  data.push_back(1);
  data.push_back(2);
  data.push_back(3);
  assert(count(data) == 3);
  assert(sum(data) == 6);
  assert(almost_equal(mean(data), 2));
  assert(median(data) == 2);
  assert(min(data) == 1);
  assert(max(data) == 3);
  assert(almost_equal(stdev(data), 1));
  assert(almost_equal(percentile(data, 0.5), 2));

  vector<double> criteria = {1, 0, 1};
  assert(filter(data, criteria, 1) == vector<double>({1, 3}));

  cout << "PASS!" << endl;
  return 0;
}

