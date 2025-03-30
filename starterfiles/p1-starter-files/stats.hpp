#ifndef STATS_HPP
#define STATS_HPP
/* stats.hpp
 *
 * Simple statistics library
 *
 * EECS 280 Statistics Project
 *
 */

#include <vector>

//NOTE: no "using namespace std;" in a .hpp file!  That's why we use "std::vector
//instead of "vector" later on.  You can add "using namespace std;" to your
//stats.cpp if you want to.

//EFFECTS: returns the count (sample size) of the numbers in v
int count(std::vector<double> v);

//REQUIRES: v is not empty
//EFFECTS: returns the sum of the numbers in v
double sum(std::vector<double> v);

//REQUIRES: v is not empty
//EFFECTS: returns the arithmetic mean of the numbers in v
//  http://en.wikipedia.org/wiki/Arithmetic_mean
double mean(std::vector<double> v);

//REQUIRES: v is not empty
//EFFECTS: returns the median of the numbers in v
//  https://en.wikipedia.org/wiki/Median#Finite_data_set_of_numbers
double median(std::vector<double> v);

//REQUIRES: v is not empty
//EFFECTS: returns the min number in v
double min(std::vector<double> v);

//REQUIRES: v is not empty
//EFFECTS: returns the max number in v
double max(std::vector<double> v);

//REQUIRES: v contains at least 2 elements
//EFFECTS: returns the corrected sample standard deviation of the numbers in v
//  http://en.wikipedia.org/wiki/Standard_deviation#Corrected_sample_standard_deviation
double stdev(std::vector<double> v);

//REQUIRES: v is not empty
//          p is between 0 and 1, inclusive
//EFFECTS: returns the percentile p of the numbers in v like Microsoft Excel.
//  Refer to the project spec for the formula to use.
//  NOTE: the definition in the spec uses indexing from 1.  You will need to
//  adapt it to use indexing from 0.
double percentile(std::vector<double> v, double p);

//REQUIRES: v and criteria have the same number of elements
//EFFECTS: returns a new, filtered version of v containing the elements (and
//         only those elements) at v[x] where criteria[x] is equal to target,
//         in the same order as they originally appear in v.
//         Note: For this function, compare values to the criteria using ==.
std::vector<double> filter(std::vector<double> v,
                           std::vector<double> criteria,
                           double target);

#endif
