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

  @Test(expected=IndexOutOfBoundsException.class)
  public void testGetSquareXTooLarge() {
    Board b = new Board(3,7);
    b.getSquare(4,5);
  }

  @Test(expected=IndexOutOfBoundsException.class)
  public void testGetSquareXTooSmall() {
    Board b = new Board(3,7);
    b.getSquare(-1,5);
  }

  @Test(expected=IndexOutOfBoundsException.class)
  public void testGetSquareYTooLarge() {
    Board b = new Board(3,7);
    b.getSquare(2,10);
  }

  @Test(expected=IndexOutOfBoundsException.class)
  public void testGetSquareYTooSmall() {
    Board b = new Board(3,7);
    b.getSquare(2,-5);
  }

}
