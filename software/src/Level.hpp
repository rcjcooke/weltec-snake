#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include "GameElements.hpp"

class Level {

public:

  Level(const char** levelMap, Direction startDirection, uint8_t coffeeTarget) {
    mLevelStartDirection = startDirection;
    mCoffeeTarget = coffeeTarget;
    Location snake[2];
    for (int x = 0; x < LEVEL_GRID_WIDTH; x++) {
      for (int y = 0; y < LEVEL_GRID_HEIGHT; y++) {
        if (levelMap[y][x] == 'W') {
          mLevelGrid[x][y] = GameSquare::Wall;
        } else if (levelMap[y][x] == ' ') {
          mLevelGrid[x][y] = GameSquare::Empty;
        } else if (levelMap[y][x] == '0') {
          mLevelGrid[x][y] = GameSquare::Snake;
          mInitialSnake[0] = Location(x, y);
        } else if (levelMap[y][x] == '1') {
          mLevelGrid[x][y] = GameSquare::Snake;
          mInitialSnake[1] = Location(x, y);
        }
      }
    }
    mInitialSnake = snake;
    mSnakeLength = 2;
  }

  GameSquare** levelGrid() {return (GameSquare**) mLevelGrid;}
  Location* initialSnake() {return mInitialSnake;}
  uint8_t snakeLength() {return mSnakeLength;}
  Direction levelStartDirection() {return mLevelStartDirection;}
  uint8_t coffeeTarget() {return mCoffeeTarget;}

private:

  GameSquare mLevelGrid[LEVEL_GRID_WIDTH][LEVEL_GRID_HEIGHT];
  Location* mInitialSnake;
  uint8_t mSnakeLength;
  Direction mLevelStartDirection = Direction::Up;
  uint8_t mCoffeeTarget;

};

#endif // __LEVEL_H_INCLUDED__