#ifndef __GAMEELEMENTS_H_INCLUDED__
#define __GAMEELEMENTS_H_INCLUDED__

#include <stdint.h>

#define LEVEL_GRID_WIDTH 64
#define LEVEL_GRID_HEIGHT 28

enum class GameSquare {
  Empty,
  Wall,
  Snake,
  Coffee
};

enum class Direction {
  Left,
  Right,
  Up,
  Down
};

enum class GameState {
  MainMenu,
  Playing,
  Paused,
  Died,
  WonLevel,
  WonGame
};

const char* translateGameSquare(GameSquare gs){
  switch (gs) {
    case GameSquare::Coffee: return "Coffee";
    case GameSquare::Empty: return "Empty";
    case GameSquare::Wall: return "Wall";
    case GameSquare::Snake: return "Snake";
  }
  return nullptr;
}

class Location {
public:
  Location() {};
  Location(uint8_t x, uint8_t y) : x(x), y(y) {}
  Location(const Location& loc) : Location(loc.x, loc.y) {}

  // Must pass an allocated bugger to this function. Buffer should be at least 10 characters wide.
  void toString(char* ret) {
    sprintf(ret, "(%d,%d)", x, y);
  }

  bool operator== (const Location& l) {return x == l.x && y == l.y;}
  bool operator!= (const Location& l) {return !(*this == l);}

  uint8_t x;
  uint8_t y;
};

#endif // __GAMEELEMENTS_H_INCLUDED__