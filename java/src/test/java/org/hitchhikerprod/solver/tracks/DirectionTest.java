package org.hitchhikerprod.solver.tracks;

import org.junit.Test;

import static org.junit.Assert.*;

/**
 * Created by becordes on 11/25/15.
 */
public class DirectionTest {

  @Test
  public void testReverse() throws Exception {
    assert (Direction.N.reverse() == Direction.S);
    assert (Direction.S.reverse() == Direction.N);
    assert (Direction.E.reverse() == Direction.W);
    assert (Direction.W.reverse() == Direction.E);
  }
}