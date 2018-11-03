#ifndef __GAMEENGINE_H_INCLUDED__
#define __GAMEENGINE_H_INCLUDED__

#include <Arduino.h>
#include <LinkedList.h>

#include "GameElements.hpp"
#include "Level.hpp"

class GameEngine {

public:
  GameEngine() {}

  void resetGame() {
    mTotalScore = 0;
  }

  void setupLevel(Level* level) {
    // Memory management
    if (mGrid != nullptr) {
      if (INFO) Serial.println("GameEngine::setupLevel(): Cleaning up game grid");
      for (int i=0; i<LEVEL_GRID_WIDTH; i++) delete mGrid[i];
      delete mGrid;
    }
    if (INFO) Serial.println("GameEngine::setupLevel(): Allocating new game grid");
    
    mGrid = new GameSquare*[LEVEL_GRID_WIDTH];
    for (int i=0; i<LEVEL_GRID_WIDTH; i++) mGrid[i] = new GameSquare[LEVEL_GRID_HEIGHT];
    
    if (INFO) Serial.println("GameEngine::setupLevel(): Clearing off snake");
    mSnake.clear();
    
    // Set up the walls and initial snake
    if (INFO) Serial.println("GameEngine::setupLevel(): Setting up walls");
    for (int x = 0; x < LEVEL_GRID_WIDTH; x++) {
      for (int y = 0; y < LEVEL_GRID_HEIGHT; y++) {
        mGrid[x][y] = level->levelGrid()[x][y];
        if (DEBUG) Serial.printf("Grid: %d,%d: %p:%s -> %p:%s\r\n", x, y, (void*) &level->levelGrid()[x][y], translateGameSquare(level->levelGrid()[x][y]), (void*) &mGrid[x][y], translateGameSquare(mGrid[x][y]));
      }
    }
    if (INFO) Serial.println("GameEngine::setupLevel(): Setting up initial snake");
    for (int i=0; i<level->snakeLength(); i++) {
      mSnake.add(Location(level->initialSnake()[i]));
      if (DEBUG) { 
        char buffer[10];
        mSnake.get(i).toString(buffer);
        Serial.printf("Snake: %d: %s\r\n", i, buffer);
      }
    }
    mCurrentDirection = level->levelStartDirection();
    mCurrentLocation = mSnake.get(0);
    setupCoffeeTarget(level->coffeeTarget());
  }

  bool isThereCoffeeAt(Location loc) {
    return mGrid[loc.x][loc.y] == GameSquare::Coffee;
  }

  bool isSnakeOrWallAt(Location loc) {
    return mGrid[loc.x][loc.y] == GameSquare::Snake || mGrid[loc.x][loc.y] == GameSquare::Wall;
  }

  void changeDirection(Direction direction) {
    mCurrentDirection = direction;
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
    // Add the new head
    mSnake.unshift(Location(mCurrentLocation));

    // Checks
    if (isSnakeOrWallAt(mCurrentLocation)) {
      // We've hit a wall or the snake
      return GameState::Died;
    }
    if (isThereCoffeeAt(mCurrentLocation)) {
      // We've scored some caffeine! :)
      if (INFO) Serial.printf("Got some coffee. New score: %d\r\n", mTotalScore);
      mNumberOfCoffees--;
      mTotalScore++;
    } else {
      // Remove the tail of the snake (because the snake has moved and not extended)
      Location tail = mSnake.pop();
      mGrid[tail.x][tail.y] = GameSquare::Empty;
    }
    // There is now a snake at this location so record it
    mGrid[mCurrentLocation.x][mCurrentLocation.y] = GameSquare::Snake;

    // If we've managed to get all the coffees then we've won the level! :)
    if (mNumberOfCoffees == 0) {
      return GameState::WonLevel;
    }
    // If we've got this far then we're all good to keep playing
    return GameState::Playing;
  }

  uint8_t getTotalScore() {
    return mTotalScore;
  }

  /********************************
   * Graphics Engine Access Methods
   ********************************/
  GameSquare** getGameGrid() {
    return (GameSquare**) mGrid;
  }

  Location getSnakeHead() {
    return mSnake.get(0);
  }

  Location getSnakeTail() {
    return mSnake.get(mSnake.size() - 1);
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

  static const bool INFO = true;
  static const bool DEBUG = false;

  // The game grid
  GameSquare** mGrid = nullptr;
  // The snake body
  LinkedList<Location> mSnake = LinkedList<Location>();
  // The number of coffees left to get on the current level
  uint8_t mNumberOfCoffees;
  // The current location of the snake's head
  Location mCurrentLocation;
  // The direction the snake will move next
  Direction mCurrentDirection;
  // The total number of coffees drunk this game across all levels
  uint8_t mTotalScore;
};

#endif // __GAMEENGINE_H_INCLUDED__