#include <Arduino.h>

#include "GameEngine.hpp"
#include "GfxEngine.hpp"
#include "Levels.hpp"

#define SHOW_FRAME_RATE true

// Core game objects
GameEngine mGameEngine = GameEngine();
GfxEngine mGfxEngine = GfxEngine();

// Level definitions and variables
Level mLevels[] = {N_BLOCK};
uint8_t mNumberOfLevels = 1;
uint8_t mCurrentLevel = 0;

// Game speed management variables
unsigned long mGameMovePeriodMillis = 250;
unsigned long mLastGameMoveTimeMillis = 0;

// Game state management
GameState mGameState = GameState::MainMenu;
GameState mPreviousGameState = GameState::Died;
bool mStateFirstEntry;

// Button variables
enum class Button {
  Up,
  Down,
  Left,
  Right,
  Pause,
  None
};
Button mLastButtonPressed = Button::None;

/*****************************
 * Constructors / Initialisers
 *****************************/
// TODO Interrupt routines for buttons

void setup()   {
  mGfxEngine.begin();
}

/***********************
 * Button Methods
 ***********************/
// Clears off any remaining buttons events
void clearAllButtonPressEvents() {
  mLastButtonPressed = Button::None;
}

// Checks to see if a button has been pressed and then clears off any remaining buttons events
bool checkAndClearAnyButtonPressed() {
  if (mLastButtonPressed != Button::None) {
    clearAllButtonPressEvents();
    return true;
  } else {
    return false;
  }
}

// Determines if one of the move buttons was pressed
bool isThereANewMoveDirection() {
  return mLastButtonPressed == Button::Up || mLastButtonPressed == Button::Down || mLastButtonPressed == Button::Left || mLastButtonPressed == Button::Right;
}

// Returns new Direction if there is one, otherwise NULL
Direction determineNewMoveDirection() {
  switch (mLastButtonPressed) {
    case Button::Up:
      mLastButtonPressed = Button::None;
      return Direction::Up;
    case Button::Down:
      mLastButtonPressed = Button::None;
      return Direction::Down;
    case Button::Left:
      mLastButtonPressed = Button::None;
      return Direction::Left;
    case Button::Right:
      mLastButtonPressed = Button::None;
      return Direction::Right;
    default:
      // Shouldn't get here as use of this method should be bracketed by isThereANewMoveDirection
      return Direction::Up;
  }
}

/***********************
 * Main Game Loop
 ***********************/
// Just keep looping, just keep looping, just keep looping, looping, looping
void loop() {

  // Detect first entries into a state (for rendering efficiency)
  mStateFirstEntry = mGameState != mPreviousGameState;
  // Main game state machine
  switch (mGameState) {

    case GameState::MainMenu:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawMainMenu();
        mGameEngine.resetGame();
        mGameEngine.setupLevel(mLevels[0]);
      }
      // Events
      // Any button press starts game
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::Playing;
      }
      break;

    case GameState::Paused:
      // Actions
      if (mStateFirstEntry) {
        clearAllButtonPressEvents();
        mGfxEngine.drawPauseMenu();
      }
      // Events
      // Any button press starts game
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::Playing;
      }
      break;

    case GameState::Playing:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawScoreUpdate(mGameEngine.getTotalScore());
        mGfxEngine.drawLevel(mGameEngine.getGameGrid());
      }
      if (isThereANewMoveDirection()) {
        mGameEngine.changeDirection(determineNewMoveDirection());
      }
      if (millis() - mLastGameMoveTimeMillis > mGameMovePeriodMillis) {
        mLastGameMoveTimeMillis = millis();
        // Though this is an action it may generate an event
        mGameState = mGameEngine.moveOneSquareAndCheck();
        mGfxEngine.drawSnakeUpdate(mGameEngine.getSnakeHead(), mGameEngine.getSnakeTail());
        mGfxEngine.drawScoreUpdate(mGameEngine.getTotalScore());
      }
      // Other Events
      if (mGameState == GameState::WonLevel && mCurrentLevel == mNumberOfLevels - 1) {
        mGameState = GameState::WonGame;
      }
      if (mGameState == GameState::Playing && mLastButtonPressed == Button::Pause) {
        clearAllButtonPressEvents();
        mGameState = GameState::Paused;
      }
      break;

    case GameState::Died:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawDiedScreen(mGameEngine.getTotalScore());
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::MainMenu;
      }
      break;

    case GameState::WonLevel:
      // Actions
      if (mStateFirstEntry) {
        mCurrentLevel++;
        mGfxEngine.drawWonLevelScreen();
        mGameEngine.setupLevel(mLevels[mCurrentLevel]);
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::Playing;
      }
      break;

    case GameState::WonGame:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawWonGameScreen();
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::MainMenu;
      }
      break;

  }

  // Some frame rate information if we want it
  if (SHOW_FRAME_RATE) mGfxEngine.updateRefreshRate();

}