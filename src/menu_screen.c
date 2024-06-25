#include "outwit.h"
#include "menu_screen.h"
#include <string.h>

void init_ui_text(UI_Text* ui, const char* text, int pos_x, int pos_y, int font_size, Color color) {
  strcpy(ui->text, text);
  ui->pos_x = pos_x;
  ui->pos_y = pos_y;
  ui->font_size = font_size;
  ui->color = color;

  Vector2 text_size = MeasureTextEx(GetFontDefault(), ui->text, ui->font_size, 1);
  ui->rectangle.x = ui->pos_x;
  ui->rectangle.y = ui->pos_y;
  ui->rectangle.width = text_size.x + 10;
  ui->rectangle.height = text_size.y;
}

void init_ui_image(UI_Image* ui, const char* image_path, int pos_x, int pos_y) {
  ui->pos_x = pos_x;
  ui->pos_y = pos_y;
  ui->image = LoadTexture(image_path);
  ui->rectangle.x = ui->pos_x;
  ui->rectangle.y = ui->pos_y;
  ui->rectangle.width = ui->image.width;
  ui->rectangle.height = ui->image.height;
}

bool is_ui_text_hovered(UI_Text* ui) {
  Vector2 mouse_pos = GetMousePosition();
  mouse_pos.x = mouse_pos.x * game_state.resolution_width_scale;
  mouse_pos.y = mouse_pos.y * game_state.resolution_height_scale;
  return CheckCollisionPointRec(mouse_pos, ui->rectangle);
}

bool is_ui_image_hovered(UI_Image* ui) {
  return CheckCollisionPointRec(GetMousePosition(), ui->rectangle);
}

void init_menu_screen(Menu_Screen* menu_screen) {
  menu_screen->venmo_img = LoadTexture("resources/venmo.png");
  init_ui_text(&menu_screen->battle_txt, "Battle", 22, 25, 20, WHITE);
  init_ui_text(&menu_screen->report_txt, "Report Issue", 22, 50, 20, WHITE);
  init_ui_text(&menu_screen->credits_txt, "Credits", 22, 75, 20, WHITE);
}

void render_ui_text(UI_Text* ui) {
  DrawText(ui->text, ui->pos_x, ui->pos_y, ui->font_size, ui->color);

  #ifdef DEBUG_MODE
  DrawRectangleLines(ui->rectangle.x, ui->rectangle.y, ui->rectangle.width, ui->rectangle.height, BLUE);
  #endif
}

void render_ui_image(UI_Image* ui) {
  DrawTexture(ui->image, ui->pos_x, ui->pos_y, WHITE);
}

void update_menu_screen(Menu_Screen* menu_screen) {
  switch ( game_state.mode ) {
      case MAIN_MENU_BATTLE: {
        if ( IsKeyPressed(KEY_TAB) && ( IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ) || IsKeyPressed(KEY_UP) ) {
          game_state.mode = MAIN_MENU_CREDITS;
        } else if ( IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN) ) {
          game_state.mode = MAIN_MENU_REPORT_ISSUE;
        }

        break;
      }

      case MAIN_MENU_REPORT_ISSUE: {
        if ( IsKeyPressed(KEY_TAB) && ( IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ) || IsKeyPressed(KEY_UP) ) {
          game_state.mode = MAIN_MENU_BATTLE;
        } else if ( IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN) ) {
          game_state.mode = MAIN_MENU_CREDITS;
        }

        break;
      }

      case MAIN_MENU_CREDITS: {
        if ( IsKeyPressed(KEY_TAB) && ( IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT) ) || IsKeyPressed(KEY_UP) ) {
          game_state.mode = MAIN_MENU_REPORT_ISSUE;
        } else if ( IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN) ) {
          game_state.mode = MAIN_MENU_BATTLE;
        }

        break;
      }
  }

  if ( IsMouseButtonPressed(MOUSE_LEFT_BUTTON) ) {
    if ( is_ui_text_hovered(&menu_screen->battle_txt) ) {
      game_state.mode = MAIN_MENU_BATTLE;
    } else if ( is_ui_text_hovered(&menu_screen->report_txt) ) {
      game_state.mode = MAIN_MENU_REPORT_ISSUE;
    } else if ( is_ui_text_hovered(&menu_screen->credits_txt) ) {
      game_state.mode = MAIN_MENU_CREDITS;
    }
  }
}

void render_menu_screen(Menu_Screen* menu_screen) {
  menu_screen->battle_txt.color  = WHITE;
  menu_screen->report_txt.color  = WHITE;
  menu_screen->credits_txt.color = WHITE;

  switch ( game_state.mode ) {
      case MAIN_MENU_BATTLE: {
        menu_screen->battle_txt.color = YELLOW;
        break;
      }

      case MAIN_MENU_REPORT_ISSUE: {
        menu_screen->report_txt.color = YELLOW;
        break;
      }

      case MAIN_MENU_CREDITS: {
        menu_screen->credits_txt.color = YELLOW;
        break;
      }
  }

  render_ui_text(&menu_screen->battle_txt);
  render_ui_text(&menu_screen->report_txt);
  render_ui_text(&menu_screen->credits_txt);
  DrawText("@Tom-Holovacko-1", 22, 180, 17, WHITE);
  DrawTexture(menu_screen->venmo_img, 22, 200, WHITE);
}
