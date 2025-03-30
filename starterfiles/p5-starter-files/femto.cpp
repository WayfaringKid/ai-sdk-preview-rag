/**
 * FEMTO: FEMTO Editor for Manipulating Text Ostensibly
 *
 * Based on E0 from Carnegie Mellon University
 *
 * E0 Authors: William Lovas and Robert Simmons (CMU)
 * E0 C++ Port: Saquib Razak (University of Michigan)
 * FEMTO Author: Amir Kamil (University of Michigan)
 */

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <deque>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ncurses.h>
#include "TextBuffer.hpp"

#ifndef FEMTO_INPUT_MODE // default to terminal input mode
#  define FEMTO_INPUT_MODE TERMINAL
#endif

class FemtoEditor {
public:
  static constexpr const char *version = "2.80";

  enum InputMode {
    TERMINAL, // terminal interprets control keys
    RAW       // control keys are passed uninterpreted to FEMTO
  };

  // Initialize the editor with the given file and input mode.
  // Starts the interaction.
  FemtoEditor(std::string filename_in, InputMode input_mode_in)
    : baseline(1), cursor_row(1), filename(filename_in),
      modified(false), percentage(0), status("initial"),
      input_mode(input_mode_in) {
    if (!filename.empty()) {
      read_file();
    }
    setup_windows();
    interact();
  }

  // disable copying
  FemtoEditor(const FemtoEditor&) = delete;
  FemtoEditor& operator=(const FemtoEditor&) = delete;

  // Shut down ncurses.
  ~FemtoEditor() {
    curs_set(visibility); // restore prior visibility
    endwin();
  }

private:
  using clock_t = std::chrono::steady_clock;
  static constexpr double MESSAGE_TIMEOUT = 5; // time in seconds
  static const std::size_t MAX_SHORT_STRING_LENGTH = 20;

  struct KeyBindings {
    static const int EXIT1 = 24; // ^X
    static const int EXIT2 = 17; // ^Q
    static const int SAVE1 = 1; // ^A
    static const int SAVE2 = 19; // ^S
    static const int SAVE3 = 15; // ^O - pico/nano binding
    static const int REFRESH = 12; // ^L
    static const int FIND1 = 6; // ^F
    static const int FIND2 = 23; // ^W - pico/nano binding
    static const int GOTO = 7; // ^G
    static const int CUT = 11; // ^K
    static const int UNCUT = 21; // ^U
    static const int CANCEL = 14; // ^N
    static const int INTERRUPT = 3; // ^C
    static const int ESCAPE = 27;
    static const int DELETE = 4; // ^D
    static const int BACKSPACE2 = 127;
    static const int BACKSPACE3 = '\b';
    static const int NEWLINE = '\n';
    static const int CARRIAGE_RETURN = '\r';
    static const int WORD_LEFT1 = 542; // ^left on MacOS
    static const int WORD_LEFT2 = 546; // ^left on Windows
    static const int WORD_LEFT3 = 547; // ^left on MacOS (raw)
    static const int WORD_RIGHT1 = 557; // ^right on MacOS
    static const int WORD_RIGHT2 = 561; // ^right on Windows
    static const int WORD_RIGHT3 = 562; // ^right on MacOS (raw)
    static const int PAGE_DOWN = 526; // ^down on Windows
    static const int PAGE_UP = 567; // ^up on Windows
    static const int IGNORE1 = -1; // sent when mucking with the window
    static const int IGNORE2 = 410; // sent when mucking with the window
    static const int MIN_CHAR = 1;
    static const int MAX_CHAR = 126;

