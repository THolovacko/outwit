#include "outwit.h"
#include "game_state.h"

void init_game_state() {
  game_state.mode = MAIN_MENU_BATTLE;
  init_menu_screen(&game_state.menu_screen);
}

void update_game_state() {
  update_menu_screen(&game_state.menu_screen);
}
