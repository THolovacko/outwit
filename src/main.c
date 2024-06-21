#include "outwit.h"
#include "menu_screen.h"
#include <unistd.h>
#include <emscripten/wasm_worker.h>

void update_draw_frame(void);
EM_BOOL resize_callback(int event_type, const EmscriptenUiEvent *ui_event, void *user_data);
void request_fullscreen();
void draw_texture_with_size(Texture2D texture, const Vector2 origin, const Vector2 size);

RenderTexture2D render_texture;
//Music music;
//float timePlayed = 0.0f;  // time played normalized [0.0f..1.0f]

Menu_Screen menu_screen;

#define RESOLUTION_WIDTH  808
#define RESOLUTION_HEIGHT 414

/*/
void load_game_task() {
    sleep(3);
    init_menu_screen(&menu_screen);
    is_loading_mode = false;
}
*/

int main(void)
{
    // get current screen width and height
    {
        double width;
        double height;
        emscripten_get_element_css_size("canvas", &width, &height);
        int screen_width  = (int)width;
        int screen_height = (int)height;
        SetConfigFlags(FLAG_MSAA_4X_HINT);
        SetWindowState(FLAG_WINDOW_RESIZABLE);
        InitWindow(screen_width, screen_height, GAME_TITLE);
    }

    render_texture = LoadRenderTexture(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    SetTextureFilter(render_texture.texture, TEXTURE_FILTER_BILINEAR);

    //InitAudioDevice();
    //music = LoadMusicStream("resources/song18.mp3");
    //PlayMusicStream(music);

    init_menu_screen(&menu_screen);

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, EM_FALSE, resize_callback);
    emscripten_set_main_loop(update_draw_frame, 0, 1);

    // this code should never be reached
    CloseWindow();  // close window and OpenGL context

    return 0;
}

void update_draw_frame(void)
{
    //UpdateMusicStream(music);
    //timePlayed = GetMusicTimePlayed(music)/GetMusicTimeLength(music);
    //if (timePlayed > 1.0f) timePlayed = 1.0f;

    BeginTextureMode(render_texture);
    {
        ClearBackground(BLACK);

        //const int resolution_width  = render_texture.texture.width;
        //const int resolution_height = render_texture.texture.height;

        render_menu_screen(&menu_screen);
    }
    EndTextureMode();

    BeginDrawing();
        Rectangle source_rec = { 0.0f, 0.0f, (float)render_texture.texture.width, ((float)render_texture.texture.height) * -1.0f };
        Rectangle dest_rec   = { 0.0f, 0.0f, GetScreenWidth(), GetScreenHeight() };
        DrawTexturePro(render_texture.texture, source_rec, dest_rec, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);

        #ifdef DEBUG_MODE
        char screenSizeText[64];
        snprintf(screenSizeText, sizeof(screenSizeText), "Screen Width: %d, Screen Height: %d", GetScreenWidth(), GetScreenHeight());
        DrawText(screenSizeText, 10, 10, 20, YELLOW);

        DrawFPS(10, 50);
        #endif
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

void draw_texture_with_size(Texture2D texture, const Vector2 origin, const Vector2 size) {
    Rectangle source_rec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
    Rectangle dest_rec   = { 0.0f, 0.0f, size.x, size.y };
    DrawTexturePro(texture, source_rec, dest_rec, origin, 0.0f, WHITE);
}
