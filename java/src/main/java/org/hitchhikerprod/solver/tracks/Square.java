package org.hitchhikerprod.solver.tracks;

import java.util.*;

class Square
{
  private class Gap
  {
    private State state;
    public Gap() { state = State.UNKN; }
    public Gap(State st) { state = st; }
    public State getState() { return state; }
    public void setState(State st) { state = st; }
  }

  private Map<Direction, Gap> gaps;
  private Map<Direction, Square> neighbors;
  private State state;

  public Square() {
    gaps = new EnumMap<>(Direction.class);
    neighbors = new EnumMap<>(Direction.class);
    setState(State.UNKN);
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

  private void setMaybeGapsTo(State st) {
    if (st == State.UNKN) return;
    gaps.values().stream()
        .filter(gap -> gap.getState() == State.UNKN)
        .forEach(gap -> gap.setState(st));
  }

  /** Perform consistency checks on this Square, based on its current state and the state of its Gaps. May change the
   state of this Square and all Gaps. */
  private void updateState() {
    long ycount = gaps.values().stream()
        .filter(gap -> (gap.getState() == State.YES)).count();
    long ncount = gaps.values().stream()
        .filter(gap -> (gap.getState() == State.NO)).count();
    if (ycount > 0)
      setState(State.YES);
    if ((ycount == 2) && (ncount != 2))
      setMaybeGapsTo(State.NO);
    if ((state == State.YES) && (ncount == 2) && (ycount != 2))
      setMaybeGapsTo(State.YES);
    if (ncount == 3)
      gaps.values().stream().forEach(gap -> gap.setState(State.NO));
    if (ncount >= 3)
      setState(State.NO);
  }

  public State getState() { return state; }

  void setState(State state) {
    if (state == State.NO) {
      gaps.values().stream().forEach(gap -> gap.setState(State.NO));
    }
    this.state = state;
  }

  /** Marks this Square as a border square in the given Direction. This implies that there will be no neighbor in
   * that direction, and that the Gap in that direction should be marked NO. Should only be used during Board
   * initialization.
   * @param d Points towards the edge of the board
   */
  public void markBorder(Direction d) { gaps.put(d, new Gap(State.NO)); }

  private Gap getGap(Direction d) { return gaps.get(d); }

  public State getGapState(Direction d) { return getGap(d).getState(); }

  public void setGapState(Direction d, State s)
  {
    gaps.get(d).setState(s);
    this.updateState();
    neighbors.get(d).updateState();
  }

  public void setGapsState(Set<Direction> dirs, State s)
  {
    dirs.stream().forEach(d -> gaps.get(d).setState(s));
    this.updateState();
    dirs.stream().forEach(d -> {
      Square n = neighbors.get(d);
      if (n != null) n.updateState();
    });
  }

  public Square getSquare(Direction d) { return neighbors.get(d); }

  public void setSquare(Direction d, Square neigh)
  {
    Direction e = d.reverse();
    Gap h;

    // Set my neighbor(d) to the parameter
    neighbors.put(d, neigh);
    // If my new neighbor(d) already has a gap(!d), set my gap(d) to it
    if ((h = neigh.getGap(e)) != null) gaps.put(d,h);
    // Otherwise create a new gap(d)
    else gaps.put(d, new Gap());
    // If my new neighbor(d) doesn't already know that I'm his neighbor(!d), ask him to set it.
    // Note this has to come last, so that I've initialized all of my state, because I'm about to call this same
    // function and I don't want to get stuck in a loop.
    if (neigh.getSquare(e) != this) neigh.setSquare(e, this);
  }

  public boolean isSolved() {
      // You can't stream a Map, but you can stream the Set of keys
    boolean unsolved = gaps.keySet().stream()
      .anyMatch(d -> gaps.get(d).getState() == State.UNKN);
    return ! unsolved;
  }
}
