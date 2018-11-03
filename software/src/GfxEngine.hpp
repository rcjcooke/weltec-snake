#ifndef __GFXENGINE_H_INCLUDED__
#define __GFXENGINE_H_INCLUDED__

#include <SPI.h>
#include <i2c_t3.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "GameElements.hpp"
#include "Images.hpp"

#define SCORE_LINE_HEIGHT 8;

class GfxEngine {

public:

  /*****************************
   * Constructors / Initialisers
   *****************************/
  void begin() {
    Serial.println("Got to gfxengine::begin");

    mDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C, false);  // initialize with the I2C addr 0x3C (for the 128x64)

    Wire.setClock(1800000);
    Wire.setOpMode(I2C_OP_MODE_DMA);

    // Clear the buffer.
    mDisplay.clearDisplay();
  }
  
  /*****************************
   * Menu Methods
   *****************************/
  // Draw the main menu
  void drawMainMenu() {
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, MAIN_MENU_BMP, 128, 64, WHITE);
    mDisplay.display();
  }
  // Draw the pause menu
  void drawPauseMenu() {
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, PAUSE_MENU_BMP, 128, 64, WHITE);
    mDisplay.display();
  }
  // Draw the game over screen
  void drawDiedScreen(uint8_t score) {
    if (INFO) Serial.printf("GfxEngine::drawDiedScreen(): score: %d\r\n", score);
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, GAME_OVER_BMP, 128, 64, WHITE);
    mDisplay.setCursor(51, 29);
    mDisplay.setTextSize(1);
    mDisplay.setTextColor(BLACK);
    mDisplay.printf("%02d", score);
    mDisplay.display();
  }
  // Draw the finished level screen
  void drawWonLevelScreen() {
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, LEVEL_FINISHED_BMP, 128, 64, WHITE);
    mDisplay.display();
  }
  // Draw the finished game screen
  void drawWonGameScreen() {
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, GAME_FINISHED_BMP, 128, 64, WHITE);
    mDisplay.display();
  }

  // Update the refresh rate
  void updateRefreshRate() {
    static unsigned long sPreviousFrame = millis();
    static float sFrameRate = 0;

    // Clear the frame rate
    mDisplay.setTextColor(BLACK);
    mDisplay.setCursor(40,0);
    mDisplay.println(sFrameRate);

    // Calculate the frame rate
    sFrameRate = 1000/(millis() - sPreviousFrame);
    sPreviousFrame = millis();

    // Draw the frame rate
    mDisplay.setTextColor(WHITE);
    mDisplay.setCursor(40,0);
    mDisplay.println((int) sFrameRate);
    
    mDisplay.display();
  }
  
  /*****************************
   * Game Element Methods
   *****************************/
  // Draw out a level from scratch
  void drawLevel(GameSquare** grid) {
    mDisplay.clearDisplay();
    // Draw the game grid
    for (int x = 0; x < LEVEL_GRID_WIDTH; x++) {
      for (int y = 0; y < LEVEL_GRID_HEIGHT; y++) {
        if (DEBUG) Serial.printf("drawLevel(%d,%d): %s\r\n", x, y, translateGameSquare(grid[x][y]));
        switch (grid[x][y]) {
          case GameSquare::Wall:
            drawWall(x, y);
            break;
          case GameSquare::Coffee:
            drawCoffee(x, y);
            break;
          case GameSquare::Snake:
            drawSnake(x, y);
            break;
          case GameSquare::Empty:
            break;
        }
      }
    }
    mDisplay.display();
  }

  // Must be called once before play begins on a new level to ensure very first snake tail is correctly cleared 
  void setInitialSnakeTail(Location tail) {
    mPreviousSnakeTail.x = tail.x;
    mPreviousSnakeTail.y = tail.y;
  }

  // The snake has moved - update it
  void drawSnakeUpdate(Location head, Location tail) {

    if (DEBUG) {
      char headBuf[10]; head.toString(headBuf);
      char tailBuf[10]; tail.toString(tailBuf);
      Serial.printf("GfxEngine::drawSnakeUpdate(): head: %s, tail: %s\r\n", headBuf, tailBuf);
    }

    if (tail != mPreviousSnakeTail) {
      clearSnake(mPreviousSnakeTail.x, mPreviousSnakeTail.y);
      mPreviousSnakeTail.x = tail.x;
      mPreviousSnakeTail.y = tail.y;
    }
    drawSnake(head.x, head.y);

    mDisplay.display();
  }

  void drawScoreUpdate(uint8_t score, bool forceDraw) {
    static uint8_t sPreviousScore = -1;
    // Only update the score if it's changed
    if (forceDraw || score != sPreviousScore) {
      // Clear the score
      mDisplay.setTextSize(1);
      mDisplay.setTextColor(BLACK);
      mDisplay.setCursor(0,0);
      mDisplay.printf("Caffeine scored: %d", sPreviousScore);
      // Draw the score bar
      mDisplay.setTextSize(1);
      mDisplay.setTextColor(WHITE);
      mDisplay.setCursor(0,0);
      mDisplay.printf("Caffeine scored: %d", score);
      sPreviousScore = score;
      mDisplay.display();
    }
  }

protected:
  // Draw a coffee cup
  void drawCoffee(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    if (DEBUG) Serial.printf("drawCoffee(dx:%d, dy:%d)\r\n", dx, dy);
    mDisplay.drawPixel(dx, dy, WHITE);
    mDisplay.drawPixel(dx+1, dy+1, WHITE);
  }

  // Draw a snake piece
  void drawSnake(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    if (DEBUG) Serial.printf("GfxEngine::drawSnake(dx:%d, dy:%d)\r\n", dx, dy);
    mDisplay.drawRect(dx, dy, 2, 2, WHITE);
  }

  // Clear a snake piece
  void clearSnake(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    if (DEBUG) Serial.printf("GfxEngine::clearSnake(dx:%d, dy:%d)\r\n", dx, dy);
    mDisplay.drawRect(dx, dy, 2, 2, BLACK);
  }

  // Draw a wall
  void drawWall(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    if (DEBUG) Serial.printf("drawWall(dx:%d, dy:%d)\r\n", dx, dy);
    mDisplay.drawRect(dx, dy, 2, 2, WHITE);
  }

  // Transform a game grid X co-ordinate into a display X co-ordinate
  int16_t transformGameX(uint8_t x) {
    return x*2;
  }

  // Transform a game grid Y co-ordinate into a display Y co-ordinate
  int16_t transformGameY(uint8_t y) {
    return y*2 + SCORE_LINE_HEIGHT;
  }

private:

  static const bool DEBUG = false;
  static const bool INFO = true;

  Adafruit_SSD1306 mDisplay = Adafruit_SSD1306(-1);
  Location mPreviousSnakeTail = Location();
};

#endif // __GFXENGINE_H_INCLUDED__