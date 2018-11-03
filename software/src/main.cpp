#include <Arduino.h>
#include <Bounce2.h>

#include "GameEngine.hpp"
#include "GfxEngine.hpp"
#include "Levels.hpp"

#define SHOW_FRAME_RATE false

/************************
 * Constants
 ************************/
static const uint8_t UP_BUTTON_PIN = 3;
static const uint8_t DOWN_BUTTON_PIN = 2;
static const uint8_t LEFT_BUTTON_PIN = 4;
static const uint8_t RIGHT_BUTTON_PIN = 1;
static const uint8_t PAUSE_BUTTON_PIN = 6;

// Delay before accepting button state change for debouncing purposes
static const unsigned long LOCKOUT_DELAY_MILLIS = 100;

/************************
 * Variables
 ************************/
// Core game objects
GameEngine gGameEngine = GameEngine();
GfxEngine gGfxEngine = GfxEngine();

// Level definitions and variables
Level gLevels[2] = {Level(), Level()};
uint8_t gNumberOfLevels = 2;
uint8_t gCurrentLevel = 0;

// Game speed management variables
unsigned long gGameMovePeriodMillis = 250;
unsigned long gLastGameMoveTimeMillis = 0;

// Game state management
GameState gGameState = GameState::MainMenu;
GameState gPreviousGameState = GameState::Died;
bool gStateFirstEntry;

// Button variables
enum class Button {
  Up,
  Down,
  Left,
  Right,
  Pause,
  None
};

class ButtonWrapper {
public:
  ButtonWrapper(const Button button, const uint8_t pin) : button(button), pin(pin) {}

  void setup() {
    pinMode(pin, INPUT_PULLDOWN);
    debouncer.interval(LOCKOUT_DELAY_MILLIS);
    debouncer.attach(pin);
  }

  const Button button;
  const uint8_t pin;
  Bounce debouncer = Bounce();
};
ButtonWrapper gButtons[] = {
  ButtonWrapper(Button::Up, UP_BUTTON_PIN),
  ButtonWrapper(Button::Down, DOWN_BUTTON_PIN),
  ButtonWrapper(Button::Left, LEFT_BUTTON_PIN),
  ButtonWrapper(Button::Right, RIGHT_BUTTON_PIN),
  ButtonWrapper(Button::Pause, PAUSE_BUTTON_PIN)
};
Button gLastButtonPressed = Button::None;

/*****************************
 * Constructors / Initialisers
 *****************************/
void setup() {
  // Used for debug purposes
  Serial.begin(115200);
  
  // Delay to give time to connect Serial interface if required
  // delay(5000);
  Serial.println("Initialised serial interface");

  // Set up graphics engine
  gGfxEngine.begin();
  // Set up levels
  gLevels[0].setup(T_BLOCK_LEVEL_MAP, T_BLOCK_DIRECTION, T_BLOCK_COFFEE_TARGET);
  gLevels[1].setup(N_BLOCK_LEVEL_MAP, N_BLOCK_DIRECTION, N_BLOCK_COFFEE_TARGET);
  // Set up buttons
  for (int i=0; i<5; i++) {
    gButtons[i].setup();
  }
}

/***********************
 * Button Methods
 ***********************/
// Determine which button was last pressed if any
void updateLastButtonPressed() {
  ButtonWrapper* mostRecentButtonPressed = &gButtons[0];
  unsigned long minDuration = gButtons[0].debouncer.duration();
  for (int i=1; i<5; i++) {
    unsigned long duration = gButtons[i].debouncer.duration();
    if (duration < minDuration) {
      mostRecentButtonPressed = &gButtons[i];
      minDuration = duration;
    }
  }
  if (mostRecentButtonPressed->debouncer.rose()) {
    gLastButtonPressed = mostRecentButtonPressed->button;
  }
}

// Clears off any remaining buttons events
void clearAllButtonPressEvents() {
  gLastButtonPressed = Button::None;
}

// Checks to see if a button has been pressed and then clears off any remaining buttons events
bool checkAndClearAnyButtonPressed() {
  if (gLastButtonPressed != Button::None) {
    clearAllButtonPressEvents();
    return true;
  } else {
    return false;
  }
}

