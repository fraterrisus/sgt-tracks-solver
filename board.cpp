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

/* ---------------------------------------- */

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
    std::cout << " " << col_count[i];
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

/* ---------------------------------------- */

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

/* ---------------------------------------- */

bool Board::solve_forced_spaces() {
  Square *rp, *sq;
  bool changes = false;

  std::cout << "solve_forced_spaces()" << std::endl;

  int x=0, y=0;
  for (rp = squares; rp != 0; rp = rp->s) {
    x=0;
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->update_state()) {
        std::cout << "  (" << x << "," << y << ") updated" << std::endl;
        changes = true;
        return true;
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
    std::cout << "  row " << j << " " << row_count[j] << "t " << y << "y" << n << "n" << m << "m";
    if (m > 0) {
      // Row has the right number of Ys; set Us to N
      if (row_count[j] == y) {
        std::cout << "  fill ";
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::NO))) { 
            std::cout << "N";
            changes = true; 
          }
        }
      }
      // Row has the right number of Ns; set Us to Y
      else if ((hgt - row_count[j]) == n) {
        std::cout << "  fill ";
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::YES))) {
            std::cout << "Y";
            changes = true;
          }
        }
      }
      // Row has one-short of the right numbers of Ys; set Gaps between Us to N
      // (In this case, there can't be any *additional* instances of Gaps
      // between Squares in this row being Y; otherwise, it would force the
      // number of Ys to be +2, which would be too many.)
      else if (row_count[j] - y == 1) {
        std::cout << "  gaps ";
        for (sq = square_at(0,j); sq->e != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->e->get_state() == Square::UNKN) &&
              (sq->gap_e->state != Square::NO)) {
            std::cout << "N";
            changes = true;
            sq->gap_e->state = Square::NO;
          }
        }
      }
    }
    std::cout << std::endl;
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
    std::cout << "  col " << i << " " << col_count[i] << "t " << y << "y" << n << "n" << m << "m";
    if (m > 0) {
      if (col_count[i] == y) {
        std::cout << "  fill ";
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::NO))) { 
            std::cout << "N";
            changes = true; 
          }
        }
      }
      else if ((hgt - col_count[i]) == n) {
        std::cout << "  fill ";
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::YES))) { 
            std::cout << "Y";
            changes = true; 
          }
        }
      }
      else if (col_count[i] - y == 1) {
        std::cout << "  gaps ";
        for (sq = square_at(i,0); sq->s != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->s->get_state() == Square::UNKN) &&
              (sq->gap_s->state != Square::NO)) {
            std::cout << "N";
            changes = true;
            sq->gap_s->state = Square::NO;
          }
        }
      }
    }
    std::cout << std::endl;
  }
  return changes;
}

bool Board::solve_unreachable_spaces() {
  std::list< Square* > *reachable = new std::list< Square* >;
  std::list< Square* >::iterator p, q;
  Square *rp, *sq;
  bool changes = false;

  std::cout << "solve_unreachable_spaces()" << std::endl;

  // All YES Squares are reachable.
  for (rp = squares; rp != 0; rp = rp->s) {
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::YES) {
        std::cout << "  push " << std::hex << sq << std::endl;
        reachable->push_back(sq);
      }
    }
  }

  // All UNKN neighbors of Squares on the reachable list are themselves
  // reachable (but don't add them if they're already on the list).
  //
  // The iterator p marks the division between squares that are reachable
  // (begin > p) and squares that are on the worklist to be examined (p > end).
  // At each iteration, p points to the next item on the worklist to be
  // examined; at the end of the iteration, that item is "moved onto the
  // reachable list" by advancing the pointer.
  for (p = reachable->begin(); p != reachable->end(); p++) {
    std::cout << "  look " << std::hex << *p << " " << (*p)->to_str() << std::endl;
    Square* neighbors[4];
    neighbors[0] = (*p)->w;
    neighbors[1] = (*p)->n;
    neighbors[2] = (*p)->e;
    neighbors[3] = (*p)->s;
    for (int i = 0; i < 4; i++) {
      sq = neighbors[i];
      if ((sq != 0) && (sq->get_state() == Square::UNKN)) {
        bool found = false;
        for (q = reachable->begin(); q != reachable->end(); q++) {
          if (*q == sq) { 
            std::cout << "  ign  " << std::hex << sq << " " << sq->to_str() << std::endl;
            found = true; 
            break; 
          }
        }
        if (!found) {
          std::cout << "  push " << std::hex << sq << " " << sq->to_str() << std::endl;
          reachable->push_back(sq);
        }
      }
    }
  }

  // All UNKN Squares that aren't on the reachable list are unreachable; 
  // mark them NO.
  for (rp = squares; rp != 0; rp = rp->s) {
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::UNKN) {
        bool found = false;
        for (q = reachable->begin(); q != reachable->end(); q++) {
          if (*q == sq) { found = true; break; }
        }
        if ((!found) && (sq->set_state(Square::NO))) changes = true;
      }
    }
  }

  delete(reachable);
  return changes;
}

void Board::solve() {
  bool changes = true;
  while (changes) {
    changes = false;

    //std::cout << "----\n" << to_str();
    inspect_all_squares();
    //std::cout << std::endl;
    
    changes = changes || solve_forced_spaces();
    changes = changes || solve_full_count();
    changes = changes || solve_unreachable_spaces();
  }
}
