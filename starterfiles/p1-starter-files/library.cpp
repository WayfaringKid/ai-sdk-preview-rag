/* library.cpp
 *
 * Libraries needed for EECS 280 Statistics project
 * 
 * by Andrew DeOrio <awdeorio@umich.edu>
 * 2015-04-28
 */

////////////////////////////// BEGIN csvstream.hpp //////////////////////////////
//////////// GitHub hash 7523c43186070edd07d4de2b5c829380cd506d42 /////////////
/* -*- mode: c++ -*- */
#ifndef CSVSTREAM_HPP
#define CSVSTREAM_HPP
/* csvstream.hpp
 *
 * Andrew DeOrio <awdeorio@umich.edu>
 *
 * An easy-to-use CSV file parser for C++
 * https://github.com/awdeorio/csvstream
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <exception>


// A custom exception type
class csvstream_exception : public std::exception {
public:
  const char * what () const noexcept override {
    return msg.c_str();
  }
  const std::string msg;
  csvstream_exception(const std::string &msg) : msg(msg) {};
};


// csvstream interface
class csvstream {
public:
  // Constructor from filename. Throws csvstream_exception if open fails.
  csvstream(const std::string &filename, char delimiter=',', bool strict=true)
    : filename(filename),
      is(fin),
      delimiter(delimiter),
      strict(strict),
      line_no(0) {

    // Open file
    fin.open(filename.c_str());
    if (!fin.is_open()) {
      throw csvstream_exception("Error opening file: " + filename);
    }

    // Process header
    read_header();
  }

  // Constructor from stream
  csvstream(std::istream &is, char delimiter=',', bool strict=true)
    : filename("[no filename]"),
      is(is),
      delimiter(delimiter),
      strict(strict),
      line_no(0) {
    read_header();
  }

  // Destructor
  ~csvstream() {
    if (fin.is_open()) fin.close();
  }

  // Return false if an error flag on underlying stream is set
  explicit operator bool() const {
    return static_cast<bool>(is);
  }

  // Return header processed by constructor
  std::vector<std::string> getheader() const {
    return header;
  }

  // Stream extraction operator reads one row. Throws csvstream_exception if
  // the number of items in a row does not match the header.
  csvstream & operator>> (std::map<std::string, std::string>& row) {
    return extract_row(row);
  }

  // Stream extraction operator reads one row, keeping column order. Throws
  // csvstream_exception if the number of items in a row does not match the
  // header.
  csvstream & operator>> (std::vector<std::pair<std::string, std::string> >& row) {
    return extract_row(row);
  }

private:
  // Filename.  Used for error messages.
  std::string filename;

  // File stream in CSV format, used when library is called with filename ctor
  std::ifstream fin;

  // Stream in CSV format
  std::istream &is;

  // Delimiter between columns
  char delimiter;

  // Strictly enforce the number of values in each row.  Raise an exception if
  // a row contains too many values or too few compared to the header.  When
  // strict=false, ignore extra values and set missing values to empty string.
  bool strict;

  // Line no in file.  Used for error messages
  size_t line_no;

  // Store header column names
  std::vector<std::string> header;

  // Disable copying because copying streams is bad!
  csvstream(const csvstream &);
  csvstream & operator= (const csvstream &);

  /////////////////////////////////////////////////////////////////////////////
  // Implementation

  // Read and tokenize one line from a stream
  static bool read_csv_line(std::istream &is,
                            std::vector<std::string> &data,
                            char delimiter
                            ) {

    // Add entry for first token, start with empty string
    data.clear();
    data.push_back(std::string());

    // Process one character at a time
    char c = '\0';
    enum State {BEGIN, QUOTED, QUOTED_ESCAPED, UNQUOTED, UNQUOTED_ESCAPED, END};
    State state = BEGIN;
    while(is.get(c)) {
      switch (state) {
      case BEGIN:
        // We need this state transition to properly handle cases where nothing
        // is extracted.
        state = UNQUOTED;

        // Intended switch fallthrough.  Beginning with GCC7, this triggers an
        // error by default.  Disable the error for this specific line.
#if __GNUG__ && __GNUC__ >= 7
        [[fallthrough]];
#endif

      case UNQUOTED:
        if (c == '"') {
          // Change states when we see a double quote
          state = QUOTED;
        } else if (c == '\\') { //note this checks for a single backslash char
          state = UNQUOTED_ESCAPED;
          data.back() += c;
        } else if (c == delimiter) {
          // If you see a delimiter, then start a new field with an empty string
          data.push_back("");
        } else if (c == '\n' || c == '\r') {
          // If you see a line ending *and it's not within a quoted token*, stop
          // parsing the line.  Works for UNIX (\n) and OSX (\r) line endings.
          // Consumes the line ending character.
          state = END;
        } else {
          // Append character to current token
          data.back() += c;
        }
        break;

      case UNQUOTED_ESCAPED:
        // If a character is escaped, add it no matter what.
        data.back() += c;
        state = UNQUOTED;
        break;

      case QUOTED:
        if (c == '"') {
          // Change states when we see a double quote
          state = UNQUOTED;
        } else if (c == '\\') {
          state = QUOTED_ESCAPED;
          data.back() += c;
        } else {
          // Append character to current token
          data.back() += c;
        }
        break;

      case QUOTED_ESCAPED:
        // If a character is escaped, add it no matter what.
        data.back() += c;
        state = QUOTED;
        break;

      case END:
        if (c == '\n') {
          // Handle second character of a Windows line ending (\r\n).  Do
          // nothing, only consume the character.
        } else {
          // If this wasn't a Windows line ending, then put character back for
          // the next call to read_csv_line()
          is.unget();
        }

        // We're done with this line, so break out of both the switch and loop.
        goto multilevel_break; //This is a rare example where goto is OK
        break;

      default:
        assert(0);
        throw state;

      }//switch
    }//while

  multilevel_break:
    // Clear the failbit if we extracted anything.  This is to mimic the
    // behavior of getline(), which will set the eofbit, but *not* the failbit
    // if a partial line is read.
    if (state != BEGIN) is.clear();

    // Return status is the underlying stream's status
    return static_cast<bool>(is);
  }

  // Process header, the first line of the file
  void read_header() {
    // read first line, which is the header
    if (!read_csv_line(is, header, delimiter)) {
      throw csvstream_exception("error reading header");
    }
  }

  // Extract a row into a map
  csvstream & extract_row(std::map<std::string, std::string>& row) {
    // Clear input row
    row.clear();

    // Read one line from stream, bail out if we're at the end
    std::vector<std::string> data;
    if (!read_csv_line(is, data, delimiter)) return *this;
    line_no += 1;

    // When strict mode is disabled, coerce the length of the data.  If data is
    // larger than header, discard extra values.  If data is smaller than header,
    // pad data with empty strings.
    if (!strict) {
      data.resize(header.size());
    }

    // Check length of data
    if (data.size() != header.size()) {
      auto msg = "Number of items in row does not match header. " +
        filename + ":L" + std::to_string(line_no) + " " +
        "header.size() = " + std::to_string(header.size()) + " " +
        "row.size() = " + std::to_string(data.size()) + " "
        ;
      throw csvstream_exception(msg);
    }

    // combine data and header into a row object
    for (size_t i=0; i<data.size(); ++i) {
      row[header[i]] = data[i];
    }

    return *this;
  }

  // Extract a row into a vector of pairs
  csvstream & extract_row(std::vector<std::pair<std::string, std::string> >& row) {
    // Clear input row
    row.clear();
    row.resize(header.size());

    // Read one line from stream, bail out if we're at the end
    std::vector<std::string> data;
    if (!read_csv_line(is, data, delimiter)) return *this;
    line_no += 1;

    // When strict mode is disabled, coerce the length of the data.  If data is
    // larger than header, discard extra values.  If data is smaller than header,
    // pad data with empty strings.
    if (!strict) {
      data.resize(header.size());
    }

    // Check length of data
    if (row.size() != header.size()) {
      auto msg = "Number of items in row does not match header. " +
        filename + ":L" + std::to_string(line_no) + " " +
        "header.size() = " + std::to_string(header.size()) + " " +
        "row.size() = " + std::to_string(row.size()) + " "
        ;
      throw csvstream_exception(msg);
    }

    // combine data and header into a row object
    for (size_t i=0; i<data.size(); ++i) {
      row[i] = make_pair(header[i], data[i]);
    }

    return *this;
  }
};

#endif
/////////////////////////////// END csvstream.hpp ///////////////////////////////

////////////////////////////// library functions /////////////////////////////

#include "library.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
using namespace std;

vector<double> extract_column(string filename,
                              string column_name) {

  // open file
  ifstream fin;
  fin.open(filename.c_str());
  if (!fin.is_open()) {
    cout << "Error opening " << filename << "\n";
    exit(1);
  }
  
  cout << "reading column " << column_name << " from " << filename << endl;

  // Guess delimiter based on filename
  char delimiter = ' ';
  string filename_lower = filename;
  transform(filename_lower.begin(), filename_lower.end(), filename_lower.begin(), ::tolower);

  if (filename_lower.find(".csv") != string::npos) { delimiter = ','; }
  else if (filename_lower.find(".tsv") != string::npos) { delimiter = '\t'; }

  // use csvstream to read file
  csvstream csvin(fin, delimiter);

  // check for column name not found
  vector<string> header = csvin.getheader();
  size_t column = header.size();
  for (size_t i = 0; i < header.size(); ++i) {
    if (header[i] == column_name) {
      column = i;
      break;
    }
  }
  if (column == header.size()) {
    cout << "Error: column name " << column_name << " not found in "
         << filename << "\n";
    fin.close();
    exit(1);
  }

  // extract column of data
  vector<double> column_data;
  vector<pair<string, string>> row;
  while (csvin >> row) {
    try {
      column_data.push_back(stod(row[column].second));
    }
    catch (invalid_argument &e) {
      // represent empty or invalid data as NaN
      column_data.push_back(std::numeric_limits<double>::quiet_NaN());
    }
  }

  return column_data;
}

//REQUIRES: v1 and v2 have the same number of elements
//MODIFIES: v1, v2
//EFFECTS: Removes elements at indices x from v1 and v2 where at least one of
//         v1[x] or v2[x] is missing (as represented by NaN - "Not a Number").
void remove_missing(vector<double> &v1, vector<double> &v2) {
  assert(v1.size() == v2.size());
  vector<double> filtered_v1;
  vector<double> filtered_v2;
  for (size_t i = 0; i < v1.size(); ++i) {
    if (!isnan(v1[i]) && !isnan(v2[i])) {
      filtered_v1.push_back(v1[i]);
      filtered_v2.push_back(v2[i]);
    }
  }
  v1 = filtered_v1;
  v2 = filtered_v2;
}

pair<vector<double>, vector<double>> extract_columns(
  string filename, string column_name1, string column_name2) {
  vector<double> v1 = extract_column(filename, column_name1);
  vector<double> v2 = extract_column(filename, column_name2);
  remove_missing(v1, v2);
  return {v1, v2};
}

vector<double> bootstrap_resample(vector<double> data, int sample_num) {
  assert(0 <= sample_num && sample_num < 10000);
  if (data.size() == 0) {
    return vector<double>();
  }

  // mt19937 is guaranteed to give consistent results across platforms:
  //  https://stackoverflow.com/questions/51929085/cross-platform-random-reproducibility
  static mt19937 mt;

  // Normalize order of data to ensure consistent sampling for autograding.
  std::sort(data.begin(), data.end());

  // Seed the random engine with a simple hash of the data plus the sample number.
  // This is not a legitimate approach and is subject to collisions, but
  // is intended here to provide consistent RNG for autograding.
  int hash = 0;
  hash += 100000 * data.size() % 10;
  hash += 10000 * static_cast<int>(data[0]) % 10;
  hash += sample_num;
  mt.seed(hash);

  // Resample with replacement
  vector<double> resample;
  resample.reserve(data.size());
  for(size_t i = 0; i < data.size(); ++i) {
    // Use % as a simple approach to generating a uniform distribution over indices,
    // which is consistent across platforms for autograding purposes. This
    // intoduces "modulo bias", but this effect minimal for our application.
    // https://stackoverflow.com/questions/10984974/why-do-people-say-there-is-modulo-bias-when-using-a-random-number-generator
    // A real implementation could use std::uniform_int_distribution, but that is not
    // guaranteed to give consistent results across platforms.
    resample.push_back(data[mt() % data.size()]);
  }
  return resample;
}