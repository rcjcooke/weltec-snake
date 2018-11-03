#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include "GameElements.hpp"

class Level {

public:

  Level() {}
  ~Level() {
    for (int i=0; i<LEVEL_GRID_WIDTH; i++) delete mLevelGrid[i];
    delete mLevelGrid;
    delete mInitialSnake;
  }

  void setup(const char** levelMap, Direction startDirection, uint8_t coffeeTarget) {
    mLevelStartDirection = startDirection;
    mCoffeeTarget = coffeeTarget;
    // Initialise arrays
    mInitialSnake = new Location[2];
    mLevelGrid = new GameSquare*[LEVEL_GRID_WIDTH];
    for (int i=0; i<LEVEL_GRID_WIDTH; i++) mLevelGrid[i] = new GameSquare[LEVEL_GRID_HEIGHT];
    // Populate the level
    for (int y = 0; y < LEVEL_GRID_HEIGHT; y++) {
      for (int x = 0; x < LEVEL_GRID_WIDTH; x++) {
        if (DEBUG) Serial.printf("%d,%d: %c\r\n", x, y, levelMap[y][x]);
        if (levelMap[y][x] == 'W') {
          if (DEBUG) Serial.println("Building Wall");
          mLevelGrid[x][y] = GameSquare::Wall;
        } else if (levelMap[y][x] == ' ') {
          if (DEBUG) Serial.println("Creating Empty Space");
          mLevelGrid[x][y] = GameSquare::Empty;
        } else if (levelMap[y][x] == '0') {
          if (DEBUG) Serial.println("Snake Head!");
          mLevelGrid[x][y] = GameSquare::Snake;
          mInitialSnake[0] = Location(x, y);
        } else if (levelMap[y][x] == '1') {
          if (DEBUG) Serial.println("Snake Tail");
          mLevelGrid[x][y] = GameSquare::Snake;
          mInitialSnake[1] = Location(x, y);
        }
      }
    }
    mSnakeLength = 2;
  }

  GameSquare** levelGrid() {return mLevelGrid;}
  Location* initialSnake() {return mInitialSnake;}
  uint8_t snakeLength() {return mSnakeLength;}
  Direction levelStartDirection() {return mLevelStartDirection;}
  uint8_t coffeeTarget() {return mCoffeeTarget;}

private:

  static const bool DEBUG = false; 

  GameSquare** mLevelGrid;
  Location* mInitialSnake;
  uint8_t mSnakeLength;
  Direction mLevelStartDirection = Direction::Up;
  uint8_t mCoffeeTarget;

};

#endif // __LEVEL_H_INCLUDED__