package org.hitchhikerprod.solver.tracks;

public enum State 
{ 
  UNKN {
    char toChar() { return 'U'; }
    char toGlyph() { return '\u25e6'; }
  },

  YES {
    char toChar() { return 'Y'; }
    char toGlyph() { return '\u2022'; }
  },

  NO {
    char toChar() { return 'N'; }
    char toGlyph() { return ' '; }
  };

  abstract char toChar();
  abstract char toGlyph();
}
