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

#ifdef DEBUG
  std::cout << "solve_forced_spaces()" << std::endl;
#endif

  int x=0, y=0;
  for (rp = squares; rp != 0; rp = rp->s) {
    x=0;
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->update_state()) {
#ifdef DEBUG
        std::cout << "  (" << x << "," << y << ") updated" << std::endl;
#endif
        changes = true;
        // uncomment to force single-step updates
        // return changes;
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

#ifdef DEBUG
  std::cout << "solve_full_count()" << std::endl;
#endif

  for (j=0; j<hgt; j++) {
    y=0; n=0; m=0;
    for (sq = square_at(0,j); sq != 0; sq = sq->e) {
      switch (sq->get_state()) {
        case Square::YES: y++; break;
        case Square::NO: n++; break;
        case Square::UNKN: m++; break;
      }
    }
    if (m > 0) {
      // Row has the right number of Ys; set Us to N
      if (row_count[j] == y) {
#ifdef DEBUG
        std::cout << "  row " << j << " " << row_count[j] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  fill ";
#endif
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::NO))) { 
#ifdef DEBUG
            std::cout << "N";
#endif
            changes = true; 
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
      // Row has the right number of Ns; set Us to Y
      else if ((hgt - row_count[j]) == n) {
#ifdef DEBUG
        std::cout << "  row " << j << " " << row_count[j] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  fill ";
#endif
        for (sq = square_at(0,j); sq != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::YES))) {
#ifdef DEBUG
            std::cout << "Y";
#endif
            changes = true;
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
      // Row has one-short of the right numbers of Ys; set Gaps between Us to N
      // (In this case, there can't be any *additional* instances of Gaps
      // between Squares in this row being Y; otherwise, it would force the
      // number of Ys to be +2, which would be too many.)
      else if (row_count[j] - y == 1) {
#ifdef DEBUG
        std::cout << "  row " << j << " " << row_count[j] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  gaps ";
#endif
        for (sq = square_at(0,j); sq->e != 0; sq = sq->e) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->e->get_state() == Square::UNKN) &&
              (sq->gap_e->state != Square::NO)) {
#ifdef DEBUG
            std::cout << "N";
#endif
            changes = true;
            sq->gap_e->state = Square::NO;
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
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
    if (m > 0) {
      if (col_count[i] == y) {
#ifdef DEBUG
        std::cout << "  col " << i << " " << col_count[i] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  fill ";
#endif
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::NO))) { 
#ifdef DEBUG
            std::cout << "N";
#endif
            changes = true; 
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
      else if ((hgt - col_count[i]) == n) {
#ifdef DEBUG
        std::cout << "  col " << i << " " << col_count[i] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  fill ";
#endif
        for (sq = square_at(i,0); sq != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->set_state(Square::YES))) { 
#ifdef DEBUG
            std::cout << "Y";
#endif
            changes = true; 
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
      else if (col_count[i] - y == 1) {
#ifdef DEBUG
        std::cout << "  col " << i << " " << col_count[i] << "t " << y << "y" << n << "n" << m << "m";
        std::cout << "  gaps ";
#endif
        for (sq = square_at(i,0); sq->s != 0; sq = sq->s) {
          if ((sq->get_state() == Square::UNKN) &&
              (sq->s->get_state() == Square::UNKN) &&
              (sq->gap_s->state != Square::NO)) {
#ifdef DEBUG
            std::cout << "N";
#endif
            changes = true;
            sq->gap_s->state = Square::NO;
          }
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
    }
  }
  return changes;
}

bool Board::Path::contains_square(Square *sq) {
  for (auto p = begin(); p != end(); p++) { if (*p == sq) { return true; } }
  return false;
}

void Board::Path::add_square(Square* sq) {
  if (! this->contains_square(sq)) {
#ifdef DEBUG
#ifdef VERBOSE
    std::cout << "      push " << std::hex << sq << std::endl;
#endif
#endif
    this->push_back(sq); 
  }
}

bool Board::squares_are_neighbors(Square *a, Square *b) {
  if ((a->e == b) || (a->w == b) || (a->n == b) || (a->s == b)) return true;
  return false;
}

std::list< Board::Path >* Board::find_all_paths() {
  std::list< Board::Path > *paths = new std::list< Board::Path >;
  std::list< Square* > candidates;
  Square *rp, *sq;

  // Build list of candidates from Squares = YES
  for (rp = squares; rp != 0; rp = rp->s) {
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::YES) {
#ifdef DEBUG
#ifdef VERBOSE
        std::cout << "  cand " << std::hex << sq << std::endl;
#endif
#endif
        candidates.push_back(sq);
      }
    }
  }

  while (!candidates.empty()) {
    // Take the next candidate and start a new Path
    Board::Path path;
    Board::Path::iterator p, q;
#ifdef DEBUG
#ifdef VERBOSE
    std::cout << "  new path" << std::endl;
#endif
#endif

    Square *x = candidates.front();
    candidates.pop_front();
    path.push_back(x);

    for (p = path.begin(); p != path.end(); p++) {
      Square *sq = *p;
#ifdef DEBUG
#ifdef VERBOSE
      std::cout << "    work " << std::hex << sq << std::endl;
#endif
#endif
    
      // Find neighbors along this track, based on YES Gaps.
      // Pull them off the candidate list and add them to this Path.
      //
      // This implementation is a little dodgy; we're assuming that the
      // neighbor is YES based on the fact that the gap is YES, and that the
      // neighbor is already on the candidate list. The first should be true if
      // we've updated our squares correctly. The second should be true unless
      // we try to remove it multiple times, but if that's the case then we
      // already have a loop, which is a different problem.
      if ((sq->gap_n->state == Square::YES) && (sq->n != 0))
      { candidates.remove(sq->n); path.add_square(sq->n); }
      if ((sq->gap_s->state == Square::YES) && (sq->s != 0))
      { candidates.remove(sq->s); path.add_square(sq->s); }
      if ((sq->gap_e->state == Square::YES) && (sq->e != 0))
      { candidates.remove(sq->e); path.add_square(sq->e); }
      if ((sq->gap_w->state == Square::YES) && (sq->w != 0))
      { candidates.remove(sq->w); path.add_square(sq->w); }
    }
    paths->push_back(path);
  }
  return paths;
}

