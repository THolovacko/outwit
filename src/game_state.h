#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "menu_screen.h"

typedef enum {
  MAIN_MENU_BATTLE,
  MAIN_MENU_CREDITS,
  MAIN_MENU_REPORT_ISSUE,

  COUNT
} Game_Mode;

typedef struct {
  Game_Mode mode;
  Menu_Screen menu_screen;
  float resolution_width_scale;
  float resolution_height_scale;
} Game_State;

void init_game_state();
void update_game_state();

extern Game_State game_state;

#endif  // GAME_STATE_H
