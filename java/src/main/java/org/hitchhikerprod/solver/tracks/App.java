package org.hitchhikerprod.solver.tracks;

import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.InputStreamReader;
import java.io.BufferedReader;

public class App 
{
  public static void main(String[] args) {
    BufferedReader reader;
    if (args.length == 0) {
      reader = new BufferedReader(new InputStreamReader(System.in));
    } else {
      FileReader infile = null;
      try {
        infile = new FileReader(args[0]);
      } catch (FileNotFoundException e) {
        System.err.println("File not found: " + args[0]);
        System.exit(1);
      }
      reader = new BufferedReader(infile);
    }
    Board b = Board.getBoardFromGame(reader);
    if (b == null) System.exit(1);
    System.out.println(b);
  }
}
