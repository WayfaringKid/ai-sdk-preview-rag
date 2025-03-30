#include "Matrix.hpp"
#include "Matrix_test_helpers.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using std::ostringstream;


// This is the public Matrix test for which the autograder gives feedback.
// It only tests VERY FEW of the expected behaviors of the Matrix module.
// It will only really tell you if your code compiles and you remembered to
// write the functions. It is not to be trusted. It tells the truth, but not
// the whole truth. It might put you in a blender. You get the point.
// You must write your own comprehensive unit tests in Matrix_tests.cpp!


TEST(test_matrix_basic) {
  Matrix mat;
  Matrix_init(&mat, 5, 5);

  ASSERT_EQUAL(Matrix_width(&mat), 5);
  ASSERT_EQUAL(Matrix_height(&mat), 5);

  Matrix_fill(&mat, 0);

  int *ptr = Matrix_at(&mat, 2, 3);
  ASSERT_EQUAL(*ptr, 0);
  *ptr = 42;

  const int *cptr = Matrix_at(&mat, 2, 3);
  ASSERT_EQUAL(*cptr, 42);

  Matrix_fill_border(&mat, 2);
  ASSERT_EQUAL(*Matrix_at(&mat, 0, 0), 2);

  ASSERT_EQUAL(Matrix_max(&mat), 42);
}

TEST(test_matrix_print) {
  Matrix mat;
  Matrix_init(&mat, 1, 1);

  *Matrix_at(&mat, 0, 0) = 42;
  ostringstream expected;
  expected << "1 1\n"
           << "42 \n";
  ostringstream actual;
  Matrix_print(&mat, actual);
  ASSERT_EQUAL(expected.str(), actual.str());
}

TEST_MAIN()
