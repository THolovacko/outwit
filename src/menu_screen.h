#ifndef MENU_SCREEN_H
#define MENU_SCREEN_H

typedef struct {
  Texture2D venmo_img;
} Menu_Screen;

void init_menu_screen(Menu_Screen* menu_screen);
void render_menu_screen(Menu_Screen* menu_screen);

#endif // MENU_SCREEN_H
