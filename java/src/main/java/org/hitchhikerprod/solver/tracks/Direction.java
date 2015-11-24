package org.hitchhikerprod.solver.tracks;

import java.util.Set;
import java.util.EnumSet;

public enum Direction
{
  N(2), S(8), E(1), W(2);

  private final int bit;
  Direction(int bit) { this.bit = bit; }
  public int getVal() { return this.bit; }

  public static Set<Direction> intToSet(int value)
  {
    EnumSet<Direction> set = EnumSet.noneOf(Direction.class);
    for (Direction d: Direction.values()) {
      if ((value & d.getVal()) > 0) set.add(d);
    }
    return set;
  }
}

