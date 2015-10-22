#include "board.h"

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

/* ---------------------------------------- */

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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
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
#ifdef DEBUG
          else { std::cout << "."; }
#endif
        }
#ifdef DEBUG
        std::cout << std::endl;
#endif
      }
    }
  }
  return changes;
}

/* ---------------------------------------- */

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
        if (ungap_neighbors(*i, *j)) { changes = true; }
      }
    }
  }

  return changes;
}

/* ---------------------------------------- */

bool Board::solve_dont_join_ends() {
  //std::list< Board::Path > *paths = this->find_all_paths();
  Square *start_sq, *end_sq, *sq, *ps;
  Board::Path start_path, end_path;
  bool changes = false;

  // If there are only two paths, then they are the start and end paths, and
  // they *should* be connected. So stop checking.
  //if (paths->size() == 2) { return false; }
  // I'm going to ignore this check, based on the assumption that this is the
  // solver of last resort, and if another move could have been made, it was.

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

  start_path = find_path(start_sq);
  end_path = find_path(end_sq);

  for (auto i: start_path) {
    for (auto j: end_path) {
      if (ungap_neighbors(i,j)) { changes = true; }
    }
  }

  return changes;
}

/* ---------------------------------------- */

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
