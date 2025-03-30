#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include "Image.hpp"
#include "processing.hpp"
#include "Matrix_test_helpers.hpp"
#include "Image_test_helpers.hpp"
#include "unit_test_framework.hpp"

using namespace std;

static void test_all(const string &prefix, const vector<int> &sizes, int num_sizes);
static void test_rotate(const Image *img, const string &prefix);
static void test_energy(const Matrix *energy_mat, const string &prefix);
static void test_cost(const Matrix *cost_mat, const string &prefix);
static void test_find_seam(const vector<int> &seam, const string &prefix);
static void test_remove_seam(const Image* img, const vector<int> &seam,
                             const string &prefix);
static void test_seam_carve(const Image *img, const string &prefix,
                            int new_width, int new_height);

static void load_matrix(Matrix* mat, const string &filename);
static void write_matrix(const Matrix* mat, const string &filename);
static void load_image(Image* img, const string &filename);
static void write_image(const Image* img, const string &filename);
static void load_seam(vector<int> &seam, const string &filename);
static void write_seam(const vector<int> &seam, const string &filename);

const string OUT_PPM_EXT = ".out.ppm";
const string OUT_TXT_EXT = ".out.txt";

static void test_all(const string &prefix, const vector<int> &sizes, int num_sizes) {
  Image img;
  load_image(&img, prefix + ".ppm");

  // Test rotate
  test_rotate(&img, prefix);

  // Test energy
  Matrix energy;
  compute_energy_matrix(&img, &energy);
  test_energy(&energy, prefix);

  // Test cost
  Matrix  cost;
  compute_vertical_cost_matrix(&energy, &cost);
  test_cost(&cost, prefix);

  // Test find seam
  vector<int> seam = find_minimal_vertical_seam(&cost);
  test_find_seam(seam, prefix);

  // Test remove seam
  test_remove_seam(&img, seam, prefix);

  // Test full seam carving algorithm on various sizes
  for(int i = 0; i < num_sizes; ++i) {
    test_seam_carve(&img, prefix, sizes[2*i], sizes[2*i + 1]);
  }

  cout << prefix << " tests PASS" << endl;
}

static void test_rotate(const Image *img, const string &prefix) {
  Image rotated_img;

  // Test left rotation
  cout << "Testing " << prefix << " rotate left..." << flush;
  rotated_img = *img;
  rotate_left(&rotated_img);
  write_image(&rotated_img, prefix + "_left" + OUT_PPM_EXT);

  Image rotated_img_correct_left;
  load_image(&rotated_img_correct_left, prefix + "_left.correct.ppm");
  ASSERT_TRUE(Image_equal(&rotated_img, &rotated_img_correct_left));
  cout << "PASS" << endl;

  // Test right rotation
  cout << "Testing " << prefix << " rotate right...";
  rotated_img = *img;
  rotate_right(&rotated_img);
  write_image(&rotated_img, prefix + "_right" + OUT_PPM_EXT);

  Image rotated_img_correct_right;
  load_image(&rotated_img_correct_right, prefix + "_right.correct.ppm");
  ASSERT_TRUE(Image_equal(&rotated_img, &rotated_img_correct_right));
  cout << "PASS" << endl;
}

static void test_energy(const Matrix *energy_mat, const string &prefix) {
  cout << "Testing " << prefix << " energy..." << flush;

  write_matrix(energy_mat, prefix + "_energy" + OUT_TXT_EXT);

  Matrix energy_mat_correct;
  load_matrix(&energy_mat_correct, prefix + "_energy_correct.txt");

  ASSERT_TRUE(Matrix_equal(energy_mat, &energy_mat_correct));
  cout << "PASS" << endl;
}


