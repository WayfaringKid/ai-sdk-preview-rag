#ifndef TEXTBUFFER_HPP
#define TEXTBUFFER_HPP
/* TextBuffer.hpp
 *
 * Character buffer that has a cursor and supports moving the cursor,
 * insertion at the cursor, deletion at the cursor, and other
 * operations useful to a text editor.
 *
 * EECS 280 List/Editor Project
 */

#include <list>
#include <string>
// Uncomment the following line to use your List implementation
// #include "List.hpp"

class TextBuffer {
  // Comment out the following two lines and uncomment the two below
  // to use your List implementation
  using CharList = std::list<char>;
  using Iterator = std::list<char>::iterator;
  // using CharList = List<char>;
  // using Iterator = List<char>::Iterator;

private:
  CharList data;           // linked list that contains the characters
  Iterator cursor;         // iterator to current element in the list
  int row;                 // current row
  int column;              // current column
  int index;               // current index

  // INVARIANT (cursor iterator):
  //   `cursor` points at an actual character in the list, or is
  //   at the past-the-end position (i.e. an end() iterator).

  // INVARIANT (row/column):
  //   `row` and `column` are the row and column numbers of the
  //   character the cursor is pointing at, determined by the
  //   placement of '\n' newline characters in the buffer.
  //   row is 1-indexed, whereas column is 0-indexed.

  // INVARIANT: (index)
  //   `index` is the 0-based index of the character the cursor is
  //   pointing at, or equal to the total number of characters in the
  //   list if the cursor is at the past-the-end position.
  //   0 <= index <= data.size()

  // The above invariants are established by the constructor and are
  // assumed to hold at the start of any member function call (i.e.
  // they are implicit conditions in the REQUIRES clause). Each function
  // must maintain these invariants (if they are temporarily broken, the
  // function must restore them before it returns).

public:
  //EFFECTS: Creates an empty text buffer. Its cursor is at the past-the-end
  //         position, with row 1, column 0, and index 0.
  TextBuffer();

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor one position forward and returns true,
  //          unless the cursor is already at the past-the-end position,
  //          in which case this does nothing and returns false.
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  bool forward();

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor one position backward and returns true,
  //          unless the cursor is already at the first character in
  //          the buffer, in which case this does nothing and returns false.
  //HINT:     Implement and use the private compute_column() member
  //          function to compute the column when moving left from the
  //          beginning of a line to the end of the previous one.
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  bool backward();

  //MODIFIES: *this
  //EFFECTS:  Inserts a character in the buffer before the cursor position.
  //          If the cursor is at the past-the-end position, this means the
  //          inserted character is the last character in the buffer.
  //          The cursor remains in the same place as before the insertion.
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  void insert(char c);

  //MODIFIES: *this
  //EFFECTS:  Removes the character from the buffer that is at the cursor and
  //          returns true, unless the cursor is at the past-the-end position,
  //          in which case this does nothing and returns false.
  //          The cursor will now point to the character that was after the
  //          deleted character, or the past-the-end position if the deleted
  //          character was the last one in the buffer.
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  bool remove();

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor to the start of the current row (column 0).
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  void move_to_row_start();

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor to the end of the current row (the
  //          newline character that ends the row, or the past-the-end
  //          position if the row is the last one in the buffer).
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  void move_to_row_end();

  //REQUIRES: new_column >= 0
  //MODIFIES: *this
  //EFFECTS:  Moves the cursor to the given column in the current row,
  //          if it exists. If the row does not have that many columns,
  //          moves to the end of the row (the newline character that
  //          ends the row, or the past-the-end position if the row is
  //          the last one in the buffer).
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  void move_to_column(int new_column);

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor to the previous row, retaining the
  //          current column if possible. If the previous row is
  //          shorter than the current column, moves to the end of the
  //          previous row (the newline character that ends the row).
  //          Does nothing if the cursor is already in the first row.
  //          Returns true if the position changed, or false if it did
  //          not (i.e. if the cursor was already in the first row).
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  bool up();

  //MODIFIES: *this
  //EFFECTS:  Moves the cursor to the next row, retaining the current
  //          column if possible. If the next row is shorter than the
  //          current column, moves to the end of the next row (the
  //          newline character that ends the row, or the past-the-end
  //          position if the row is the last one in the buffer). Does
  //          nothing if the cursor is already in the last row.
  //          Returns true if the position changed, or false if it did
  //          not (i.e. if the cursor was already in the last row).
  //NOTE:     Your implementation must update the row, column, and index
  //          if appropriate to maintain all invariants.
  bool down();

  //EFFECTS:  Returns whether the cursor is at the past-the-end position.
  bool is_at_end() const;

  //REQUIRES: the cursor is not at the past-the-end position
  //EFFECTS:  Returns the character at the current cursor
  char data_at_cursor() const;

  //EFFECTS:  Returns the row of the character at the current cursor.
  int get_row() const;

  //EFFECTS:  Returns the column of the character at the current cursor.
  int get_column() const;

  //EFFECTS:  Returns the index of the character at the current cursor
  //          with respect to the entire contents. If the cursor is at
  //          the past-the-end position, returns size() as the index.
  int get_index() const;

  //EFFECTS:  Returns the number of characters in the buffer.
  int size() const;

  //EFFECTS:  Returns the contents of the text buffer as a string.
  //HINT: Implement this using the string constructor that takes a
  //      begin and end iterator. You may use this implementation:
  //        return std::string(data.begin(), data.end());
  std::string stringify() const;

private:
  //EFFECTS: Computes the column of the cursor within the current row.
  //NOTE: This does not assume that the "column" member variable has
  //      a correct value (i.e. the row/column INVARIANT can be broken).
  int compute_column() const;
};

#endif // TEXTBUFFER_HPP