    static constexpr bool is_exit(int c) {
      return c == EXIT1 || c == EXIT2 || c == INTERRUPT;
    }
    static constexpr bool is_save(int c) {
      return c == SAVE1 || c == SAVE2 || c == SAVE3;
    }
    static constexpr bool is_refresh(int c) {
      return c == REFRESH;
    }
    static constexpr bool is_goto(int c) {
      return c == GOTO;
    }
    static constexpr bool is_find(int c) {
      return c == FIND1 || c == FIND2;
    }
    static constexpr bool is_cut(int c) {
      return c == CUT;
    }
    static constexpr bool is_uncut(int c) {
      return c == UNCUT;
    }
    static constexpr bool is_cancel(int c) {
      return c == CANCEL || c == INTERRUPT || c == ESCAPE;
    }
    static constexpr bool is_up(int c) {
      return c == KEY_UP; // ncurses constant
    }
    static constexpr bool is_down(int c) {
      return c == KEY_DOWN; // ncurses constant
    }
    static constexpr bool is_pageup(int c) {
      return c == KEY_PPAGE /* ncurses constant */ || c == PAGE_UP;
    }
    static constexpr bool is_pagedown(int c) {
      return c == KEY_NPAGE /* ncurses constant */ || c == PAGE_DOWN;
    }
    static constexpr bool is_enter(int c) {
      return c == KEY_ENTER // ncurses constant
        || c == NEWLINE || c == CARRIAGE_RETURN;
    }
    static constexpr bool is_backspace(int c) {
      return c == KEY_BACKSPACE // ncurses constant
        || c == BACKSPACE2 || c == BACKSPACE3;
    }
    static constexpr bool is_delete(int c) {
      return c == KEY_DC /* ncurses constant */ || c == DELETE;
    }
    static constexpr bool is_left(int c) {
      return c == KEY_LEFT; // ncurses constant
    }
    static constexpr bool is_right(int c) {
      return c == KEY_RIGHT; // ncurses constant
    }
    static constexpr bool is_home(int c) {
      return c == KEY_HOME; // ncurses constant
    }
    static constexpr bool is_end(int c) {
      return c == KEY_END; // ncurses constant
    }
    static constexpr bool is_word_left(int c) {
      return c == WORD_LEFT1 || c == WORD_LEFT2 || c == WORD_LEFT3;
    }
    static constexpr bool is_word_right(int c) {
      return c == WORD_RIGHT1 || c == WORD_RIGHT2 || c == WORD_RIGHT3;
    }
    static constexpr bool is_ignore(int c) {
      return c == IGNORE1 || c == IGNORE2;
    }
  };

  struct Buffer {
    TextBuffer text;
    WINDOW *window;
    bool reverse;        // whether A_REVERSE is set on the window
    std::string long_prefix; // prefix string before placing characters
    std::string short_prefix; // shorter prefix for narrow windows
    int view_row;        // cursor row
    int view_column;     // first text column to show in cursor row
    char left_overflow_marker;
    char right_overflow_marker;

    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // Set prefixes to the given values.
    void set_prefix(const std::string &long_prefix_in,
                    const std::string &short_prefix_in) {
      long_prefix = long_prefix_in;
      short_prefix = short_prefix_in;
    }

    // Get appropriate prefix given window size.
    std::string& get_prefix() {
      // compare window size to tab + overflow markers
      if (static_cast<int>(long_prefix.size()) > getmaxx(window) - 10) {
        return short_prefix;
      } else {
        return long_prefix;
      }
    }

    // Compute the new view column based on the cursor and move the
    // text buffer to that position.
    // REQUIRES: femto.text.get_row() == cursor_row
    void recompute_view_column(FemtoEditor &femto, int cursor_row,
                               int cursor_column) {
      if (cursor_row != view_row || cursor_column < view_column) {
        view_row = cursor_row;
        view_column = 0; // recompute from the left
      }
      text.move_to_column(view_column);
      std::string &prefix = get_prefix();
      int window_width = getmaxx(window) - prefix.size() - 1;
      // column in the window where current character will be written
      int window_column = (view_column != 0 ? 1 : 0);
      for (; text.get_column() <= cursor_column
             && !text.is_at_end() // handle end of buffer
             && text.get_row() == cursor_row; // handle end of row
           text.forward()) {
        char c = text.data_at_cursor();
        window_column += femto.display_width(window_column, c);
        if (window_column > window_width && c != '\n') {
          // slide view column to the right
          window_width = getmaxx(window) - prefix.size() -  1;
          int remaining = window_width - 1; //right overflow marker
          // max of current char + 4 chars to the left of current
          for (int i = 0, ichar = text.data_at_cursor();
               i < 5 && remaining - femto.display_width(0, ichar) >= 0;
               remaining -= femto.display_width(0, ichar), ++i,
                 text.backward(), ichar = text.data_at_cursor());
          text.forward(); // we went back too far by one character
          view_column = text.get_column();
          // set window column after current character
          window_column =
            1 + femto.display_width(1, text.data_at_cursor());
        }
      }
      if (text.get_row() != cursor_row) { // we moved to the next row
        text.up();
      }
      text.move_to_column(view_column);
    }
  };

