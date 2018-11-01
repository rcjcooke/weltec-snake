#ifndef __GFXENGINE_H_INCLUDED__
#define __GFXENGINE_H_INCLUDED__

#include <SPI.h>
#include <i2c_t3.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "GameElements.hpp"
#include "Images.hpp"

#define SCORE_LINE_HEIGHT 6;

class GfxEngine {

public:

  /*****************************
   * Constructors / Initialisers
   *****************************/
  void begin() {
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
    mDisplay.clearDisplay();
    mDisplay.drawBitmap(0, 0, GAME_OVER_BMP, 128, 64, WHITE);
    mDisplay.setCursor(120, 60);
    mDisplay.setTextSize(3);
    mDisplay.print(score);
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
    static unsigned long previousFrame = millis();
    static float frameRate = 0;

    // Clear the frame rate
    mDisplay.setTextColor(BLACK);
    mDisplay.setCursor(0,0);
    mDisplay.println(frameRate);

    // Calculate the frame rate
    frameRate = 1000/(millis() - previousFrame);
    previousFrame = millis();

    // Draw the frame rate
    mDisplay.setTextColor(WHITE);
    mDisplay.setCursor(0,0);
    mDisplay.println((int) frameRate);
    
    mDisplay.display();
  }
  
  /*****************************
   * Game Element Methods
   *****************************/
  // Draw out a level from scratch
  void drawLevel(GameSquare** grid) {
    mDisplay.clearDisplay();
    for (int x = 0; x <= LEVEL_GRID_WIDTH; x++) {
      for (int y = 0; y <= LEVEL_GRID_HEIGHT; y++) {
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

  // The snake has moved - update it
  void drawSnakeUpdate(Location head, Location tail) {
    if (mPreviousSnakeTail != tail) {
      clearSnake(mPreviousSnakeTail.x, mPreviousSnakeTail.y);
      mPreviousSnakeTail = tail;
    }
    drawSnake(head.x, head.y);
  }

protected:
  // Draw a coffee cup
  void drawCoffee(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    mDisplay.drawPixel(dx, dy, WHITE);
    mDisplay.drawPixel(dx+1, dy+1, WHITE);
  }

  // Draw a snake piece
  void drawSnake(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    mDisplay.drawRect(dx, dy, 2, 2, WHITE);
  }

  // Clear a snake piece
  void clearSnake(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
    mDisplay.drawRect(dx, dy, 2, 2, BLACK);
  }

  // Draw a wall
  void drawWall(uint8_t x, uint8_t y) {
    int dx = transformGameX(x);
    int dy = transformGameY(y);
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

  Adafruit_SSD1306 mDisplay = Adafruit_SSD1306(-1);
  Location mPreviousSnakeTail;

};

#endif // __GFXENGINE_H_INCLUDED__