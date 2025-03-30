#ifndef MATRIX_TEST_HELPERS_HPP
#define MATRIX_TEST_HELPERS_HPP

/* Matrix_test_helpers.hpp
 * Originally written by James Juett at the University of Michigan
 * for the image processing project in EECS 280, Winter 2016.
 */

#include "Matrix.hpp"

// REQUIRES: mat1 points to a valid Matrix
//           mat2 points to a valid Matrix
// EFFECTS:  Returns true if mat1 and mat2 are the same size and
//           contain exactly the same elements. Returns false otherwise.
bool Matrix_equal(const Matrix* mat1, const Matrix* mat2);

#endif // MATRIX_TEST_HELPERS_HPP
