package org.hitchhikerprod.solver.tracks;

import java.util.*;
import java.util.stream.*;

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
    gaps = new EnumMap<Direction, Gap>(Direction.class);
    neighbors = new EnumMap<Direction, Square>(Direction.class);
    state = State.UNKN;
  }

  public String toString() {
    if (state == State.NO) { return "\u00d7"; }
    if (state == State.UNKN) { return "\u25e6"; }
    return "?";
  }

  public State getState() { return state; }

  // TODO
  public boolean setState(State st) { state = st; return true; }

  public Gap nextGap(Direction d) { return gaps.get(d); }
  public void setGap(Direction d, Gap g) { gaps.put(d, g); }

  public Square nextSquare(Direction d) { return neighbors.get(d); }
  public void setNeighbor(Direction d, Square s) { neighbors.put(d, s); }

  public boolean updateState() {
    return true;
  }

  public boolean isSolved() {
    updateState();
    // You can't stream a Map, but you can stream the Set of keys
    boolean unsolved = gaps.keySet().stream()
      .anyMatch(d -> gaps.get(d).getState() == State.UNKN);
    return ! unsolved;
  }

}
