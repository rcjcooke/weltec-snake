#ifndef __GAMEENGINE_H_INCLUDED__
#define __GAMEENGINE_H_INCLUDED__

#define LEVEL_GRID_WIDTH 30
#define LEVEL_GRID_HEIGHT 15

#include <Arduino.h>
#include <GameElements.hpp>
#include <Level.hpp>
#include <LinkedList.h>

class GameEngine {

public:
  GameEngine();

  void setupLevel(Level level) {
    // Set up the walls and initial snake
    for (int x = 0; x < LEVEL_GRID_WIDTH; x++) {
      for (int y = 0; y < LEVEL_GRID_HEIGHT; y++) {
        mGrid[x][y] = level.levelGrid()[x][y];
      }
    }
    for (int i=0; i<level.snakeLength(); i++) {
      mSnake.add(level.initialSnake()[i]);
    }
    mCurrentDirection = level.levelStartDirection();
    mCurrentLocation = mSnake.get(mSnake.size() - 1);
    setupCoffeeTarget(level.coffeeTarget());
  }

  bool isThereCoffeeAt(Location loc) {
    return mGrid[loc.x][loc.y] == GameSquare::Coffee;
  }

  bool isSnakeOrWallAt(Location loc) {
    return mGrid[loc.x][loc.y] == GameSquare::Snake || mGrid[loc.x][loc.y] == GameSquare::Wall;
  }

  void changeDirection(Direction direction) {
    if (direction != Direction::Same) {
      mCurrentDirection = direction;
    }
  }

  // Moves the snake one square, performs the checks and return the resulting game state
  GameState moveOneSquareAndCheck() {
    // Work out where the new head of the snake is
    switch (mCurrentDirection) {
      case Direction::Up:
        mCurrentLocation.y--;
        break;
      case Direction::Down:
        mCurrentLocation.y++;
        break;
      case Direction::Left:
        mCurrentLocation.x--;
        break;
      case Direction::Right:
        mCurrentLocation.x++;
        break;
    }
    mSnake.add(Location(mCurrentLocation));

    // Checks
    if (isSnakeOrWallAt(mCurrentLocation)) {
      // We've hit a wall or the snake
      return GameState::Died;
    }
    if (!isThereCoffeeAt(mCurrentLocation)) {
      // Remove the tail of the snake (because the snake has moved and not extended)
      Location tail = mSnake.shift();
      mGrid[tail.x][tail.y] = GameSquare::Empty;
      mNumberOfCoffees--;
    }
    if (mNumberOfCoffees == 0) {
      // If we've managed to get all the coffees then we've won the level! :)
      return GameState::Won;
    }
    // If we've got this far then we're all good to keep playing
    return GameState::Playing;
  }

protected:

  // Note: This must be called AFTER the level grid has been set up
  void setupCoffeeTarget(uint8_t numberOfCoffees) {
    for (int i=0; i<numberOfCoffees; i++) {
      bool placed = false;
      while (!placed) {
        uint8_t x = (uint8_t) random(LEVEL_GRID_WIDTH);
        uint8_t y = (uint8_t) random(LEVEL_GRID_HEIGHT);
        if (mGrid[x][y] == GameSquare::Empty) {
          mGrid[x][y] = GameSquare::Coffee;
          placed = true;
        }
      }
    }
    mNumberOfCoffees = numberOfCoffees;
  }

private:
  GameSquare mGrid[LEVEL_GRID_WIDTH][LEVEL_GRID_HEIGHT];
  LinkedList<Location> mSnake;
  uint8_t mNumberOfCoffees;
  Location mCurrentLocation;
  Direction mCurrentDirection;
};

#endif // __GAMEENGINE_H_INCLUDED__