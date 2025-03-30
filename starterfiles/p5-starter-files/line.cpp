/*
 * Visualization for testing the text buffer implementation.
 *
 * Note that this is just for testing! This code will only work once
 * you have all the functionality of the TextBuffer class
 * Originally written for 15-122 Principles of Imperative Computation
 * Ported to C++ by Saquib Razak for EECS 280. */

#include <stdio.h>
#include <iostream>
#include <string>
#include "TextBuffer.hpp"

using namespace std;

// REQUIRES: start >= 0 and end <= text.size()
// MODIFIES: cout
// EFFECTS:  Prints out the characters from text in the range [start,
//           end) to cout, replacing newline characters with the \n
//           escape sequence.
void print_range(string text, int start, int end) {
  for (int i = start; i < end; ++i) {
    if (text[i] == '\n') {
      cout << "\\n";
    } else {
      cout << text[i];
    }
  }
}

// MODIFIES: cout
// EFFECTS:  Prints out the data in the given buffer to cout. Newline
//           characters are replaced with the \n escape sequence, and
//           the cursor position is signified by a | character. Also
//           prints out the cursor row and column.
void visualize_buffer(TextBuffer &buffer) {
  string text = buffer.stringify();
  int index = buffer.get_index();
  print_range(text, 0, index);
  cout << '|';
  print_range(text, index, text.size());
  cout << "\t:(" << buffer.get_row() << "," << buffer.get_column()
       << " )\n";
}

// MODIFIES: buffer
// EFFECTS:  Handle a single character from user input. Translates
//           special characters to their respective buffer commands;
//           inserts non-special characters directly.
void process_char(TextBuffer &buffer, char c)  {
  if (c == '<') {
    cout << "left  : ";
    buffer.backward();
  } else if (c == '>') {
    cout << "right : ";
    buffer.forward();
  } else if (c == '^') {
    cout << "up    : ";
    buffer.up();
  } else if (c == '!') {
    cout << "down  : ";
    buffer.down();
  } else if (c == '#') {
    cout << "del   : ";
    buffer.remove();
  } else if (c == '[') {
    cout << "home  : ";
    buffer.move_to_row_start();
  } else if (c == ']') {
    cout << "end   : ";
    buffer.move_to_row_end();
  } else if (c == '@') {
    cout << "enter : ";
    buffer.insert('\n');
  } else {
    cout << "add   : ";
    buffer.insert(c);
  }
}

// MODIFIES: buffer
// EFFECTS:  Handle multiple characters from user input. Translates
//           special characters to their respective buffer commands;
//           inserts non-special characters directly.
void process_string(TextBuffer &buffer, string s) {
  int limit = s.size();
  for (int i = 0; i < limit; i++) {
    process_char(buffer, s[i]);
    visualize_buffer(buffer);
  }
}

// MODIFIES: cin, cout
// EFFECTS:  Runs the LINE visualization, reading user input and
//           responding to it.
int main() {
  TextBuffer buffer;
  cout << "LINE Is Not an Editor -- it is a linear visualization of a"
       << " TextBuffer.\n"
       << "The '<' character mimics a call to backward()\n"
       << "The '>' character mimics a call to forward()\n"
       << "The '#' character mimics a call to remove()\n"
       << "The '^' character mimics a call to up()\n"
       << "The '!' character mimics a call to down()\n"
       << "The '[' character mimics a call to move_to_row_start()\n"
       << "The ']' character mimics a call to move_to_row_end()\n"
       << "The '@' character mimics a call to insert() with a newline\n"
       << "All other characters just mimic insert() with that character\n\n"
       << "Give initial input (empty line quits):"
       << endl;

  string s;
  while (getline(cin, s) && s != "") {
    cout << "STARTING\nstart : ";
    visualize_buffer(buffer);
    process_string(buffer, s);
    cout << "\n";

    cout << "Done. More input? (empty line quits):" << endl;
  }

  cout << "Goodbye." << endl;
}