  Buffer editbuffer = {{}, nullptr, false, "", "", 1, 0, '$', '$'};
  Buffer minibuffer = {{}, nullptr, true, "", "", 1, 0, '<', '>'};
  int baseline;         // row of top line in canvas
  int cursor_row;
  std::string filename;
  bool modified;        // whether or not the text has been modified
  int percentage;       // how far in the text the cursor is
  std::string status;   // file modification status
  std::string message;  // info/error message
  std::chrono::time_point<clock_t> message_time;
  std::string cut_value;
  std::string previous_search;
  WINDOW *main_window;
  WINDOW *canvas;
  WINDOW *top_bar;
  WINDOW *overflow_bar;
  WINDOW *message_bar;
  WINDOW *bottom_bar;
  bool input_mode;
  int visibility;
  int char_widths[256]; // onscreen width of each character

  // Initial curses setup.
  // look the other way if you've ever programmed using curses
  void setup_windows(bool highlight_canvas_cursor = true) {
    main_window = initscr();
    if (input_mode == RAW) {
      raw();
    } else {
      cbreak();
    }
    noecho();
    keypad(main_window, true);
    visibility = curs_set(0);

    int ncols = getmaxx(main_window);
    int nlines = getmaxy(main_window);
    int begx = getbegx(main_window);
    int begy = getbegy(main_window);
    canvas = subwin(main_window,
                    nlines - 4  /* lines: 4 for padding/status info */,
                    ncols       /* cols: same as main window */,
                    begy + 2    /* begy: skip top padding/status bar */,
                    begx        /* begx: same as main window */);

    top_bar = subwin(main_window, 1 /* lines */, ncols, begy, begx);
    overflow_bar = subwin(main_window, 1 /* lines */, ncols, begy + 1, begx);
    message_bar = subwin(main_window, 1 /* lines */, ncols, nlines - 2, begx);
    bottom_bar = subwin(main_window, 1 /* lines */, ncols, nlines - 1, begx);
    editbuffer.window = canvas;
    minibuffer.window = bottom_bar;
    compute_character_widths();
    render_all(highlight_canvas_cursor); // render everything
  }

  // Compute onscreen character widths.
  void compute_character_widths() {
    int x, y [[maybe_unused]];
    for (int i = 0; i <= KeyBindings::MAX_CHAR; ++i) {
      werase(canvas);
      wmove(canvas, 0, 0);
      waddch(canvas, i);
      getyx(canvas, y, x);
      char_widths[i] = x;
    }
    // Remaining chars are escaped
    for (unsigned i = static_cast<unsigned>(KeyBindings::MAX_CHAR + 1);
         i < 256; ++i) {
      char buf[11];
      std::snprintf(buf, sizeof(buf) / sizeof(char), "%o", i);
      char_widths[i] = 1 + std::strlen(buf);
    }
    // Special handling for backspace and delete
    char_widths[static_cast<unsigned char>('\b')] = 2;
    char_widths[static_cast<unsigned char>('\x7f')] = 2;
  }

  // Render all windows.
  void render_all(bool highlight_canvas_cursor = true) {
    render_canvas(highlight_canvas_cursor);
    wrefresh(canvas);
    render_top_bars();
    wrefresh(top_bar);
    wrefresh(overflow_bar);
    render_message_bar();
    wrefresh(message_bar);
    render_bottom_bar();
    wrefresh(bottom_bar);
  }

  // Main interaction loop -- respond to user input.
  void interact() {
    do {
      render_all();
    } while (handle_edit_input(getch()));
  }

  // Handle an input character in the edit buffer. Returns whether or
  // not interaction should continue.
  bool handle_edit_input(int c) {
    clear_message();
    if (KeyBindings::is_exit(c)) {
      return !handle_exit();
    } else if (KeyBindings::is_save(c)) {
      set_modified(!handle_save(), true);
    } else if (KeyBindings::is_goto(c)) {
      handle_goto();
    } else if (KeyBindings::is_find(c)) {
      handle_find();
    } else if (KeyBindings::is_cut(c)) {
      return handle_cut();
    } else if (KeyBindings::is_uncut(c)) {
      handle_uncut();
    } else if (KeyBindings::is_up(c)) {
      editbuffer.text.up();
    } else if (KeyBindings::is_down(c)) {
      editbuffer.text.down();
    } else if (KeyBindings::is_pageup(c)) {
      move_page(2 - getmaxy(canvas));
    } else if (KeyBindings::is_pagedown(c)) {
      move_page(getmaxy(canvas) - 2);
    } else {
      set_modified(handle_buffer_input(editbuffer, c,
                                       KeyBindings::MIN_CHAR,
                                       KeyBindings::MAX_CHAR));
    }
    return true;
  }

