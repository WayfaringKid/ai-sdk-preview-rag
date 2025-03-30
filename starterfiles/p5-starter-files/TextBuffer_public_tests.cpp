#include <iostream>
#include <string>
#include "unit_test_framework.hpp"
#include "TextBuffer.hpp"

using namespace std;

TEST(test_new_editor) {
  TextBuffer buffer;
  ASSERT_TRUE(buffer.is_at_end());
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(),0);
  ASSERT_EQUAL(buffer.stringify(), "");
  ASSERT_EQUAL(buffer.get_index(), 0);
}

TEST(test_insert) {
  TextBuffer buffer;
  buffer.insert('A');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'A');
}

TEST(test_insert_row) {
  TextBuffer buffer;
  ASSERT_EQUAL(buffer.get_row(), 1);
  buffer.insert('A');
  ASSERT_EQUAL(buffer.get_row(), 1);
  buffer.insert('b');
}

TEST(test_insert_col) {
  TextBuffer buffer;
  ASSERT_EQUAL(buffer.get_column(), 0);
  buffer.insert('A');
  ASSERT_EQUAL(buffer.get_column(), 1);
  buffer.insert('b');
}

TEST(test_insert_multiple_chars) {
  TextBuffer buffer;
  buffer.insert('a');
  buffer.insert('b');
  buffer.insert('C');
  buffer.insert('?');
  buffer.insert('+');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), '+');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), '?');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'C');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'b');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'a');
}

TEST(test_insert_newline) {
  TextBuffer buffer;
  buffer.insert('a');
  buffer.insert('b');
  buffer.insert('C');
  buffer.insert('\n');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), '\n');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'C');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'b');
  buffer.backward();
  ASSERT_EQUAL(buffer.data_at_cursor(), 'a');
}

TEST(test_remove) {
  TextBuffer buffer;
  buffer.insert('a');
  buffer.insert('b');
  buffer.insert('C');
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 3);
  ASSERT_EQUAL(buffer.stringify(), "abC");
  buffer.backward();
  buffer.remove();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 2);
  ASSERT_EQUAL(buffer.stringify(), "ab");
}

TEST(test_move_to_row_start) {
  TextBuffer buffer;
  buffer.insert('a');
  buffer.insert('b');
  buffer.insert('C');
  buffer.move_to_row_start();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 0);
}

TEST(test_move_to_row_end) {
  TextBuffer buffer;
  buffer.insert('a');
  buffer.insert('b');
  buffer.insert('C');
  buffer.move_to_row_start();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 0);
  buffer.move_to_row_end();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 3);
}

TEST(test_move_to_column) {
  TextBuffer buffer;
  for (int i = 0; i < 62; i++) {
    buffer.insert((char)('0'+i));
  }
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 62);
  for (int i = 0; i < 62; i += 2) {
    buffer.move_to_column(i);
    ASSERT_EQUAL(buffer.get_row(), 1);
    ASSERT_EQUAL(buffer.get_column(), i);
  }
  for (int i = 1; i < 62; i += 2) {
    buffer.move_to_column(i);
    ASSERT_EQUAL(buffer.get_row(), 1);
    ASSERT_EQUAL(buffer.get_column(), i);
  }
  buffer.forward();
  buffer.insert('\n');
  for (int i = 0; i < 62; i++) {
    buffer.insert((char)('0'+i));
  }
  for (int i = 0; i <62; i++) {
    buffer.move_to_column(i);
    ASSERT_EQUAL(buffer.get_row(), 2);
    ASSERT_EQUAL(buffer.get_column(), i);
  }
}

TEST(test_up) {
  TextBuffer buffer;
  for (int i = 0; i < 5; i++) {
    buffer.insert((char)('0'+i));
  }
  buffer.insert('\n');
  for (int i = 0; i < 10; i++) {
    buffer.insert((char)('0'+i));
  }
  ASSERT_EQUAL(buffer.get_row(), 2);
  ASSERT_EQUAL(buffer.get_column(), 10);
  buffer.up();
  ASSERT_EQUAL(buffer.get_row(), 1);
  ASSERT_EQUAL(buffer.get_column(), 5);
}

TEST(test_down) {
  TextBuffer buffer;
  for (int i = 0; i < 10; i++) {
    buffer.insert((char)('0'+i));
  }
  buffer.insert('\n');
  for (int i = 0; i < 5; i++) {
    buffer.insert((char)('0'+i));
  }
  buffer.move_to_row_start();
  buffer.backward();
  buffer.down();
  ASSERT_EQUAL(buffer.get_row(), 2);
  ASSERT_EQUAL(buffer.get_column(), 5);
}

TEST(test_size) {
  TextBuffer buffer;
  ASSERT_EQUAL(buffer.size(), 0);
  buffer.insert('a');
  ASSERT_EQUAL(buffer.size(), 1);
}

TEST(test_is_at_end) {
  TextBuffer buffer;
  ASSERT_TRUE(buffer.is_at_end());
  buffer.insert('e');
  ASSERT_TRUE(buffer.is_at_end());
}

TEST(test_stringify) {
  TextBuffer buffer;
  buffer.insert('e');
  buffer.insert('f');
  buffer.insert('g');
  buffer.insert('\n');
  buffer.insert('h');
  buffer.insert('i');
  buffer.insert('j');
  buffer.insert('k');
  buffer.insert('\n');
  ASSERT_EQUAL(buffer.stringify(), "efg\nhijk\n");
  ASSERT_EQUAL(buffer.get_index(),9);
}

TEST_MAIN()
