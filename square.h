#ifndef _SQUARE_H
#define _SQUARE_H

#include <iostream>
#include <string>
#include <bitset>

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
  bool is_solved();
  bool set_value(std::bitset<4>);
  bool add_value(int);
  State get_state() { get_edges(); return state; }
  bool set_state(State);

  int get_noes();
  int get_edges();
private:
};

#endif