  // Handle an input character for the given buffer. Returns whether
  // or not the buffer was modified.
  bool handle_buffer_input(Buffer &buffer, int c,
                           int min_char, int max_char,
                           bool highlight_canvas_cursor = true) {
    if (KeyBindings::is_refresh(c)) {
      endwin();
      setup_windows(highlight_canvas_cursor);
    } else if (KeyBindings::is_delete(c)) {
      return buffer.text.remove();
    } else if (KeyBindings::is_backspace(c)) {
      if (buffer.text.backward()) { // make sure there is a character
        buffer.text.remove();
        return true;
      }
    } else if (KeyBindings::is_left(c)) {
      buffer.text.backward();
    } else if (KeyBindings::is_right(c)) {
      buffer.text.forward();
    } else if (KeyBindings::is_home(c)) {
      buffer.text.move_to_row_start();
    } else if (KeyBindings::is_end(c)) {
      buffer.text.move_to_row_end();
    } else if (KeyBindings::is_enter(c)) {
      buffer.text.insert('\n'); // convert to newline
      return true;
    } else if (KeyBindings::is_word_left(c)) {
      // skip over alphanumeric characters
      while (is_alphanumeric(buffer) && buffer.text.backward());
      // skip over non-alphanumeric characters
      while (!is_alphanumeric(buffer) && buffer.text.backward());
    } else if (KeyBindings::is_word_right(c)) {
      // skip over alphanumeric characters
      while (is_alphanumeric(buffer) && buffer.text.forward());
      // skip over non-alphanumeric characters
      while (!is_alphanumeric(buffer) && buffer.text.forward());
    } else if (KeyBindings::is_ignore(c)) { // do nothing
    } else if (min_char <= c && c <= max_char) {
      buffer.text.insert(c);
      return true;
    } else {
      beep(); // reject and alert the user
    }
    return false;
  }

  // Determine whether the cursor is over an alphanumeric character.
  bool is_alphanumeric(Buffer &buffer) {
    return !buffer.text.is_at_end()
      && ((buffer.text.data_at_cursor() >= 'a'
           && buffer.text.data_at_cursor() <= 'z')
          || (buffer.text.data_at_cursor() >= 'A'
              && buffer.text.data_at_cursor() <= 'Z')
          || (buffer.text.data_at_cursor() >= '0'
              && buffer.text.data_at_cursor() <= '9'));
  }

  // Read a line number in the minibuffer and go to that line.
  void handle_goto() {
    minibuffer.set_prefix("Goto line (^N to cancel): ", "Goto: ");
    clear_line(minibuffer);
    get_minibuffer_input('0', '9');
    std::string input = minibuffer.text.stringify();
    if (!input.empty()) {
      try {
        int target = std::stoi(input);
        goto_line(target);
      } catch (const std::out_of_range&) {
        set_message("ERROR: Invalid integer", "Invalid integer");
      }
    } else {
      set_message("Canceled", "Canceled");
    }
  }

  // Read user input in the minibuffer. Return whether input was
  // not canceled.
  bool get_minibuffer_input(int min_char, int max_char) {
    render_canvas(false); // unhighlight cursor
    wrefresh(canvas);
    render_minibuffer();
    wrefresh(bottom_bar);
    int input;
    while (!KeyBindings::is_enter(input = getch())
           && !KeyBindings::is_cancel(input)) {
      handle_buffer_input(minibuffer, input, min_char, max_char, false);
      render_minibuffer();
      wrefresh(bottom_bar);
    }
    if (KeyBindings::is_cancel(input)) {
      clear_line(minibuffer);
      return false;
    }
    return true;
  }

  // Go to the start of a specific line in the text.
  void goto_line(int target) {
    editbuffer.text.move_to_row_start();
    while (editbuffer.text.get_row() < target
           && editbuffer.text.down());
    while (editbuffer.text.get_row() > target
           && editbuffer.text.up());
  }

