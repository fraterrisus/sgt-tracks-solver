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
  cout << "Board size   : " << w << "x" << h << endl;

  Board *b = new Board(w,h);

  while (p != ':') { cin >> p; }
  cin >> p;
  while (p != ',') { gameid = gameid + p; cin >> p; }

  cout << "GameID       : " << gameid << endl;
  int pos = 0, val = 0;
  for (string::iterator it = gameid.begin(); it != gameid.end(); it++) {
    val = -1;
    if ((*it >= 'a') && (*it <= 'z')) {
      pos += *it - 'a';
    }
    if ((*it >= '1') && (*it <= '9')) {
      val = atoi(&(*it));
    }
    if ((*it >= 'A') && (*it <= 'Z')) {
      val = *it - 'A' + 10;
    }
    //cout << "(" << pos << ") " << *it;
    if (val != -1) { 
      //cout << " " << val;
      b->set_square(pos, val);
    }
    //cout << endl;
    pos++;
  }
  if (pos != (w * h)) {
    cerr << "Format error: GameID string not long enough" << endl;
    return 0;
  }
  
  cout << "Column counts: ";
  for (int i=0; i<w; i++) {
    // try to read a number
    cin >> n;
    // if that doesn't work, try reading a letter and then a number
    if (cin.fail()) { 
      cin.clear(); 
      cin >> p;
      if (p != 'S') {
        cout << endl;
        cerr << "Format error: expected 'S' or digit\n";
        return 0;
      }
      cin >> n;
    }
    cout << n;
    b->col_count[i] = n;
    cin >> p;
    if (p != ',') {
      cout << endl;
      cerr << "Format error: expected ,\n";
      return 0;
    }
  }
  cout << endl;

  cout << "Row counts   : ";
  for (int i=0; i<h; i++) {
    cin >> n;
    if (cin.fail()) { 
      cin.clear(); 
      cin >> p;
      if (p != 'S') {
        cout << endl;
        cerr << "Format error: expected 'S' or digit\n";
        return 0;
      }
      cin >> n;
    }
    cout << n;
    b->row_count[i] = n;
    if (i != h-1) {
      cin >> p;
      if (p != ',') {
        cout << endl;
        cerr << "Format error: expected ,\n";
        return 0;
      }
    }
  }
  cout << endl;

  return b;
}

int main(int argc, char **argv) {
  Board *board = read_game();
  if (board == 0) {
    return 1;
  }
  board->solve();
  cout << board->to_str() << std::endl;
  delete(board);
  return 0;
}
