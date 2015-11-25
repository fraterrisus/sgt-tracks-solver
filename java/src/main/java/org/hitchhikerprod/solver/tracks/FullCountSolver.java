package org.hitchhikerprod.solver.tracks;

import java.util.HashMap;

/**
 * Created by becordes on 11/25/15.
 */
public class FullCountSolver
    implements SolvingStrategy
{
  private FullCountSolver() { }
  private static final FullCountSolver INSTANCE = new FullCountSolver();
  public static FullCountSolver getInstance() { return INSTANCE; }

  @Override
  public boolean solve(Board b) {
    System.out.println("FullCountSolver.solve()");

    boolean changes = false;
    int i,j;

    for (j=0; j < b.getHeight(); j++) {
      HashMap<State, Integer> counts = countStates(b.getSquare(0,j), Direction.E);

      if (b.rowHints[j] == counts.get(State.YES)) {
        changes = setUnknownSquaresTo(State.NO, b.getSquare(0,j), Direction.E);
      }
      else if (b.getHeight() - b.rowHints[j] == counts.get(State.NO)) {
        changes = setUnknownSquaresTo(State.YES, b.getSquare(0,j), Direction.E);
      }
      else if (b.rowHints[j] - counts.get(State.YES) == 1) {
        changes = setUnknownGapsToNo(b.getSquare(0,j), Direction.E);
      }
    }

    for (i=0; i < b.getWidth(); i++) {
      HashMap<State, Integer> counts = countStates(b.getSquare(i,0), Direction.S);

      if (b.colHints[i] == counts.get(State.YES)) {
        changes = setUnknownSquaresTo(State.NO, b.getSquare(i,0), Direction.S);
      }
      else if (b.getHeight() - b.colHints[i] == counts.get(State.NO)) {
        changes = setUnknownSquaresTo(State.YES, b.getSquare(i,0), Direction.S);
      }
      else if (b.colHints[i] - counts.get(State.YES) == 1) {
        changes = setUnknownGapsToNo(b.getSquare(i,0), Direction.S);
      }
    }

    return changes;
  }

  private boolean setUnknownGapsToNo(Square start, Direction travel) {
    boolean changes = false;
    for (Square sq = start; sq.getSquare(travel) != null; sq = sq.getSquare(travel)) {
      if ((sq.getState() == State.UNKN) &&
          (sq.getSquare(travel).getState() == State.UNKN) &&
          (sq.getGapState(travel) != State.NO)) {
        sq.setGapState(travel, State.NO);
        changes = true;
      }
    }
    return changes;
  }

  private boolean setUnknownSquaresTo(State state, Square start, Direction travel) {
    boolean changes = false;
    for (Square sq = start; sq != null; sq = sq.getSquare(travel)) {
      if (sq.getState() == State.UNKN) {
        sq.setState(state);
        changes = true;
      }
    }
    return changes;
  }

  private HashMap<State, Integer> countStates(Square start, Direction travel) {
    HashMap<State, Integer> counts = new HashMap<>(State.values().length);
    for (State st: State.values()) counts.put(st, 0);
    for (Square sq = start; sq != null; sq = sq.getSquare(travel)) {
      State st = sq.getState();
      counts.put(st, counts.get(st) + 1);
    }
    return counts;
  }
}

