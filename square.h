#ifndef _SQUARE_H
#define _SQUARE_H

#include <string>

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
};

#endif
