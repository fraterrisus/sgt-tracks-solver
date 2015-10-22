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

void Board::connect_grid() {
  int x, y;
  Square *me, *them;
  
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
    me->gap_n->state = Square::NO;

    me = square_at(x,hgt-1);
    me->gap_s = new Square::Gap;
    me->gap_s->state = Square::NO;
  }

  for (y=0; y<hgt; y++) {
    me = square_at(0,y);
    me->gap_w = new Square::Gap;
    me->gap_w->state = Square::NO;

    me = square_at(wid-1,y);
    me->gap_e = new Square::Gap;
    me->gap_e->state = Square::NO;
  }

}

Board::Board(int _w, int _h) {
  wid = _w;
  hgt = _h;
  col_count = new int[wid];
  row_count = new int[hgt];
  squares = new Square[wid * hgt];

  connect_grid();
}

Board::Board(Board &that) {
  wid = that.wid;
  hgt = that.hgt;
  col_count = new int[wid];
  row_count = new int[hgt];
  squares = new Square[wid * hgt];

  memcpy(row_count, that.row_count, hgt * sizeof(int));
  memcpy(col_count, that.col_count, wid * sizeof(int));
  connect_grid();

  for (int y=0; y<hgt; y++) {
    for (int x=0; x<wid; x++) {
      Square *me = square_at(x,y);
      Square *them = that.square_at(x,y);
      me->set_state(them->get_state());
      me->gap_s->state = them->gap_s->state;
      me->gap_e->state = them->gap_e->state;
    }
  }
}

Board::~Board() {
  if (col_count == 0) { delete(col_count); }
  if (row_count == 0) { delete(row_count); }
  // there's a malloc/free bug here
  //if (squares != 0) { delete(squares); }
}

/* ---------------------------------------- */

std::string Board::to_str() {
  std::stringstream rv;
  int x,y;

  if (col_count == 0) { return ""; }
  if (row_count == 0) { return ""; }
  if (squares == 0) { return ""; }

  for (x=0; x<wid; x++) { rv << std::setw(1) << col_count[x]; }
  rv << std::endl;
  for (y=0; y<hgt; y++) {
    rv << "";
    for (x=0; x<wid; x++) {
      rv << square_at(x,y)->to_str() << "";
    }
    rv << row_count[y] << std::endl;
  }

  return rv.str();
}

#define RED "\e[31m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define BLUE "\e[36m"
#define WHITE "\e[0m"

void Board::inspect_all_squares() {
  Square *rp, *sq;
  std::cout << "inspect_all_squares()" << std::endl;
  int i;

  std::cout << RED;
  for (i=0; i<wid; i++) {
    std::cout << std::setw(2) << col_count[i];
  }
  std::cout << WHITE << std::endl;
  i = 0;
  for (sq = squares; sq != 0; sq = sq->e) {
    std::cout << " " << BLUE << Square::state_to_sym(sq->gap_n->state) << WHITE;
  }
  std::cout << std::endl;
  for (rp = squares; rp != 0; rp = rp->s) {
    std::cout << BLUE << Square::state_to_sym(rp->gap_w->state) << WHITE;
    for (sq = rp; sq != 0; sq = sq->e) {
      std::cout << Square::state_to_str(sq->get_state());
      std::cout << BLUE << Square::state_to_sym(sq->gap_e->state) << WHITE;
    }
    std::cout << RED << " " << row_count[i] << WHITE;
    std::cout << std::endl;
    for (sq = rp; sq != 0; sq = sq->e) {
      std::cout << " " << BLUE << Square::state_to_sym(sq->gap_s->state) << WHITE;
    }
    std::cout << std::endl;
    i++;
  }
}

std::bitset<4> Board::value_to_bitset(int value) {
  std::bitset<4> bs;
  if ((value & 0x8) > 0) { bs[3] = true; }
  if ((value & 0x4) > 0) { bs[2] = true; }
  if ((value & 0x2) > 0) { bs[1] = true; }
  if ((value & 0x1) > 0) { bs[0] = true; }
  return bs;
}

bool Board::set_square(int idx, int value) {
  return squares[idx].set_value(value_to_bitset(value));
}

bool Board::set_square(int x, int y, int value) {
  return square_at(x,y)->set_value(value_to_bitset(value));
}

/** If these two Squares are neighbors, set the gap between them to NO. */
bool Board::ungap_neighbors(Square *a, Square *b) {
  bool changes = false;
  if ((a->s == b) && (a->gap_s->state == Square::UNKN)) { 
#ifdef DEBUG
    std::cout << std::hex << "  ungap " << a << " S " << b << std::endl;
#endif
    a->gap_s->state = Square::NO; 
    changes = true; 
  }
  if ((a->w == b) && (a->gap_w->state == Square::UNKN)) { 
#ifdef DEBUG
    std::cout << std::hex << "  ungap " << a << " W " << b << std::endl;
#endif
    a->gap_w->state = Square::NO; 
    changes = true; 
  }
  if ((a->n == b) && (a->gap_n->state == Square::UNKN)) { 
#ifdef DEBUG
    std::cout << std::hex << "  ungap " << a << " N " << b << std::endl;
#endif
    a->gap_n->state = Square::NO; 
    changes = true; 
  }
  if ((a->e == b) && (a->gap_e->state == Square::UNKN)) { 
#ifdef DEBUG
    std::cout << std::hex << "  ungap " << a << " E " << b << std::endl;
#endif
    a->gap_e->state = Square::NO; 
    changes = true; 
  }
  return changes;
}