// Determines if one of the move buttons was pressed
bool isThereANewMoveDirection() {
  return gLastButtonPressed == Button::Up || gLastButtonPressed == Button::Down || gLastButtonPressed == Button::Left || gLastButtonPressed == Button::Right;
}

// Returns new Direction if there is one, otherwise NULL
Direction determineNewMoveDirection() {
  switch (gLastButtonPressed) {
    case Button::Up:
      gLastButtonPressed = Button::None;
      return Direction::Up;
    case Button::Down:
      gLastButtonPressed = Button::None;
      return Direction::Down;
    case Button::Left:
      gLastButtonPressed = Button::None;
      return Direction::Left;
    case Button::Right:
      gLastButtonPressed = Button::None;
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
  // Manage buttons
  for (int i=0; i<5; i++) {
    gButtons[i].debouncer.update();
  }
  updateLastButtonPressed();
  
  // Detect first entries into a state
  gStateFirstEntry = gGameState != gPreviousGameState;
  gPreviousGameState = gGameState;

  // Main game state machine
  switch (gGameState) {

    case GameState::MainMenu:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::MainMenu firstEntry");
        gGfxEngine.drawMainMenu();
        gGameEngine.resetGame();
        gGameEngine.setupLevel(&gLevels[0]);
      }
      // Events
      // Any button press starts game
      if (checkAndClearAnyButtonPressed()) {
        gGameState = GameState::Playing;
      }
      break;

    case GameState::Paused:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::Paused firstEntry");
        clearAllButtonPressEvents();
        gGfxEngine.drawPauseMenu();
      }
      // Events
      // Any button press starts game
      if (checkAndClearAnyButtonPressed()) {
        gGameState = GameState::Playing;
      }
      break;

    case GameState::Playing:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::Playing firstEntry");
        gGfxEngine.drawLevel(gGameEngine.getGameGrid());
        gGfxEngine.setInitialSnakeTail(gGameEngine.getSnakeTail());
        gGfxEngine.drawScoreUpdate(gGameEngine.getTotalScore(), true);
      }
      if (isThereANewMoveDirection()) {
        gGameEngine.changeDirection(determineNewMoveDirection());
      }
      if (millis() - gLastGameMoveTimeMillis > gGameMovePeriodMillis) {
        gLastGameMoveTimeMillis = millis();
        // Though this is an action it may generate an event
        gGameState = gGameEngine.moveOneSquareAndCheck();
        gGfxEngine.drawSnakeUpdate(gGameEngine.getSnakeHead(), gGameEngine.getSnakeTail());
        gGfxEngine.drawScoreUpdate(gGameEngine.getTotalScore(), false);
      }
      // Other Events
      if (gGameState == GameState::WonLevel && gCurrentLevel == gNumberOfLevels - 1) {
        gGameState = GameState::WonGame;
      }
      if (gGameState == GameState::Playing && gLastButtonPressed == Button::Pause) {
        clearAllButtonPressEvents();
        gGameState = GameState::Paused;
      }
      break;

    case GameState::Died:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::Died firstEntry");
        gGfxEngine.drawDiedScreen(gGameEngine.getTotalScore());
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        gGameState = GameState::MainMenu;
      }
      break;

    case GameState::WonLevel:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::WonLevel firstEntry");
        gCurrentLevel++;
        gGfxEngine.drawWonLevelScreen();
        gGameEngine.setupLevel(&gLevels[gCurrentLevel]);
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        gGameState = GameState::Playing;
      }
      break;

    case GameState::WonGame:
      // Actions
      if (gStateFirstEntry) {
        Serial.println("GameState::WonGame firstEntry");
        gGfxEngine.drawWonGameScreen();
      }
      // Events
      if (checkAndClearAnyButtonPressed()) {
        gGameState = GameState::MainMenu;
      }
      break;

  }

  // Some frame rate information if we want it
  if (SHOW_FRAME_RATE) gGfxEngine.updateRefreshRate();

}