  // Read a search string in the minibuffer, attempt to find it, and
  // if it is found, go to that location.
  void handle_find() {
    std::string prefix = "Search (^N to cancel)";
    if (!previous_search.empty()) {
      prefix += " [" + previous_search + "]: ";
    } else {
      prefix += ": ";
    }
    minibuffer.set_prefix(prefix, "Search: ");
    clear_line(minibuffer);
    if (!get_minibuffer_input(KeyBindings::MIN_CHAR,
                              KeyBindings::MAX_CHAR)) {
      set_message("Canceled", "Canceled");
      return;
    }
    std::string search = minibuffer.text.stringify();
    if (search.empty() && previous_search.empty()) {
      set_message("Canceled", "Canceled");
      return;
    } else if (search.empty()) {
      search = previous_search;
    }
    previous_search = search;

    // save old position, in case the string is not found
    int old_row = editbuffer.text.get_row();
    int old_column = editbuffer.text.get_column();
    int old_index = editbuffer.text.get_index();
    std::deque<char> search_deque{search.begin(), search.end()};
    editbuffer.text.forward(); // skip current char
    if (!find_helper(editbuffer.text, search_deque)) {
      // try again from beginning
      goto_line(1);
      if (!find_helper(editbuffer.text, search_deque, old_index + 1)) {
        set_message("\"" + shorten_string(search) + "\" not found",
                    "Not found");
        // restore old position
        goto_line(old_row);
        editbuffer.text.move_to_column(old_column);
        return;
      }
    }
    // found string, need to move backwards to its beginning
    for (std::size_t i = 1; i < search.size();
         ++i, editbuffer.text.backward());
    if (editbuffer.text.get_index() <= old_index) {
      set_message("Search wrapped", "Search wrapped");
    } else {
      set_message("", "");
    }
  }

  // Search the given buffer for the string. If max_index is provided,
  // the search ends upon exceeding that position by the size of the
  // search string.
  bool find_helper(TextBuffer &text, const std::deque<char> &search,
                   int max_index = -1) {
    int size = search.size();
    std::deque<char> window;
    for (; !text.is_at_end()
           && (max_index == -1 || text.get_index() < max_index + size);
         text.forward()) {
      if (window.size() == search.size()) {
        window.pop_front();
      }
      window.push_back(text.data_at_cursor());
      if (window == search) {
        return true;
      }
    }
    return false;
  }

  // Clear the contents of the current line and return the contents.
  std::string clear_line(Buffer &buffer) {
    std::string line;
    buffer.text.move_to_row_end();
    if (buffer.text.forward()) {
      line.push_back('\n');
      buffer.text.backward();
      buffer.text.remove();
    }
    bool moved = false;
    while ((moved = buffer.text.backward())
           && buffer.text.data_at_cursor() != '\n') {
      line.push_back(buffer.text.data_at_cursor());
      buffer.text.remove();
    }
    if (moved) {
      buffer.text.forward(); // move back to start of line
    }
    std::reverse(line.begin(), line.end());
    return line;
  }

  // Remove each line as long as CUT is input, saving them in
  // cut_value. Handles the input following the last CUT and returns
  // the result.
  bool handle_cut() {
    std::string new_cut_value;
    int input = KeyBindings::CUT;
    while (KeyBindings::is_cut(input)) {
      std::string line = clear_line(editbuffer);
      if (line.empty()) {
        set_message("Nothing to cut", "Nothing to cut");
      }
      new_cut_value += line;
      set_modified(!new_cut_value.empty()); // update status before
      render_all();                         // re-rendering
      input = getch();
    }
    if (!new_cut_value.empty()) {
      cut_value = new_cut_value;
    }
    return handle_edit_input(input); // handle last user input
  }

  // Insert all characters from cut_value into the buffer.
  void handle_uncut() {
    for (std::size_t i = 0; i < cut_value.size(); ++i) {
      editbuffer.text.insert(cut_value[i]);
    }
    set_modified(!cut_value.empty());
    if (cut_value.empty()) {
      set_message("Nothing to uncut", "Nothing to uncut");
    }
  }

  // Mark buffer as modified if argument is true.
  void set_modified(bool modify = true, bool force_overwrite = false) {
    if (modify) {
      modified = true;
      status = "modified";
    } else if (force_overwrite) {
      modified = modify;
    }
  }

  // Set message state and time.
  void set_message(const std::string &long_message,
                   const std::string &short_message) {
    if (static_cast<int>(long_message.size()) + 4 // [ and ] markers
        > getmaxx(message_bar)) {
      message = short_message;
    } else {
      message = long_message;
    }
    message_time = clock_t::now();
  }

  // Clear message state.
  void clear_message() {
    if (!message.empty() // clear message after timeout has passed
        && static_cast<std::chrono::duration<double>>( // seconds
             clock_t::now() - message_time
           ).count() > MESSAGE_TIMEOUT) {
      message = "";
    }
  }

