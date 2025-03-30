/***************************************
 ** E0: a hacky text editor front-end **
 **                                   **
 **         by William Lovas          **
 **         and Robert Simmons        **
 **          ported to C++ by         **
 **          Saquib Razak             **
 **                                   **
 ***************************************/

#include <iostream>
#include <string>
#include <utility> // pair
#include <ncurses.h>
#include "TextBuffer.hpp"

using namespace std;

void render_topbar(WINDOW *window) {
  werase(window);
  for (int i = getbegx(window); i < getmaxx(window); ++i)
    waddch(window, ' ');
  wmove(window, 0, 1);
  waddstr(window, "E0, the minimalist editor -- ^X to exit, ^L to refresh");
}

void render_botbar(TextBuffer &buffer, WINDOW *window) {
  werase(window);
  for (int i = getbegx(window); i < getmaxx(window); ++i)
    waddch(window, ' '|A_STANDOUT);
  wmove(window, 0, 1);
  wattron(window, A_REVERSE);

  string msg = "Position (";
  msg = msg + std::to_string(buffer.get_row());
  msg = msg +  ",";
  msg = msg + std::to_string(buffer.get_column());
  msg = msg+ ")";
  waddstr(window, msg.c_str());
  wattroff(window, A_REVERSE);
}

void render_buf(TextBuffer &buffer, WINDOW *window) {
  wmove(window, 0, 0);
  werase(window);

  std::string data = buffer.stringify();
  int cursor = buffer.get_index();
  for (int i = 0; i < static_cast<int>(data.size()); ++i) {
    char c = data[i];
    // The display character is either ' ' (if it's a newline) or the char
    // The display character is what gets highlighted if we're at the point
    int display = c == '\n' ? ' ' : c;
    if (i == cursor) display = display|A_STANDOUT;
    int x, y;
    getyx(window, y, x);
    if (y == getmaxy(window) - 1) {
      // Special corner cases: last line of the buffer
      if (c != '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Show a regular character (common case)
      } else {
        if (c == '\n') waddch(window, display);
        getyx(window,y,x);
        while (x != getmaxx(window) - 1){
          waddch(window, ' ');
          getyx(window, y, x);
        }
        waddch(window, '>');
        return;
      }
    } else {
      // Normal cases: in the buffer
      getyx(window, y, x);
      if (c != '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Show a regular character (common case)
      } else if (c == '\n' && x < getmaxx(window) - 1) {
        waddch(window, display); // Newline (common case)
        waddch(window, '\n');
      } else if (c == '\n') {
        waddch(window, display); // Newline (edge case, newline at end of line)
      } else {
        waddch(window, '\\');
        waddch(window, display); // Wrap to the next line
      }
    }
  }

  // We're at the end of the buffer. This only matters if end = cursor
  if (buffer.is_at_end()) {
    waddch(window,' '|A_STANDOUT);
  }
}

/** main: look the other way if you've ever programmed using curses **/
int main() {
  cout << "Starting\n";
  WINDOW *mainwin = initscr();
  cbreak();
  noecho();
  keypad(mainwin, true);
  int vis = curs_set(0);

  int ncols = getmaxx(mainwin);
  int nlines = getmaxy(mainwin);
  int begx = getbegx(mainwin);
  int begy = getbegy(mainwin);

  WINDOW *canvas = subwin(mainwin,
                          nlines - 3  /* lines: save 3 for status info */,
                          ncols       /* cols: same as main window */,
                          begy + 1    /* begy: skip top status bar */,
                          begx        /* begx: same as main window */);
  WINDOW *topbar = subwin(mainwin, 1 /* lines */, ncols, begy, begx);
  WINDOW *botbar = subwin(mainwin, 1 /* lines */, ncols, nlines - 2, begx);
  TextBuffer buffer;
  render_topbar(topbar);

  while (true) {
    render_buf(buffer, canvas);
    wrefresh(canvas);
    render_botbar(buffer, botbar);
    wrefresh(botbar);

    int c = getch();
    if (c == 24) /* ^X */ { break; }
    else if (c == 12) /* ^L */ {
      wclear(mainwin);
      render_topbar(topbar);
      wrefresh(mainwin);
    }
    else if (c == KEY_BACKSPACE || c == 127) {
      if (buffer.backward()) { buffer.remove(); }
    }
    else if (c == KEY_DC)  { buffer.remove(); }
    else if (c == KEY_LEFT)  { buffer.backward(); }
    else if (c == KEY_RIGHT) { buffer.forward(); }
    else if (c == KEY_ENTER) { buffer.insert('\n'); }
    else if (c == KEY_UP) { buffer.up(); }
    else if (c == KEY_DOWN) { buffer.down(); }
    else if (c == KEY_HOME) { buffer.move_to_row_start(); }
    else if (c == KEY_END) { buffer.move_to_row_end(); }
    else if (0 < c && c < 127) { buffer.insert(c); }
  }

  curs_set(vis);
  endwin();

  cout << "thanks for flying E !" << endl; // <- wing commander homage -wjl
}
