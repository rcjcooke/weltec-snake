#ifndef __LEVEL_H_INCLUDED__
#define __LEVEL_H_INCLUDED__

#include <GameElements.hpp>

class Level {

public:

  GameSquare** levelGrid() {return (GameSquare**) mLevelGrid;}
  Location* initialSnake() {return (Location*) mInitialSnake;}
  uint8_t snakeLength() {return mSnakeLength;}
  Direction levelStartDirection() {return mLevelStartDirection;}
  uint8_t coffeeTarget() {return mCoffeeTarget;}

private:

  GameSquare mLevelGrid[LEVEL_GRID_WIDTH][LEVEL_GRID_HEIGHT];
  Location mInitialSnake[] = {};
  uint8_t mSnakeLength;
  Direction mLevelStartDirection = Direction::Up;
  uint8_t mCoffeeTarget;

};

#endif // __LEVEL_H_INCLUDED__