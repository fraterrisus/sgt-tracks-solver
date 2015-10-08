#include "board.h"

Board::Board() {
  wid = 0;
  hgt = 0;
  col_count = 0;
  row_count = 0;
  squares = 0;
}

int Board::index_of(int x, int y) {
  return x + (y * wid);
}

Square* Board::square_at(int x, int y) {
  return squares + index_of(x,y);
}

Board::Board(int _w, int _h) {
  int x, y;
  Square *me, *them;

  wid = _w;
  hgt = _h;
  col_count = new int[wid];
  row_count = new int[hgt];
  squares = new Square[wid * hgt];

  // Establish N-S relationships
  for (y=0; y<hgt-1; y++) {
    for (x=0; x<wid; x++) {
      me = square_at(x,y);
      me->gap_s = new Square::Gap;

      them = square_at(x,y+1);
      me->s = them; them->n = me; them->gap_n = me->gap_s;
    }
  }

  // Establish E-W relationships
  for (x=0; x<wid-1; x++) {
    for (y=0; y<hgt; y++) {
      me = square_at(x,y);
      me->gap_e = new Square::Gap;

      them = square_at(x+1,y);
      me->e = them; them->w = me; them->gap_w = me->gap_e;
    }
  }

  // Manage border
  for (x=0; x<wid; x++) {
    me = square_at(x,0);
    me->gap_n = new Square::Gap;

    me = square_at(x,hgt-1);
    me->gap_s = new Square::Gap;
  }

  for (y=0; y<hgt; y++) {
    me = square_at(0,y);
    me->gap_w = new Square::Gap;

    me = square_at(wid-1,y);
    me->gap_e = new Square::Gap;
  }

}

Board::~Board() {
  if (col_count != 0) { delete(col_count); }
  if (row_count != 0) { delete(row_count); }
  // there's a malloc/free bug here
  //if (squares != 0) { delete(squares); }
}

std::string Board::to_str() {
  std::stringstream rv;
  int x,y;

  if (col_count == 0) { return ""; }
  if (row_count == 0) { return ""; }
  if (squares == 0) { return ""; }

  for (x=0; x<wid; x++) { rv << std::setw(2) << col_count[x]; }
  rv << std::endl;
  for (y=0; y<hgt; y++) {
    rv << " ";
    for (x=0; x<wid; x++) {
      rv << square_at(x,y)->to_str() << " ";
    }
    rv << std::setw(2) << row_count[y] << std::endl;
  }

  return rv.str();
}

bool Board::set_square(int idx, int value) {
  return squares[idx].add_value(value);
}

bool Board::set_square(int x, int y, int value) {
  return square_at(x,y)->add_value(value);
}
