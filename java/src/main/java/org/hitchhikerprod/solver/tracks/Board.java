package org.hitchhikerprod.solver.tracks;

import java.io.BufferedReader;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class Board 
{
  // -------------------- Fields
  public int rowHints[], colHints[];
  private int wid, hgt;
  private List<Square> squares;

  // -------------------- Getters / Setters
  public int getWidth() { return wid; }
  public int getHeight() { return hgt; }

  /** Getter method for Squares, which should be referenced by X/Y coordinates.
   * @param x
   * @param y
   * @return a Square object
   * @throws IndexOutOfBoundsException
   */
  public Square getSquare(int x, int y)
      throws IndexOutOfBoundsException
  {
    if ((x < 0) || (x >= wid))
      throw new IndexOutOfBoundsException("Illegal value for x (" + x + ")");
    if ((y < 0) || (y >= hgt))
      throw new IndexOutOfBoundsException("Illegal value for y (" + y + ")");
    return squares.get(x + (y * wid));
  }

  // -------------------- Constructors / Factories
  public Board(int width, int height)
  {
    wid = width;
    hgt = height;
    colHints = new int[wid];
    rowHints = new int[hgt];
    squares = new ArrayList<>();

    for (int i=0; i < wid*hgt; i++) {
      squares.add(new Square());
    }
    connectGrid();
  }

  /** Reads a game from a BufferedReader, which should produce a one-line "game ID" as returned from the sgt-tracks
   * executable using the "--generate" command line switch.
   * @param cin The BufferedReader that contains the game ID string.
   * @return a new Board object, initialized with the input data.
   */
  public static Board getBoardFromGame(BufferedReader cin)
  {
    Pattern gamespec_pat = Pattern.compile("\\A(\\d+)x(\\d+)[^:]*:([^,]*),([0-9S,]*)\\z");
    Matcher gamespec_mat;

    try {
      gamespec_mat = gamespec_pat.matcher(cin.readLine());
      cin.close();

      if (! gamespec_mat.matches()) {
        System.err.println("Error: couldn't parse gameID string");
        System.err.println("Format error");
        return null;
      }
    }
    catch (java.io.IOException e) {
      System.err.println("Error: couldn't parse gameID string");
      System.err.println("Caught IO exception: " + e.getMessage());
      return null;
    }

    int w = Integer.parseInt( gamespec_mat.group(1) );
    int h = Integer.parseInt( gamespec_mat.group(2) );
    String gameid = gamespec_mat.group(3);
    String counts = gamespec_mat.group(4);

    System.out.format("Board size: " + w + " x " + h + "\n");
    System.out.println("Game ID   : " + gameid);
    System.out.println("Counts    : " + counts);

    Board b = new Board(w,h);

    try {
      List<Integer> icounts = new ArrayList<>();
      Scanner s = new Scanner(counts).useDelimiter(",S?");
      while (s.hasNextInt()) icounts.add(s.nextInt());
      b.loadCounts(icounts);
    }
    catch (IndexOutOfBoundsException e) {
      System.err.println("Error: couldn't parse gameID string");
      System.err.println("Wrong number of row or column counts: " + e.getMessage());
      return null;
    }

    try {
      b.loadSquareState(gameid);
    }
    catch (IndexOutOfBoundsException e) {
      System.err.println("Error: couldn't parse gameID string");
      System.err.println("Initial state doesn't match board size: " + e.getMessage());
      return null;
    }

    return b;
  }

  // -------------------- Private Methods
  private void connectGrid()
  {
    // Establish N-S relationships
    for (int y=0; y<hgt-1; y++) {
      for (int x=0; x<wid; x++) {
        getSquare(x,y).setSquare(Direction.S, getSquare(x,y+1));
      }
    }

    // Establish E-W relationships
    for (int x=0; x<wid-1; x++) {
      for (int y=0; y<hgt; y++) {
        getSquare(x,y).setSquare(Direction.E, getSquare(x+1,y));
      }
    }

    // Manage border
    for (int x=0; x<wid; x++) {
      getSquare(x, 0).markBorder(Direction.N);
      getSquare(x, hgt-1).markBorder(Direction.S);
    }

    for (int y=0; y<hgt; y++) {
      getSquare(0, y).markBorder(Direction.W);
      getSquare(wid - 1, y).markBorder(Direction.E);
    }
  }

  private void loadSquareState(String gameid)
      throws IndexOutOfBoundsException
  {
    assert ((gameid != null) && (gameid.length() > 0));

    int pos=0, val;
    for (char c: gameid.toCharArray()) {
      val = -1;
      //System.out.print(pos + ":" + c);
      if ((c >= 'a') && (c <= 'z')) {
        int cpos = (int) c - (int) 'a';
        //System.out.print(" pos += " + cpos);
        pos += cpos;
      }
      if ((c >= '1') && (c <= '9')) {
        val = (int) c - (int) '0';
        //System.out.print(" val = " + val);
      }
      if ((c >= 'A') && (c <= 'Z')) {
        val = (int) c - (int) 'A' + 10;
        //System.out.print(" val = " + val);
      }
      //System.out.print("\n");
      if (val != -1) {
        Square square = squares.get(pos);
        square.setGapsState(Direction.intToSet(val), State.YES);
      }
      pos++;
    }
    if (pos != wid * hgt) throw new IndexOutOfBoundsException("ended at " + pos);
  }

  private void loadCounts(List<Integer> counts)
      throws IndexOutOfBoundsException
  {
    assert ((counts != null) && (counts.size() > 0));

    ListIterator<Integer> it = counts.listIterator();
    for (int y=0; y<wid; y++) {
      if (!it.hasNext()) throw new IndexOutOfBoundsException(y + " columns needed");
      colHints[y] = it.next();
    }
    for (int x=0; x<wid; x++) {
      if (!it.hasNext()) throw new IndexOutOfBoundsException(x + " rows needed");
      rowHints[x] = it.next();
    }
  }

  // -------------------- Public Methods
  public String toString()
  {
    StringBuilder sb = new StringBuilder();
    int x, y;

    for (x=0; x<wid; x++) {
      sb.append(String.format("%d", colHints[x]));
    }
    sb.append("\n");
    for (y=0; y<hgt; y++) {
      //sb.append(" ");
      for (x=0; x<wid; x++) {
        sb.append(getSquare(x,y).toString());
        //sb.append(" ");
      }
      sb.append(rowHints[y]);
      sb.append("\n");
    }
    return sb.toString();
  }

  public void solve() {
    Set<SolvingStrategy> solvers = new HashSet<>();
    solvers.add(FullCountSolver.getInstance());

    boolean changes = true;
    while (changes) {
      changes = false;
      solvers.stream().anyMatch(s -> s.solve(this));
      System.out.println(this);
    }
  }
}
