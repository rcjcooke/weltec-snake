#ifndef __GAMEELEMENTS_H_INCLUDED__
#define __GAMEELEMENTS_H_INCLUDED__

#define LEVEL_GRID_WIDTH 30
#define LEVEL_GRID_HEIGHT 15

#include <Arduino.h>

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
  Down,
  Same
};

enum class GameState {
  MainMenu,
  Playing,
  Paused,
  Died,
  Won
};

class Location {
public:
  Location(uint8_t x, uint8_t y) : x(x), y(y) {}
  Location(const Location& loc) : Location(loc.x, loc.y) {}

  uint8_t x;
  uint8_t y;
};

#endif // __GAMEELEMENTS_H_INCLUDED__