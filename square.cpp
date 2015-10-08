#include "square.h"

Square::Square() {
  gap_n = 0;
  gap_s = 0;
  gap_e = 0;
  gap_w = 0;
  n = 0;
  s = 0;
  e = 0;
  w = 0;
}

/* When deleting this Square, also delete any associated Gaps and clear the
 * pointers. If this Square has neighbors, and those neighbors are linked to
 * the same Gaps that I am, then clear their pointers to the Gaps I'm about to
 * delete. */
Square::~Square() {
  if (gap_n != 0) {
    if ((n != 0) && (n->gap_s == gap_n)) { n->gap_s = 0; }
    delete(gap_n); gap_n = 0;
  }

  if (gap_e != 0) {
    if ((e != 0) && (e->gap_w == gap_e)) { e->gap_w = 0; }
    delete(gap_e); gap_e = 0;
  }

  if (gap_s != 0) {
    if ((s != 0) && (s->gap_n == gap_s)) { s->gap_n = 0; }
    delete(gap_s); gap_s = 0;
  }

  if (gap_w != 0) {
    if ((w != 0) && (w->gap_e == gap_w)) { w->gap_e = 0; }
    delete(gap_w); gap_w = 0;
  }
}

std::string Square::to_str() {
  int edges = get_edges();
  if (state == NO) { return "\u00d7"; }
  if (state == UNKN) { return "\u25e6"; }
  switch(edges) {
    case 0: return "\u2022";
    case 1: return "\u2576"; //"\u257a"
    case 2: return "\u2575"; //"\u2579"
    case 3: return "\u2570"; //"\u2517"
    case 4: return "\u2574"; //"\u2578"
    case 5: return "\u2500"; //"\u2501"
    case 6: return "\u256f"; //"\u251b"
    case 8: return "\u2577"; //"\u257b"
    case 9: return "\u256d"; //"\u250f"
    case 10: return "\u2502"; //"\u2503"
    case 12: return "\u256e"; //"\u2513"
  }
  return "?";
}

bool Square::is_solved() {
  if (gap_s->state == Square::UNKN) { return false; }
  if (gap_n->state == Square::UNKN) { return false; }
  if (gap_e->state == Square::UNKN) { return false; }
  if (gap_w->state == Square::UNKN) { return false; }
  return true;
}

/* This method is not additive. 1s will be set to YES and 0s will be set 
 * to NO. */
/*
bool Square::set_value(int val) {
  gap_s->state = ((val & SOUTH) > 0) ? Square::YES : Square::NO;
  gap_n->state = ((val & NORTH) > 0) ? Square::YES : Square::NO;
  gap_e->state = ((val & EAST ) > 0) ? Square::YES : Square::NO;
  gap_w->state = ((val & WEST ) > 0) ? Square::YES : Square::NO;
  return true;
}
*/

bool Square::set_value(std::bitset<4> val) {
  gap_s->state = (val[3]) ? Square::YES : Square::NO;
  gap_w->state = (val[2]) ? Square::YES : Square::NO;
  gap_n->state = (val[1]) ? Square::YES : Square::NO;
  gap_e->state = (val[0]) ? Square::YES : Square::NO;
  return true;
}

bool Square::add_value(int val) {
  int edges = get_edges();
  int newedges = edges | val;
  if (edges == newedges) { return true; }
  // Illegal states: 0111 1011 1101 1110 1111
  if ((newedges == 7) || (newedges == 11) || (newedges == 13) || 
      (newedges == 14) || (newedges == 15)) {
    return false;
  }
  // This is possibly redundant, but that's okay.
  if ((newedges & SOUTH) > 0) { gap_s->state = Square::YES; }
  if ((newedges & NORTH) > 0) { gap_n->state = Square::YES; }
  if ((newedges & EAST ) > 0) { gap_e->state = Square::YES; }
  if ((newedges & WEST ) > 0) { gap_w->state = Square::YES; }
  return true;
}

bool Square::set_state(State st) {
  switch (st) {
    case YES:
      state = YES;
      return true;
    case UNKN:
      return false;
    case NO:
      if ((gap_s->state == YES) ||
          (gap_n->state == YES) ||
          (gap_e->state == YES) ||
          (gap_w->state == YES)) {
        return false;
      } else {
        state = NO;
        gap_s->state = NO;
        gap_n->state = NO;
        gap_e->state = NO;
        gap_w->state = NO;
        return true;
      }
  }
  // Something seriously weird happened
  return false;
}

int Square::get_edges() {
  int edges = 0;
  if (gap_s->state == YES) { edges += SOUTH; state = YES; }
  if (gap_w->state == YES) { edges += WEST;  state = YES; }
  if (gap_n->state == YES) { edges += NORTH; state = YES; }
  if (gap_e->state == YES) { edges += EAST;  state = YES; }
  return edges;
}

int Square::get_noes() {
  int edges = 0;
  if (gap_s->state == NO) { edges += SOUTH; }
  if (gap_w->state == NO) { edges += WEST;  }
  if (gap_n->state == NO) { edges += NORTH; }
  if (gap_e->state == NO) { edges += EAST;  }
  return edges;
}
