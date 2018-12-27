#ifdef MX_REND_SDL

#include "rend.h"

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

const int MX_WINDOWPOS_CENTERED         = SDL_WINDOWPOS_CENTERED;

const uint32_t MX_WINDOW_FULLSCREEN     = SDL_WINDOW_FULLSCREEN;
const uint32_t MX_WINDOW_BORDERLESS     = SDL_WINDOW_BORDERLESS;
const uint32_t MX_WINDOW_RESIZABLE      = SDL_WINDOW_RESIZABLE;
const uint32_t MX_WINDOW_MINIMIZED      = SDL_WINDOW_MINIMIZED;
const uint32_t MX_WINDOW_MAXIMIZED      = SDL_WINDOW_MAXIMIZED;

const uint32_t MX_DEVICE_SOFTWARE       = SDL_RENDERER_SOFTWARE;
const uint32_t MX_DEVICE_ACCELERATED    = SDL_RENDERER_ACCELERATED;

#define alloc(T) (T*)malloc(sizeof(T))


int mx_graphics_init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("error: %s", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        SDL_Log("error: %s", SDL_GetError());
        return 1;
    }
    return 0;
}

void mx_graphics_free() {
    TTF_Quit();
    SDL_Quit();
}

struct mx_window {
    SDL_Window* native;
};

mx_window_t* mx_window_new(
        const char* title,
        int x,
        int y,
        int w,
        int h,
        uint32_t flags) {

    SDL_Window* native = SDL_CreateWindow(title, x, y, w, h, flags);
    if (!native) {
        return NULL;
    }

    mx_window_t* window = alloc(mx_window_t);
    window->native = native;
    return window;
}

void mx_window_free(mx_window_t* p) {
    SDL_DestroyWindow(p->native);
    free(p);
}

struct mx_device {
    SDL_Renderer* native;
};

mx_device_t* mx_device_new(mx_window_t* window, uint32_t flags) {
    SDL_Renderer* native = SDL_CreateRenderer(window->native, -1, flags);
    if (!native) {
        return NULL;
    }

    mx_device_t* device = alloc(mx_device_t);
    device->native = native;
    return device;
}

void mx_device_free(mx_device_t* p) {
    SDL_DestroyRenderer(p->native);
    free(p);
}

void mx_device_clear(mx_device_t* device) {
    SDL_RenderClear(device->native);
}

void mx_device_render_texture(
        mx_device_t* device,
        mx_texture_t* texture,
        mx_rect_t rect) {

    SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(device->native, texture, NULL, &rect);
}

mx_texture_t* mx_device_render_surface(
        mx_device_t* device,
        mx_surface_t* surface) {

    return (mx_texture_t*)SDL_CreateTextureFromSurface(
        device->native,
        surface);
}

void mx_device_show(mx_device_t* device) {
    SDL_RenderPresent(device->native);
}

int mx_event_wait(mx_event_t* e) {
    return SDL_WaitEvent((SDL_Event*)e);
}

int mx_event_type(mx_event_t* e) {
    return ((SDL_Event*)e)->type;
}

mx_font_t* mx_font_open(const char* path, uint32_t size) {
    return (mx_font_t*)TTF_OpenFont(path, size);
}

void mx_font_close(mx_font_t* p) {
    TTF_CloseFont((TTF_Font*)p);
}

mx_surface_t* mx_font_render(
        mx_font_t* font,
        mx_color_t color,
        const char* text) {

    return (mx_surface_t*)TTF_RenderText_Blended(font, text, color);
}


void mx_surface_free(mx_surface_t* p) {
    SDL_FreeSurface((SDL_Surface*)p);
}

#endif //! MX_REND_SDL
