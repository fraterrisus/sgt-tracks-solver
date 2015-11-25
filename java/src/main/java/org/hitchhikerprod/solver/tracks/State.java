package org.hitchhikerprod.solver.tracks;

public enum State 
{
  UNKN ('U', '\u25e6'),
  YES  ('Y', '\u2022'),
  NO   ('N', ' ');

  private final char ascii, glyph;
  State(char ascii, char glyph) { this.ascii = ascii; this.glyph = glyph; }
  public char toChar() { return this.ascii; }
  public char toGlyph() { return this.glyph; }
}
