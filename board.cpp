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
  return squares[idx].set_value(value);
}

bool Board::set_square(int x, int y, int value) {
  return square_at(x,y)->set_value(value);
}

/* ---------------------------------------- */

bool Board::solve_forced_spaces() {
  Square *rp, *sq;
  bool changes = false;

  std::cout << "solve_forced_spaces()" << std::endl;

  int x=0, y=0;
  for (rp = squares; rp != 0; rp = rp->s) {
    x=0;
    for (sq = rp; sq != 0; sq = sq->e) {
      if ((sq->get_state() == Square::YES) && (!sq->is_solved())) {
        std::cout << "  (" << x << "," << y << ") ";
        std::bitset<4> ys = sq->get_edges();
        std::bitset<4> ns = sq->get_noes();
        if ((ns.count() == 2) && (ys.count() != 2)) {
          ns.flip();
          std::cout << " set " << ns;
          sq->set_value(ns);
          changes = true;
        }
        std::cout << std::endl;
      }
      x++;
    }
    y++;
  }
  return changes;
}

bool Board::solve_full_count() {
  int i,j;
  int y,n,m;
  bool changes = false;
  Square *sq;

  std::cout << "solve_full_count()" << std::endl;

  for (j=0; j<hgt; j++) {
    y=0; n=0; m=0;
    for (sq = square_at(0,j); sq != 0; sq = sq->e) {
      switch (sq->get_state()) {
        case Square::YES: y++; break;
        case Square::NO: n++; break;
        case Square::UNKN: m++; break;
      }
    }
    std::cout << "  row " << j << " " << row_count[j] << "t " << y << "y" << n << "n" << m << "m" << std::endl;
    if (m > 0) {
      if (row_count[j] == y) {
        std::cout << "  filling in N" << std::endl;
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if (sq->get_state() == Square::UNKN) {
            changes = true;
            sq->set_state(Square::NO);
          }
        }
      }
      if ((hgt - row_count[j]) == n) {
        std::cout << "  filling in Y" << std::endl;
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if (sq->get_state() == Square::UNKN) {
            changes = true;
            sq->set_state(Square::YES);
          }
        }
      }
    }
  }

  for (i=0; i<wid; i++) {
    y=0; n=0; m=0;
    for (sq = square_at(i,0); sq != 0; sq = sq->s) {
      switch (sq->get_state()) {
        case Square::YES: y++; break;
        case Square::NO: n++; break;
        case Square::UNKN: m++; break;
      }
    }
    std::cout << "  col " << i << " " << col_count[i] << "t " << y << "y" << n << "n" << m << "m" << std::endl;
    if (m > 0) {
      if (col_count[i] == y) {
        std::cout << "  filling in N" << std::endl;
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if (sq->get_state() == Square::UNKN) {
            changes = true;
            sq->set_state(Square::NO);
          }
        }
      }
      if ((hgt - col_count[i]) == n) {
        std::cout << "  filling in Y" << std::endl;
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if (sq->get_state() == Square::UNKN) {
            changes = true;
            sq->set_state(Square::YES);
          }
        }
      }
    }
  }
  return changes;
}

void Board::solve() {
  bool changes = true;
  while (changes) {
    changes = false;
    changes = changes || solve_forced_spaces();
    changes = changes || solve_full_count();
    std::cout << to_str() << std::endl;
  }
}
