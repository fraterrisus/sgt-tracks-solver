#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <stdlib.h>

#include "board.h"

using namespace std;

Board* read_game() {
  int h, w, n;
  char p;
  string gameid;

  cin >> w;
  cin >> p;
  if (p != 'x') {
    cerr << "Format error: expected (width)x(height)\n";
    return 0;
  }
  cin >> h;
#ifdef DEBUG
  cout << "Board size   : " << w << "x" << h << endl;
#endif

  Board *b = new Board(w,h);

  while (p != ':') { cin >> p; }
  cin >> p;
  while (p != ',') { gameid = gameid + p; cin >> p; }

#ifdef DEBUG
  cout << "GameID       : " << gameid << endl;
#endif
  int pos = 0, val = 0;
  for (char &c : gameid) {
    val = -1;
    if ((c >= 'a') && (c <= 'z')) {
      pos += c - 'a';
    }
    if ((c >= '1') && (c <= '9')) {
      val = c - '0';
    }
    if ((c >= 'A') && (c <= 'Z')) {
      val = c - 'A' + 10;
    }
#ifdef DEBUG
#ifdef VERBOSE
    cout << "(" << pos << ") " << c;
#endif
#endif
    if (val != -1) { 
#ifdef DEBUG
#ifdef VERBOSE
      cout << " " << val;
#endif
#endif
      b->set_square(pos, val);
    }
#ifdef DEBUG
#ifdef VERBOSE
    cout << endl;
#endif
#endif
    pos++;
  }
  if (pos != (w * h)) {
    cerr << "Format error: GameID string not long enough" << endl;
    return 0;
  }
  
#ifdef DEBUG
  cout << "Column counts: ";
#endif
  for (int i=0; i<w; i++) {
    // try to read a number
    cin >> n;
    // if that doesn't work, try reading a letter and then a number
    if (cin.fail()) { 
      cin.clear(); 
      cin >> p;
      if (p != 'S') {
#ifdef DEBUG
        cout << endl;
#endif
        cerr << "Format error: expected 'S' or digit\n";
        return 0;
      }
      cin >> n;
    }
#ifdef DEBUG
    cout << n;
#endif
    b->col_count[i] = n;
    cin >> p;
    if (p != ',') {
#ifdef DEBUG
      cout << endl;
#endif
      cerr << "Format error: expected ,\n";
      return 0;
    }
  }
#ifdef DEBUG
  cout << endl;

  cout << "Row counts   : ";
#endif
  for (int i=0; i<h; i++) {
    cin >> n;
    if (cin.fail()) { 
      cin.clear(); 
      cin >> p;
      if (p != 'S') {
#ifdef DEBUG
        cout << endl;
#endif
        cerr << "Format error: expected 'S' or digit\n";
        return 0;
      }
      cin >> n;
    }
#ifdef DEBUG
    cout << n;
#endif
    b->row_count[i] = n;
    if (i != h-1) {
      cin >> p;
      if (p != ',') {
#ifdef DEBUG
        cout << endl;
#endif
        cerr << "Format error: expected ,\n";
        return 0;
      }
    }
  }
#ifdef DEBUG
  cout << endl;
#endif

  return b;
}

int main(int argc, char **argv) {
  Board *board = read_game();
  if (board == 0) {
    return 1;
  }
  cout << board->to_str() << endl;
  for (int i=0; i<3; i++) {
    Board *temp = new Board(*board);
    cout << temp->to_str() << endl;
  }
  /*
  auto then = chrono::high_resolution_clock::now();
  board->solve();
  auto now = chrono::high_resolution_clock::now();
  long long elapsed_us = chrono::duration_cast<std::chrono::microseconds>(now - then).count();
  cout << board->to_str() << endl;
  cout << "Solving time: " << elapsed_us / 100000.0 << endl;
  */
  delete(board);
  return 0;
}
