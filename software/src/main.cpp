#include <Arduino.h>
#include "GameEngine.hpp"
#include "GfxEngine.hpp"
#include "Levels.hpp"

enum class Button {
  Up,
  Down,
  Left,
  Right,
  Pause,
  None
};

// State, Game and Menu variables
GameState mGameState = GameState::MainMenu;
GameState mPreviousGameState = GameState::Died;
GameEngine mGameEngine = GameEngine();
GfxEngine mGfxEngine = GfxEngine();
Level mLevels[] = {T_BLOCK, N_BLOCK};
uint8_t mNumberOfLevels = 2;
uint8_t mCurrentLevel = 0;
unsigned long mLastGameMoveTimeMillis = 0;
unsigned long mGameMovePeriodMillis = 250;

bool mStateFirstEntry;

// Button variables
Button mLastButtonPressed = Button::None;

/*****************************
 * Constructors / Initialisers
 *****************************/
void setup()   {
  // TODO
}

/***********************
 * Button Methods
 ***********************/
// Checks to see if a button has been pressed and then clears off any remaining buttons events
bool checkAndClearAnyButtonPressed() {
  if (mLastButtonPressed != Button::None) {
    clearAllButtonPressEvents();
    return true;
  } else {
    return false;
  }
}

// Clears off any remaining buttons events
void clearAllButtonPressEvents() {
  mLastButtonPressed = Button::None;
}

Direction checkForNewMoveDirection() {
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
      return Direction::Same;
  }
}

/***********************
 * Main Game Loop
 ***********************/
void loop() {

  // Detect first entries into a state (for rendering efficiency)
  mStateFirstEntry = mGameState != mPreviousGameState;
  // Main game state machine
  switch (mGameState) {
    case GameState::MainMenu:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawMainMenu();
      }
      // Events
      // Any button press starts game
      if (checkAndClearAnyButtonPressed()) {
        mGameEngine.setupLevel(mLevels[0]);
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
        mGfxEngine.drawLevel();
      }
      mGameEngine.changeDirection(checkForNewMoveDirection());
      if (millis() - mLastGameMoveTimeMillis > mGameMovePeriodMillis) {
        mLastGameMoveTimeMillis = millis();
        mGameState = mGameEngine.moveOneSquareAndCheck();
        mGfxEngine.drawSnakeUpdate();
      }
      // Other Events
      if (mGameState == GameState::Playing && mLastButtonPressed == Button::Pause) {
        clearAllButtonPressEvents();
        mGameState = GameState::Paused;
      }
      break;
    case GameState::Died:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawDiedScreen();
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        mGameState = GameState::MainMenu;
      }
      break;
    case GameState::Won:
      // Actions
      if (mStateFirstEntry) {
        mGfxEngine.drawWonScreen();
        mCurrentLevel++;
        if (mCurrentLevel != mNumberOfLevels) mGameEngine.setupLevel(mLevels[mCurrentLevel]);
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        if (mCurrentLevel == mNumberOfLevels) {
          mGameState = GameState::MainMenu;
        } else {
          mGameState = GameState::Playing;
        }
      }
      break;
  }
}