  // Handle pageup and pagedown events.
  void move_page(int offset) {
    int column = editbuffer.text.get_column();
    // move cursor first
    while (editbuffer.text.get_row() < baseline + offset
           && editbuffer.text.down()); // handle hitting the last row
    while (editbuffer.text.get_row() > baseline + offset
           && editbuffer.text.up()); // handle hitting the first row
    // restore column
    editbuffer.text.move_to_column(column);
    // set new baseline
    if (editbuffer.text.get_row() == 1) {
      baseline = 1;
    } else if (editbuffer.text.get_row() < baseline + offset) {
      // page down at the bottom should not change view
    } else {
      baseline = editbuffer.text.get_row();
    }
  }

  // Return shortened string (e.g. for filenames or messages).
  std::string shorten_string(const std::string &original,
                             std::size_t limit = MAX_SHORT_STRING_LENGTH) {
    std::string result = original;
    if (original.size() > limit) {
      result = "...";
      result += original.substr(original.size() - limit + result.size());
    }
    return result;
  }

  // Handle save dialogue.
  bool handle_save() {
    minibuffer.set_prefix("File to write (^N to cancel): ", "Save as: ");
    clear_line(minibuffer);
    // add existing filename to minibuffer
    for (char ch : filename) {
      minibuffer.text.insert(ch);
    }
    get_minibuffer_input(KeyBindings::MIN_CHAR, KeyBindings::MAX_CHAR);
    std::string file_to_write = minibuffer.text.stringify();
    if (!file_to_write.empty()) {
      return write_file(file_to_write);
    } else {
      set_message("Canceled", "Canceled");
      return !modified;
    }
  }

  // Handle exit confirmation.
  bool handle_exit() {
    if (modified) {
      minibuffer.set_prefix("Save modified buffer before "
                            "exiting? (Y)es/(N)o/(C)ancel ",
                            "Save? (Y/N/C) ");
      clear_line(minibuffer);
      render_canvas(false); // unhighlight cursor
      wrefresh(canvas);
      render_minibuffer();
      wrefresh(bottom_bar);
      while (true) {
        int c = getch();
        if (c == 'y' || c == 'Y') {
          return handle_save();
        } else if (c == 'n' || c == 'N') {
          return true;
        } else if (c == 'c' || c == 'C' || KeyBindings::is_cancel(c)) {
          set_message("Canceled", "Canceled");
          return false;
        } else {
          beep(); // reject and alert the user
        }
      }
    }
    return true;
  }

  // Render the status/overflow bars at the top.
  void render_top_bars() {
    const char *femto_info = " U-M FEMTO ";
    std::string file_info = (modified ? "** " : "-- ");
    file_info +=
      (filename.empty() ? "<new file>" :
       shorten_string(filename, std::min<int>(MAX_SHORT_STRING_LENGTH,
                                              getmaxx(top_bar) - 3)));
    file_info += " ";
    std::string position_info =
      std::to_string(percentage) + "% ("
      + std::to_string(editbuffer.text.get_row()) + ","
      + std::to_string(editbuffer.text.get_column()) + ") ";
    reset_bar(top_bar);
    werase(overflow_bar);
    int info_length = std::strlen(femto_info) + file_info.size()
      + position_info.size() + status.size();
    if (info_length <= getmaxx(top_bar)) {
      waddstr(top_bar, femto_info);
    }
    waddstr(top_bar, file_info.c_str());
    if (info_length - int(std::strlen(femto_info)) <= getmaxx(top_bar)) {
      waddstr(top_bar, position_info.c_str());
      waddstr(top_bar, status.c_str());
    } else {
      reset_bar(overflow_bar);
      waddstr(overflow_bar, position_info.c_str());
      waddstr(overflow_bar, status.c_str());
      wattroff(overflow_bar, A_REVERSE);
    }
    wattroff(top_bar, A_REVERSE);
  }

  // Reset given bar to be blank, with default position and attributes.
  void reset_bar(WINDOW *bar) {
    werase(bar);
    for (int i = getbegx(bar); i < getmaxx(bar); ++i) {
      waddch(bar, ' '|A_STANDOUT);
    }
    wmove(bar, 0, 0);
    wattron(bar, A_REVERSE);
  }

  // Render the message bar near the bottom.
  void render_message_bar() {
    werase(message_bar);
    if (!message.empty()) {
      // center message
      int remaining = getmaxx(message_bar) - message.size() - 4;
      wmove(message_bar, 0, remaining / 2);
      wattron(message_bar, A_REVERSE);
      waddstr(message_bar, "[ ");
      waddstr(message_bar, message.c_str());
      waddstr(message_bar, " ]");
      wattroff(message_bar, A_REVERSE);
    }
  }

