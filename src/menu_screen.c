#include "outwit.h"
#include "menu_screen.h"

void init_menu_screen(Menu_Screen* menu_screen) {
    menu_screen->venmo_img = LoadTexture("resources/venmo.png");
}

void render_menu_screen(Menu_Screen* menu_screen) {
    DrawText("Battle", 22, 25, 20, YELLOW);
    DrawText("Report Issue", 22, 50, 20, WHITE);
    DrawText("Credits", 22, 75, 20, WHITE);
    DrawText("@Tom-Holovacko-1", 22, 180, 17, WHITE);
    DrawTexture(menu_screen->venmo_img, 22, 200, WHITE);
}
