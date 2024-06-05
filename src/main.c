#include "raylib.h"
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

void update_draw_frame(void);
EM_BOOL resize_callback(int event_type, const EmscriptenUiEvent *ui_event, void *user_data);
void request_fullscreen();

int main(void)
{
    // get current screen width and height
    {
        double width;
        double height;
        emscripten_get_element_css_size("canvas", &width, &height);
        int screen_width  = (int)width;
        int screen_height = (int)height;
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        InitWindow(screen_width, screen_height, GAME_TITLE);
    }

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, resize_callback);
    emscripten_set_main_loop(update_draw_frame, 0, 1);

    // this code should never be reached
    CloseWindow();  // close window and OpenGL context

    return 0;
}

void update_draw_frame(void)
{
    BeginDrawing();
        ClearBackground(BLACK);

        int screen_width  = GetScreenWidth();
        int screen_height = GetScreenHeight();

        const char *text = "Hello, World!";
        int font_size    = 20;

        int text_width = MeasureText(text, font_size);

        int pos_x = (screen_width  - text_width) / 2;
        int pos_y = (screen_height - font_size)  / 2;

        DrawText(text, pos_x, pos_y, font_size, LIGHTGRAY);

    EndDrawing();
}

EM_BOOL resize_callback(int event_type, const EmscriptenUiEvent *ui_event, void *user_data) {
    double width;
    double height;

    emscripten_get_element_css_size("canvas", &width, &height);
    int screen_width  = (int)width;
    int screen_height = (int)height;
    SetWindowSize(screen_width, screen_height);

    return EM_TRUE;
}

void request_fullscreen() {
    EM_ASM({
        var canvas = Module['canvas'];
        if (canvas.requestFullscreen) {
            canvas.requestFullscreen();
        } else if (canvas.mozRequestFullScreen) { // Firefox
            canvas.mozRequestFullScreen();
        } else if (canvas.webkitRequestFullScreen) { // Chrome, Safari and Opera
            canvas.webkitRequestFullScreen();
        } else if (canvas.msRequestFullscreen) { // IE/Edge
            canvas.msRequestFullscreen();
        }
    });
}
