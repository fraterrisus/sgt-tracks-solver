#ifndef _BOARD_H
#define _BOARD_H

#include <sstream>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <list>
#include <vector>
#include <deque>
#include <iterator>

#include "square.h"

class Board {

// Types
public:
  class Path: public std::deque< Square* > {
  public:
    void add_square(Square*);
    bool contains_square(Square*);
  };

// Members
public:
  int *row_count, *col_count;
private:
  int wid, hgt;
  Square *squares;

// Methods
private:
  int index_of(int, int);
  Square* square_at(int, int);

  void inspect_all_squares();

  static std::bitset<4> value_to_bitset(int);

  std::list< Path >* find_all_paths();
  bool squares_are_neighbors(Square*, Square*);

  bool solve_full_count();
  bool solve_forced_spaces();
  bool solve_no_loops();
  bool solve_dont_join_ends();
  bool solve_unreachable_spaces();

public:
  Board();
  Board(int, int);
  ~Board();

  void solve();

  std::string to_str();
  bool set_square(int, int);
  bool set_square(int, int, int);
};

#endif
