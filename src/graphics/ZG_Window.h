/* Define the SDL window structure, corresponding to toplevel windows */
struct SDL_Window
{
    const void *magic;
    Uint32 id;
    char *title;
    SDL_Surface *icon;
    int x, y;
    int w, h;
    int min_w, min_h;
    int max_w, max_h;
    Uint32 flags;
    Uint32 last_fullscreen_flags;
    Uint32 display_index;

    /* Stored position and size for windowed mode */
    SDL_Rect windowed;

    SDL_DisplayMode fullscreen_mode;

    float opacity;

    float brightness;
    Uint16 *gamma;
    Uint16 *saved_gamma;        /* (just offset into gamma) */

    SDL_Surface *surface;
    SDL_bool surface_valid;

    SDL_bool is_hiding;
    SDL_bool is_destroying;
    SDL_bool is_dropping;       /* drag/drop in progress, expecting SDL_SendDropComplete(). */

    SDL_Rect mouse_rect;

    SDL_WindowShaper *shaper;

    SDL_HitTest hit_test;
    void *hit_test_data;

    SDL_WindowUserData *data;

    void *driverdata;

    SDL_Window *prev;
    SDL_Window *next;
};