  // Render the command/minibuffer bar at the bottom.
  void render_bottom_bar() {
    reset_bar(bottom_bar);
    waddstr(bottom_bar,
            " ^X exit | ^F find | ^A save | ^K cut | ^U uncut"
            " | ^G goto | ^L redraw");
    wattroff(bottom_bar, A_REVERSE);
  }

  // Render the minibuffer at the bottom.
  void render_minibuffer() {
    reset_bar(bottom_bar);
    std::string data = minibuffer.text.stringify();
    int old_column = minibuffer.text.get_column();
    render_row(minibuffer, 1, old_column, true);
    wattroff(bottom_bar, A_REVERSE);
    minibuffer.text.move_to_column(old_column); // restore position
    if (minibuffer.text.is_at_end()) {
      waddch(bottom_bar, ' '|A_NORMAL);
    }
  }

  // Render the canvas with the text data.
  void render_canvas(bool highlight_cursor = true) {
    wmove(canvas, 0, 0);
    werase(canvas);
    rebase();

    // save current position
    int old_row = editbuffer.text.get_row();
    int old_column = editbuffer.text.get_column();
    percentage = editbuffer.text.is_at_end() ? 100 :
      100LL * editbuffer.text.get_index() / editbuffer.text.size();
    // display as many rows as fit on the canvas, starting at baseline
    for (int row = baseline; row < baseline + getmaxy(canvas); ++row) {
      goto_line(row); // move to start of target row
      if (editbuffer.text.get_row() == row) { // guard against end
        render_row(editbuffer, old_row, old_column, highlight_cursor);
      }
    }

    // restore previous position
    goto_line(old_row);
    editbuffer.text.move_to_column(old_column);

    if (highlight_cursor && editbuffer.text.is_at_end()) {
      // add highlighted cursor at the end of the buffer
      waddch(canvas, ' '|A_STANDOUT);
    }
  }

  // Handle character escaping when displaying to the given window.
  void escape_char(WINDOW *window, char display, int attributes) {
    if (display == '\b' || display == '\x7f') {
      // special handling for backspace and delete
      waddch(window, '^'|attributes);
      waddch(window, (display == '\b' ? 'H' : '?')|attributes);
    } else if (static_cast<unsigned char>(display) > KeyBindings::MAX_CHAR) {
      // escape these with a backslash
      waddch(window, '\\'|attributes);
      char buf[11];
      std::snprintf(buf, sizeof(buf) / sizeof(char), "%o",
                    static_cast<unsigned char>(display));
      for (std::size_t i = 0; i < std::strlen(buf); ++i) {
        waddch(window, buf[i]|attributes);
      }
    } else {
      waddch(window, display|attributes);
    }
  }

  // Display a character in the window with proper highlighting.
  void display_char(Buffer &buffer, char display, bool highlight) {
    if (highlight && buffer.reverse) {
      wattroff(buffer.window, A_REVERSE);
      escape_char(buffer.window, display, A_NORMAL);
      wattron(buffer.window, A_REVERSE);
    } else if (highlight) {
      escape_char(buffer.window, display, A_STANDOUT);
    } else {
      escape_char(buffer.window, display, A_NORMAL);
    }
  }

  // Compute display width of a character written at column x.
  int display_width(int x, char c) {
    if (c == '\t') { // special case for tab
      int width = char_widths[static_cast<unsigned char>(c)];
      return width - x % width;
    } else {
      return char_widths[static_cast<unsigned char>(c)];
    }
  }

  // Render the current buffer row in the window.
  void render_row(Buffer &buffer, int cursor_row, int cursor_column,
                  bool highlight_cursor) {
    int init_x, init_y;
    getyx(buffer.window, init_y, init_x); // initial location
    render_current_row_prefix(buffer, cursor_row, cursor_column);
    for (int current_row = buffer.text.get_row();
         !buffer.text.is_at_end()
           && buffer.text.get_row() == current_row;
         buffer.text.forward()) {
      char c = buffer.text.data_at_cursor();
      // The display character is either ' ' (if it's a newline) or
      // the char. The display character is what gets highlighted if
      // the current position is at that point.
      char display = (c == '\n' || c == '\r') ? ' ' : c;
      bool highlight = false;
      if (highlight_cursor
          && buffer.text.get_row() == cursor_row
          && buffer.text.get_column() == cursor_column) {
        highlight = true;
      }

      int x, y;
      getyx(buffer.window, y, x); // current location
      if (c == '\n' && x == getmaxx(buffer.window) - 1 && y == init_y) {
        // Newline (edge case, newline at end of line)
        display_char(buffer, display, highlight);
      } else if (c == '\n' && x < getmaxx(buffer.window) - 1) {
        // Newline (common case)
        display_char(buffer, display, highlight);
        waddch(buffer.window, '\n');
      } else if (display_width(x, c) >= getmaxx(buffer.window) - x) {
        // Character goes off window
        display_char(buffer, display, highlight);
        wmove(buffer.window, init_y, getmaxx(buffer.window) - 1);
        waddch(buffer.window, buffer.right_overflow_marker);
        break;
      } else {
        // Show a regular character (common case)
        display_char(buffer, display, highlight);
      }
    }
  }

