#include "board.h"

bool Board::Path::contains_square(Square *sq) {
  for (auto p = begin(); p != end(); p++) { if (*p == sq) { return true; } }
  return false;
}

void Board::Path::add_square(Square* sq) {
  if (! this->contains_square(sq)) { this->push_back(sq); }
}

/** Returns a Path, an *unordered* list of Squares that are all connected. A
 * Square belongs on this path if it is a neighbor of another Square on the
 * Path; two Squares are neighbors if they are joined by a YES Gap.
 *
 * This method iterates over Squares on the Path (starting with the method
 * argument), adding neighbors that are not already on the Path. When it runs
 * out of new Squares to add, it stops. */
Board::Path Board::find_path(Square *sq) {
  Board::Path path;

#ifdef DEBUG
#ifdef VERBOSE
  std::cout << "find_path(" << std::hex << sq << ")" << std::endl;
#endif
#endif

  path.push_back(sq);
  for (auto it = path.begin(); it != path.end(); it++) {
    Square *ptr = *it;
#ifdef DEBUG
#ifdef VERBOSE
    std::cout << "  square " << std::hex << ptr << std::endl;
#endif
#endif
    if ((ptr->n != 0) && (ptr->gap_n->state == Square::YES)) { path.add_square(ptr->n); }
    if ((ptr->s != 0) && (ptr->gap_s->state == Square::YES)) { path.add_square(ptr->s); }
    if ((ptr->e != 0) && (ptr->gap_e->state == Square::YES)) { path.add_square(ptr->e); }
    if ((ptr->w != 0) && (ptr->gap_w->state == Square::YES)) { path.add_square(ptr->w); }
  }
  return path;
}

/** Finds all Paths in the current Board state. Generates a worklist from all
 * Squares whose current state is YES, then attempts to generate a Path for
 * each one. Once a Path is generated, all Squares on that Path are removed
 * from the worklist in order to avoid duplicates. */
std::list< Board::Path >* Board::find_all_paths() {
  std::list< Board::Path > *paths = new std::list< Board::Path >;
  std::list< Square* > worklist;
  Square *rp, *sq;

  for (rp = squares; rp != 0; rp = rp->s) {
    for (sq = rp; sq != 0; sq = sq->e) {
      if (sq->get_state() == Square::YES) {
        worklist.push_back(sq);
      }
    }
  }

  while (!worklist.empty()) {
    paths->push_back(find_path(worklist.front())); 
    for (Square *s: paths->back()) { worklist.remove(s); }
  }

  return paths;
}
