#ifndef MX_REND_H
#define MX_REND_H

#include <stdint.h>

#if defined(MX_REND_SDL) + defined(MX_REND_TEST) != 1
#error Only one rendering engine should be defined
#endif

#ifdef MX_REND_SDL
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

extern const int      MX_WINDOWPOS_CENTERED;
extern const uint32_t MX_WINDOW_FULLSCREEN;
extern const uint32_t MX_WINDOW_BORDERLESS;
extern const uint32_t MX_WINDOW_RESIZABLE;
extern const uint32_t MX_WINDOW_MINIMIZED;
extern const uint32_t MX_WINDOW_MAXIMIZED;
extern const uint32_t MX_DEVICE_SOFTWARE;
extern const uint32_t MX_DEVICE_ACCELERATED;

#ifdef MX_REND_SDL
#define MX_EVENT_QUIT SDL_QUIT
#endif

#ifdef MX_REND_SDL
typedef SDL_Event   mx_event_t;
typedef SDL_Color   mx_color_t;
typedef TTF_Font    mx_font_t;
typedef SDL_Rect    mx_rect_t;
typedef SDL_Surface mx_surface_t;
typedef SDL_Texture mx_texture_t;
#endif

int mx_graphics_init();
void mx_graphics_free();

typedef struct mx_window mx_window_t;

mx_window_t* mx_window_new(
    const char* title,
    int x,
    int y,
    int w,
    int h,
    uint32_t wflags);

void mx_window_free(mx_window_t* p);


typedef struct mx_device mx_device_t;

mx_device_t* mx_device_new(mx_window_t* w, uint32_t flags);

void mx_device_free(mx_device_t* p);

void mx_device_clear(mx_device_t* device);

void mx_device_render_texture(
    mx_device_t* device,
    mx_texture_t* texture,
    mx_rect_t rect);

mx_texture_t* mx_device_render_surface(
    mx_device_t* device,
    mx_surface_t* surface);

void mx_device_show(mx_device_t* device);


int mx_event_wait(mx_event_t* e);

int mx_event_type(mx_event_t* e);


mx_font_t* mx_font_open(const char* path, uint32_t size);

void mx_font_close(mx_font_t* p);

mx_surface_t* mx_font_render(
    mx_font_t* font,
    mx_color_t color,
    const char* txt);


void mx_surface_free(mx_surface_t* p);

#endif //! MX_REND_H
