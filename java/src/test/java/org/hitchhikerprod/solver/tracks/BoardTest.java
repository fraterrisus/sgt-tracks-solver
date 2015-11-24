package org.hitchhikerprod.solver.tracks;

import org.junit.Test;
import static org.junit.Assert.*;

public class BoardTest
{
  @Test
  public void testCreateBoard() {
    Board b = new Board(3,7);
    assertEquals(3, b.getWidth());
    assertEquals(7, b.getHeight());
  }
}