bool Board::solve_no_loops() {
  std::list< Board::Path > *paths = this->find_all_paths();
  bool changes = false;

#ifdef DEBUG
  std::cout << "solve_no_loops()" << std::endl;
#endif

  for (auto path: *paths) {
    // Given a Path, if any two Squares on that Path are adjacent but not along
    // the track, the Gap between them must be set to NO.
#ifdef DEBUG
#ifdef VERBOSE
    std::cout << "  run path" << std::endl;
#endif
#endif
    for (auto i = path.begin(); i != path.end(); i++) {
#ifdef DEBUG
#ifdef VERBOSE
      std::cout << "    work " << std::hex << *i << std::endl;
#endif
#endif
      auto j = i; j++;
      for (; j != path.end(); j++) {
        if (((*i)->s == *j) && ((*i)->gap_s->state == Square::UNKN)) { 
#ifdef DEBUG
          std::cout << std::hex << "  " << *i << " S " << *j << std::endl;
#endif
          (*i)->gap_s->state = Square::NO; 
          changes = true; 
        }
        if (((*i)->w == *j) && ((*i)->gap_w->state == Square::UNKN)) { 
#ifdef DEBUG
          std::cout << std::hex << "  " << *i << " W " << *j << std::endl;
#endif
          (*i)->gap_w->state = Square::NO; 
          changes = true; 
        }
        if (((*i)->n == *j) && ((*i)->gap_n->state == Square::UNKN)) { 
#ifdef DEBUG
          std::cout << std::hex << "  " << *i << " N " << *j << std::endl;
#endif
          (*i)->gap_n->state = Square::NO; 
          changes = true; 
        }
        if (((*i)->e == *j) && ((*i)->gap_e->state == Square::UNKN)) { 
#ifdef DEBUG
          std::cout << std::hex << "  " << *i << " E " << *j << std::endl;
#endif
          (*i)->gap_e->state = Square::NO; 
          changes = true; 
        }
      }
    }
  }

  return changes;
}

