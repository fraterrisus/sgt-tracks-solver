#ifndef _SQUARE_H
#define _SQUARE_H

#include <string>

#define SOUTH 0x8
#define WEST 0x4
#define NORTH 0x2
#define EAST 0x1

class Square {

/* classes */

public:
  enum State { UNKN, YES, NO };

  class Gap {
  public:
    Square::State state;
    Gap() { state = UNKN; }
  };

/* members */

public:
  Gap *gap_n, *gap_s, *gap_e, *gap_w;
  Square *n, *s, *e, *w;

private:
  State state;

/* methods */

public:
  Square();
  ~Square();
  std::string to_str();
  bool add_value(int);

private:
  int get_edges();
};

#endif
