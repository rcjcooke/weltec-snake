#ifndef __LEVELS_H_INCLUDED__
#define __LEVELS_H_INCLUDED__

#include  "Level.hpp"

const char* N_BLOCK_LEVEL_MAP[] = {
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
  "WWWWWWW               W             W   W                WWWWWWW",
  "WWWWWWW        WWWWWWWWWWWWWWWWWWWWWW   W                WWWWWWW",
  "WWWWWWW        W         W          W   W                WWWWWWW",
  "WWWWWWW        W         W              W                WWWWWWW",
  "WWWWWWW        W         W              W                WWWWWWW",
  "WWWWWWW        W         W          W   W                WWWWWWW",
  "WWWWWWW        WWW  WWWWWWWWWWWWWWWWW   W                WWWWWWW",
  "WWWWWWW                                 W                WWWWWWW",
  "WWWWWWW                                 W                WWWWWWW",
  "WWWWWWWWWWWWWWWW  WWWWWWW  WWWWWWWWWW   W      WWWWWWWWWWWWWWWWW",
  "WWWWWWW              W              W   W      W         WWWWWWW",
  "WWWWWWW              W              W   WWWWWWWW         WWWWWWW",
  "WWWWWWW              W              W          W         WWWWWWW",
  "WWWWWWW              W              W 0 WWWW             WWWWWWW",
  "WWWWWWW              W              W 1 WWWW             WWWWWWW",
  "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
};
Level N_BLOCK = Level((const char**) N_BLOCK_LEVEL_MAP, Direction::Up, 20);

// Level T_BLOCK = Level();// TODO

#endif // __LEVELS_H_INCLUDED__