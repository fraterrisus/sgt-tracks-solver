package org.hitchhikerprod.solver.tracks;

import java.util.Set;
import java.util.EnumSet;

public enum Direction
{
  N(2) { Direction reverse() { return S; } },
  S(8) { Direction reverse() { return N; } },
  E(1) { Direction reverse() { return W; } },
  W(4) { Direction reverse() { return E; } };

  private final int bit;
  Direction(int bit) { this.bit = bit; }
  int getVal() { return this.bit; }
  abstract Direction reverse();

  public static Set<Direction> intToSet(int value)
  {
    EnumSet<Direction> set = EnumSet.noneOf(Direction.class);
    for (Direction d: Direction.values()) {
      if ((value & d.getVal()) > 0) set.add(d);
    }
    return set;
  }
}

