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
  int edges = 0;
  if (gap_s->state == YES) { edges += 8; state = YES; }
  if (gap_w->state == YES) { edges += 4; state = YES; }
  if (gap_n->state == YES) { edges += 2; state = YES; }
  if (gap_e->state == YES) { edges += 1; state = YES; }
  if (state == NO) { return "\u00d7"; }
  if (state == UNKN) { return "\u25e6"; }
  switch(edges) {
    case 0: return "\u2022";
    case 1: return "\u2574"; //"\u257a"
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