bool Board::solve_dont_join_ends() {
  std::list< Board::Path > *paths = this->find_all_paths();
  Square *start_sq, *end_sq, *sq, *ps;
  Board::Path *start_path, *end_path;
  bool changes = false;

  // If there are only two paths, then they are the start and end paths, and
  // they *should* be connected. So stop checking.
  if (paths->size() == 2) { return false; }

#ifdef DEBUG
  std::cout << "solve_dont_join_ends()" << std::endl;
#endif

  // Find the start and end squares. The start square is along the W border and
  // the end square is along the S border, so we only need to iterate over
  // those lines.
  ps = 0;
  for (sq = squares; sq != 0; sq = sq->s) {
    ps = sq; // make sure we keep track of the SW-most square
    if ((sq->get_state() == Square::YES) &&
        (sq->gap_w->state == Square::YES))
      start_sq = sq;
  }
  for (sq = ps; sq != 0; sq = sq->e) {
    if ((sq->get_state() == Square::YES) &&
        (sq->gap_s->state == Square::YES))
      end_sq = sq;
  }

  // If we hadn't needed to count every path above, then we could more
  // efficiently take the start and end square and perform path discovery on
  // them in order to build the start and end paths. But we already have them,
  // so instead let's just find them in the list of paths, even though that's
  // way less optimal. (FIXME)
  start_path = 0; end_path = 0;
  for (auto it = paths->begin(); it != paths->end(); it++) {
    if (it->contains_square(start_sq)) start_path = &(*it);
    if (it->contains_square(end_sq)) end_path = &(*it);
  }
  if ((start_path == 0) || (end_path == 0)) {
    std::cerr << "Error: couldn't find start or end path" << std::endl;
    exit(1);
  }

  for (auto i: *start_path) {
    for (auto j: *end_path) {
      // if *sit and *eit are neighbors, set the gap to NO
      // FIXME: refactor
      if ((i->s == j) && (i->gap_s->state == Square::UNKN)) { 
#ifdef DEBUG
        std::cout << std::hex << "  " << i << " S " << j << std::endl;
#endif
        i->gap_s->state = Square::NO; 
        changes = true; 
      }
      if ((i->w == j) && (i->gap_w->state == Square::UNKN)) { 
#ifdef DEBUG
        std::cout << std::hex << "  " << i << " W " << j << std::endl;
#endif
        i->gap_w->state = Square::NO; 
        changes = true; 
      }
      if ((i->n == j) && (i->gap_n->state == Square::UNKN)) { 
#ifdef DEBUG
        std::cout << std::hex << "  " << i << " N " << j << std::endl;
#endif
        i->gap_n->state = Square::NO; 
        changes = true; 
      }
      if ((i->e == j) && (i->gap_e->state == Square::UNKN)) { 
#ifdef DEBUG
        std::cout << std::hex << "  " << i << " E " << j << std::endl;
#endif
        i->gap_e->state = Square::NO; 
        changes = true; 
      }
    }
  }

  return changes;
}

bool Board::solve_unreachable_spaces() {
  std::list< Square* > reachable;
  std::list< Square* >::iterator p, q;
  Square *rp, *sq;
  bool changes = false;

#ifdef DEBUG
  std::cout << "solve_unreachable_spaces()" << std::endl;
#endif

  // All YES Squares are reachable.
  for (rp = squares; rp != 0; rp = rp->s) {
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::YES) {
        reachable.push_back(sq);
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
  for (p = reachable.begin(); p != reachable.end(); p++) {
    Square* neighbors[4];
    neighbors[0] = (*p)->w;
    neighbors[1] = (*p)->n;
    neighbors[2] = (*p)->e;
    neighbors[3] = (*p)->s;
    for (int i = 0; i < 4; i++) {
      sq = neighbors[i];
      if ((sq != 0) && (sq->get_state() == Square::UNKN)) {
        bool found = false;
        for (q = reachable.begin(); q != reachable.end(); q++) {
          if (*q == sq) { 
            found = true; 
            break; 
          }
        }
        if (!found) {
          reachable.push_back(sq);
        }
      }
    }
  }

  // All UNKN Squares that aren't on the reachable list are unreachable; 
  // mark them NO.
  int x, y;
  y = 0;
  for (rp = squares; rp != 0; rp = rp->s) {
    x = 0;
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::UNKN) {
        bool found = false;
        for (q = reachable.begin(); q != reachable.end(); q++) {
          if (*q == sq) { found = true; break; }
        }
        if ((!found) && (sq->set_state(Square::NO))) {
#ifdef DEBUG
          std::cout << "  mark (" << x << "," << y << ")" << std::endl;
#endif
          changes = true;
        }
      }
      x++;
    }
    y++;
  }

  return changes;
}

void Board::solve() {
  bool changes = true;
  while (changes) {
    changes = false;

#ifdef DEBUG
    //std::cout << "----\n" << to_str();
    inspect_all_squares();
    //std::cout << std::endl;
#endif
    
    changes = changes || solve_forced_spaces();
    changes = changes || solve_full_count();
    changes = changes || solve_no_loops();
    changes = changes || solve_unreachable_spaces();
    changes = changes || solve_dont_join_ends();
  }
}
