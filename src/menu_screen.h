#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

#include "game_state.h"

typedef struct {
  char text[16];
  Rectangle rectangle;
  int pos_x;
  int pos_y;
  int font_size;
  Color color;
} UI_Text;

typedef struct {
  Texture2D image;
  Rectangle rectangle;
  int pos_x;
  int pos_y;
} UI_Image;

typedef struct {
  Texture2D venmo_img;
  UI_Text battle_txt;
  UI_Text report_txt;
  UI_Text credits_txt;
} Menu_Screen;

void init_menu_screen(Menu_Screen* menu_screen);
void update_menu_screen(Menu_Screen* menu_screen);
void render_menu_screen(Menu_Screen* menu_screen);

void init_ui_text(UI_Text* ui, const char* text, int pos_x, int pos_y, int font_size, Color color);
void init_ui_image(UI_Image* ui, const char* image_path, int pos_x, int pos_y);
bool is_ui_text_hovered(UI_Text* ui);
bool is_ui_image_hovered(UI_Image* ui);
void render_ui_text(UI_Text* ui);
void render_ui_image(UI_Image* ui);

#endif // MENU_SCREEN_H
