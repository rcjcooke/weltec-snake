#ifndef __GFXENGINE_H_INCLUDED__
#define __GFXENGINE_H_INCLUDED__

class GfxEngine {

public:
  void drawMainMenu();
  void drawPauseMenu();
  void drawLevel();
  void drawSnakeUpdate();
  void drawDiedScreen();
  void drawWonScreen();
};

#endif // __GFXENGINE_H_INCLUDED__