  // Render the start of a row if it is the current row. Moves the
  // buffer to the first character to be displayed.
  void render_current_row_prefix(Buffer &buffer, int cursor_row,
                                 int cursor_column) {
    if (cursor_row == buffer.text.get_row()) {
      // Show prefix
      std::string &prefix = buffer.get_prefix();
      for (std::size_t i = 0; i < prefix.size(); ++i) {
        display_char(buffer, prefix[i], false);
      }
      // Handle showing subset of current line if it is too long
      buffer.recompute_view_column(*this, cursor_row, cursor_column);
      if (buffer.view_column != 0) {
        // not showing line start - add marker
        display_char(buffer, buffer.left_overflow_marker, false);
      }
    }
  }

  // Move the baseline by half the window if the cursor is offscreen.
  // Also set the cursor row and reset the view column if needed.
  void rebase() {
    if (editbuffer.text.get_row() < baseline
        || editbuffer.text.get_row() >= baseline + getmaxy(canvas)) {
      baseline =
        std::max(1, editbuffer.text.get_row() - getmaxy(canvas) / 2);
      wclear(canvas); // required for some terminals
    }
    if (editbuffer.text.get_row() != cursor_row) {
      editbuffer.view_column = 0;
      cursor_row = editbuffer.text.get_row();
    }
  }

  // Read initial contents of the file.
  void read_file() {
    std::ifstream input(filename);
    const std::streamsize SIZE = 128;
    char arr[SIZE];
    char last = '\0';
    while (input) {
      input.read(arr, SIZE);
      for (std::streamsize i = 0; i < input.gcount(); ++i) {
        // Convert CR and CRLF to just LF
        if (last != '\r' || arr[i] != '\n') {
          editbuffer.text.insert(arr[i] == '\r' ? '\n' : arr[i]);
        }
        last = arr[i];
      }
    }
    // move to start of buffer
    while (editbuffer.text.get_row() != 1) {
      editbuffer.text.up();
    }
    editbuffer.text.move_to_row_start();
  }

  // Write the contents of the buffer to the file.
  bool write_file(const std::string &file_to_write) {
    std::ofstream output(file_to_write);
    std::string text = editbuffer.text.stringify();
    if (output << text) {
      filename = file_to_write;
      status = "saved";
      set_message("Wrote " + shorten_string(file_to_write),
                  "Wrote file");
      return true;
    } else {
      set_message("ERROR: Unable to write "
                  + shorten_string(file_to_write),
                  "Write FAILED");
    }
    return !modified;
  }
};


int main(int argc, char **argv) {
  std::string filename = "";
  FemtoEditor::InputMode input_mode = FemtoEditor::FEMTO_INPUT_MODE;
  if (argc > 1 && argv[1] == std::string("-r")) {
    input_mode = FemtoEditor::RAW;
    --argc;
    ++argv;
  } else if (argc > 1 && argv[1] == std::string("-t")) {
    input_mode = FemtoEditor::TERMINAL;
    --argc;
    ++argv;
  }
  if (argc > 1 && argv[1][0] == '-') {
    std::string arg = argv[1];
    int exit_value = 0;
    std::string info =
      "U-M FEMTO (FEMTO Editor for Manipulating Text Ostensibly) v";
    info += FemtoEditor::version;
    info += "\nAuthor: Amir Kamil";
    std::string usage = "Usage: ";
    usage += argv[0];
    usage += " [-r|-t] [filename]";
    usage += "\n\t-r\tenable raw input mode";
    usage += "\n\t-t\tenable terminal input mode";
    if (arg != "-h" && arg != "-v" && arg != "--help") {
      std::cout << "Unknown option " << arg << "\n";
      exit_value = 1;
    }
    std::cout << info << "\n" << usage << std::endl;
    return exit_value;
  }
  if (argc > 1) {
    filename = argv[1];
  }
  FemtoEditor fedit(filename, input_mode);
}
