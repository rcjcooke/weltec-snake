#ifndef __LEVELS_H_INCLUDED__
#define __LEVELS_H_INCLUDED__

#include  "Level.hpp"

static const char* N_BLOCK_LEVEL_MAP[] = {
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
  "WWWWWWW      W                             W               WWWWW",
  "WWWWWWW      W     WWWWW  WW   WW  WW      W               WWWWW",
  "WWWWWWW      W     WWWWW           WW      W               WWWWW",
  "WWWWWWW      W            WW   WW  WW   WW W               WWWWW",
  "WWWWWWW      W WW         WW                               WWWWW",
  "WWWWWWW      W            WW   WW  WW   WW                 WWWWW",
  "WWWWWWW      W     WWWWW  WW   WW  WW                      WWWWW",
  "WWWWWWW      W                          WW W               WWWWW",
  "WWWWWWW      W                             W               WWWWW",
  "WWWWWWW      WWWWWWWWWWWWWWWWWW   WWW   WWWWWWWWWWWWWWWWWWWWWWWW",
  "WWWWWWW               W             W   W                WWWWWWW",
  "WWWWWWW               W             W   W                WWWWWWW",
  "WWWWWWW               W             W   W                WWWWWWW",
  "WWWWWWW        WWWWWWWWWWWWWWWWWWWWWW   W                WWWWWWW",
  "WWWWWWW        W         W          W   W                WWWWWWW",
  "WWWWWWW        W         W              W                WWWWWWW",
  "WWWWWWW        W         W              W                WWWWWWW",
  "WWWWWWW        W         W          W   W                WWWWWWW",
  "WWWWWWW        WWW  WWWWWWWWWWWWWWWWW                    WWWWWWW",
  "WWWWWWW                                                  WWWWWWW",
  "WWWWWWW                                                  WWWWWWW",
  "WWWWWWWWWWWWWWWW  WWWWWWW  WWWWWWWWWW   W      WWWWWWWWWWWWWWWWW",
  "WWWWWWW              W              W   WWWWWWWW         WWWWWWW",
  "WWWWWWW              W              W          W         WWWWWWW",
  "WWWWWWW              W              W 0 WWWW             WWWWWWW",
  "WWWWWWW              W              W 1 WWWW             WWWWWWW",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
};
static const Direction N_BLOCK_DIRECTION = Direction::Up;
static const uint8_t N_BLOCK_COFFEE_TARGET = 20;

static const char* T_BLOCK_LEVEL_MAP[] = {
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
  "W                                                    W    1    W",
  "W                                                    W    0    W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                    W         W",
  "W                                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W               W                                              W",
  "W                                                              W",
  "W                                                              W",
  "W                                                              W",
  "W                                                              W",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW        WW",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW        WW",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW        WW",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
};
static const Direction T_BLOCK_DIRECTION = Direction::Down;
static const uint8_t T_BLOCK_COFFEE_TARGET = 20;



#endif // __LEVELS_H_INCLUDED__