static void test_cost(const Matrix *cost_mat, const string &prefix) {
  cout << "Testing " << prefix << " cost..." << flush;

  write_matrix(cost_mat, prefix + "_cost" + OUT_TXT_EXT);

  Matrix cost_mat_correct;
  load_matrix(&cost_mat_correct, prefix + "_cost_correct.txt");

  ASSERT_TRUE(Matrix_equal(cost_mat, &cost_mat_correct));
  cout << "PASS" << endl;
}

static void test_find_seam(const vector<int> &seam, const string &prefix) {
  cout << "Testing " << prefix << " find seam..." << flush;
  write_seam(seam, prefix + "_seam" + OUT_TXT_EXT);

  vector<int> seam_correct;
  load_seam(seam_correct, prefix + "_seam_correct.txt");

  ASSERT_SEQUENCE_EQUAL(seam, seam_correct);
  cout << "PASS" << endl;
}

static void test_remove_seam(const Image* img, const vector<int> &seam,
                             const string &prefix) {
  cout << "Testing " << prefix << " remove seam..." << flush;

  Image removed_img = *img;
  remove_vertical_seam(&removed_img, seam);
  write_image(&removed_img, prefix + "_removed" + OUT_PPM_EXT);

  Image removed_img_correct;
  load_image(&removed_img_correct, prefix + "_removed.correct.ppm");

  ASSERT_TRUE(Image_equal(&removed_img, &removed_img_correct));
  cout << "PASS" << endl;
}

static void test_seam_carve(const Image *img, const string &prefix,
                            int new_width, int new_height) {
  cout << "Testing " << prefix << " seam carve ";
  cout << new_width << "x" << new_height << "..." << flush;
  Image carved_img = *img;
  seam_carve(&carved_img, new_width, new_height);
  write_image(&carved_img,
              prefix + "_" + to_string(new_width) +
              "x" + to_string(new_height) + OUT_PPM_EXT);

  Image carved_img_correct;
  load_image(&carved_img_correct,
             prefix + "_" + to_string(new_width) +
             "x" + to_string(new_height) + ".correct.ppm");

  ASSERT_TRUE(Image_equal(&carved_img, &carved_img_correct));
  cout << "PASS" << endl;
}

static void load_matrix(Matrix* mat, const string &filename) {
  ifstream fin(filename);
  if (!fin.is_open()) {
    cout << "Unable to open " << filename << endl;
    exit(EXIT_FAILURE);
  }

  int width, height;
  fin >> width >> height;
  Matrix_init(mat, width, height);

  for (int r = 0; r < height; ++r) {
    for (int c = 0; c < width; ++c) {
      fin >> *Matrix_at(mat, r, c);
    }
  }
}

static void write_matrix(const Matrix* mat, const string &filename) {
  ofstream fout(filename);
  Matrix_print(mat, fout);
}

static void load_image(Image* img, const string &filename) {
  ifstream fin(filename);
  if (!fin.is_open()) {
    cout << "Unable to open " << filename << endl;
    exit(EXIT_FAILURE);
  }

  Image_init(img, fin);
}

static void write_image(const Image* img, const string &filename) {
  ofstream fout(filename);
  Image_print(img, fout);
}

static void load_seam(vector<int> &seam, const string &filename) {
  ifstream fin(filename);
  if (!fin.is_open()) {
    cout << "Unable to open " << filename << endl;
    exit(EXIT_FAILURE);
  }

  int column;
  while (fin >> column) {
    seam.push_back(column);
  }
}

static void write_seam(const vector<int> &seam, const string &filename) {
  ofstream fout(filename);
  for (int value : seam) {
    fout << value << endl;
  }
}

TEST(test1_dog) {
  vector<int> dog_sizes = {4, 5};
  test_all("dog", dog_sizes, 1);
}

TEST(test2_crabster) {
  vector<int> crabster_sizes = {50, 45, 70, 35};
  test_all("crabster", crabster_sizes, 2);
}

TEST(test3_horses) {
  vector<int> horses_sizes = {300, 382, 400, 250};
  test_all("horses", horses_sizes, 2);
}

TEST_MAIN()
