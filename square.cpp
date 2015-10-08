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
  std::cout << "delete square " << std::hex << this << std::endl;
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

std::string Square::state_to_str(State st) {
  switch(st) {
    case Square::YES:  return "Y";
    case Square::NO:   return "N";
    case Square::UNKN: return "U";
    default:           return "?";
  }
}

std::string Square::to_str() {
  if (state == NO) { return "\u00d7"; }
  if (state == UNKN) { return "\u25e6"; }
  switch(get_yeses().to_ulong()) {
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
  update_state();
  if (gap_s->state == Square::UNKN) { return false; }
  if (gap_n->state == Square::UNKN) { return false; }
  if (gap_e->state == Square::UNKN) { return false; }
  if (gap_w->state == Square::UNKN) { return false; }
  return true;
}

/* This method is not additive. 1s will be set to YES and 0s will be set 
 * to NO. */
bool Square::set_value(std::bitset<4> val) {
  gap_s->state = (val[3]) ? Square::YES : Square::NO;
  gap_w->state = (val[2]) ? Square::YES : Square::NO;
  gap_n->state = (val[1]) ? Square::YES : Square::NO;
  gap_e->state = (val[0]) ? Square::YES : Square::NO;
  state = (val.count() > 0) ? Square::YES : Square::NO;
  return true;
}

/*
bool Square::add_value(int val) {
  int edges = get_yeses();
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
*/

bool Square::update_state() {
  std::bitset<4> y = get_yeses();
  std::bitset<4> n = get_noes();
  bool update = false;
  if (y.count() > 0) {
    if (set_state(YES)) { update = true; }
  }
  // (Solved) If two edges are YES, then we are YES and the other two edges are NO
  if ((y.count() == 2) && (n.count() != 2)) {
    if (set_value(y)) { update = true; }
  }
  // (Forced) If we are YES but two edges are NO, then the other two edges must be YES
  if ((state == YES) && (y.count() != 2) && (n.count() == 2)) {
    n.flip();
    if (set_value(n)) { update = true; }
  }
  // (Alleys) If three edges are NO, then we are NO and all four edges are NO
  if (n.count() == 3) {
    n.reset(); // n.set().flip();
    if (set_value(n)) { update = true; }
  }
  if (n.count() == 4) {
    if (set_state(NO)) { update = true; }
  }
  return update;
}

bool Square::set_state(State st) {
  switch (st) {
    case YES:
      if (state == YES)
        return false;
      else {
        state = YES;
        return true;
      }
    case UNKN:
      return false;
    case NO:
      if ((state == NO) &&
          (gap_s->state == NO) &&
          (gap_n->state == NO) &&
          (gap_e->state == NO) &&
          (gap_w->state == NO)) {
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

std::bitset<4> Square::get_yeses() {
  std::bitset<4> yeses;
  yeses.reset();
  if (gap_s->state == YES) { yeses.set(3, true); }
  if (gap_w->state == YES) { yeses.set(2, true); }
  if (gap_n->state == YES) { yeses.set(1, true); }
  if (gap_e->state == YES) { yeses.set(0, true); }
  return yeses;
}

std::bitset<4> Square::get_noes() {
  std::bitset<4> noes;
  noes.reset();
  if (gap_s->state == NO) { noes.set(3, true); }
  if (gap_w->state == NO) { noes.set(2, true); }
  if (gap_n->state == NO) { noes.set(1, true); }
  if (gap_e->state == NO) { noes.set(0, true); }
  return noes;
}
