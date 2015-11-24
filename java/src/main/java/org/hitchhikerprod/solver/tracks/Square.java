package org.hitchhikerprod.solver.tracks;

import java.util.*;

class Square
{
  class Gap
  {
    private State state;
    public Gap() { state = State.UNKN; }
    public State getState() { return state; }
    public void setState(State st) { state = st; }
  }

  private Map<Direction, Gap> gaps;
  private Map<Direction, Square> neighbors;
  private State state;

  public Square() {
    gaps = new EnumMap<>(Direction.class);
    neighbors = new EnumMap<>(Direction.class);
    state = State.UNKN;
  }

  public String toString() {
    if (state == State.NO) { return "\u00d7"; }
    if (state == State.UNKN) { return "\u25e6"; }

    int intval = 0;
    for (Direction d: Direction.values()) {
      if (gaps.get(d).getState() == State.YES) intval += d.getVal();
    }

    switch (intval) {
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

  public State getState() { return state; }
  // TODO
  public boolean setState(State st) { state = st; return true; }

  public Gap getGap(Direction d) { return gaps.get(d); }
  public void setGap(Direction d, Gap g) { gaps.put(d, g); }
  public void setGapState(Direction d, State s) { gaps.get(d).setState(s);}
  public void setGapsState(Set<Direction> dirs, State s) { dirs.stream().forEach(d -> setGapState(d, s)); }

  public Square getSquare(Direction d) { return neighbors.get(d); }
  public void setSquare(Direction d, Square s) { neighbors.put(d, s); }

  public boolean isSolved() {
      // You can't stream a Map, but you can stream the Set of keys
    boolean unsolved = gaps.keySet().stream()
      .anyMatch(d -> gaps.get(d).getState() == State.UNKN);
    return ! unsolved;
  }
}
