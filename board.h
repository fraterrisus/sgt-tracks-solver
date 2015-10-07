#ifndef _BOARD_H
#define _BOARD_H

#include <sstream>
#include <string>
#include <iomanip>

#include "square.h"

class Board {

public:
  int *row_count, *col_count;
private:
  int wid, hgt;
  Square *squares;

private:
  int index_of(int, int);
  Square* square_at(int, int);

public:
  Board();
  Board(int, int);
  ~Board();

  std::string to_str();
};

#endif
