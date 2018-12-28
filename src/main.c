#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gc/gc.h>

#include "rend/rend.h"

#define TITLE "MxTerm"

#define SUCCES 0
#define FATAL  1

// #define FONT_FILE "/Users/vova/Library/Fonts/SFMono Light Nerd Font Complete.otf"
// #define FONT_FILE "/Users/vova/Library/Fonts/SFMono Regular Nerd Font Complete.otf"
// #define FONT_FILE "/Users/vova/Library/Fonts/Roboto Mono for Powerline.ttf"
#define FONT_FILE "PTMono.ttc"
// #define FONT_FILE "/Users/vova/Downloads/SF-Mono-Regular-Nerd-Font-Complete.ttf"
#define FONT_SIZE 12

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

#define GLYPH_CACHE_SIZE 256

static const int GLYPH_MIN = 33;
static const int GLYPH_MAX = 127;

#define TEXT "vbogretsov@vova $ echo xxx! The C programming language is great!"

const mx_color_t FG = { 0x93, 0xa1, 0xa1, 0xFF };
const mx_color_t BG = { 0x00, 0x00, 0x00, 0x00 };


const int NUM_CHARS = 100;


mx_texture_t* render_text(mx_device_t* device, const char* text) {
    mx_texture_t* texture = NULL;

    gcalloc();
    do {
        mx_font_t* font = mx_font_open(FONT_FILE, FONT_SIZE);
        if (font == NULL) {
            break;
        }
        gcdefer(mx_font_close, font);

        mx_surface_t* surface = mx_font_render(font, FG, text);
        if (!surface) {
            break;
        }
        gcdefer(mx_surface_free, surface);

        texture = mx_device_render_surface(device, surface);
    } while (0);
    gcclean();

    return texture;
}


static int fill_glyph_cache(mx_device_t* device, mx_texture_t** cache, size_t len) {
    gcalloc();
    do {
        mx_font_t* font = mx_font_open(FONT_FILE, FONT_SIZE);
        if (font == NULL) {
            break;
        }
        gcdefer(mx_font_close, font);

        for (char c = GLYPH_MIN; c < GLYPH_MAX; ++c) {
            mx_surface_t* s = mx_glyph_render(font, c, FG, BG);
            if (!s) {
                fprintf(stderr, "error: unable to render glyph %c\n", c);
                break;
            }
            mx_texture_t* t = mx_device_render_surface(device, s);
            mx_surface_free(s);
            cache[c - GLYPH_MIN] = t;
        }
    } while (0);
    gcclean();

    return 0;
}


static int start(mx_window_t* window, mx_device_t* device) {
    bool running = true;

    // mx_texture_t* text = render_text(device, TEXT);
    mx_texture_t* glyph_cache[GLYPH_CACHE_SIZE];
    fill_glyph_cache(device, glyph_cache, GLYPH_CACHE_SIZE);

    srand(time(NULL));

    mx_event_t e;
    while (running && mx_event_wait(&e)) {
        // if (mx_event_wait(&e)) {
        //     // fprintf(stderr, "event: %d\n", mx_event_type(&e));
        //     switch (mx_event_type(&e)) {
        //         case MX_EVENT_QUIT:
        //             running = false;
        //             break;
        //         default:
        //             break;
        //     }
        // }
        switch (mx_event_type(&e)) {
            case MX_EVENT_QUIT:
                running = false;
                break;
            default:
                break;
        }

        // mx_rect_t a = { .x = 0, .y = 0 };
        // mx_rect_t b = { .x = 8, .y = 0 };
        // mx_rect_t c = { .x = 16, .y = 0 };
        // mx_rect_t rect = { .x = 0, .y = 0 };
        // mx_device_render_texture(device, glyph_cache['A' - 33], a);
        // mx_device_render_texture(device, glyph_cache['B' - 33], b);
        // mx_device_render_texture(device, glyph_cache['C' - 33], c);

        mx_device_clear(device);

        // for (int i = 0; i < NUM_CHARS; ++i) {
        //     // int x = abs(rand()) % 500;
        //     mx_rect_t r = { .x = abs(rand()) % 700, .y = abs(rand()) % 700 };
        //     char c = abs(rand()) % (GLYPH_MAX - GLYPH_MIN);
        //     mx_device_render_texture(device, glyph_cache[(int)c], r);
        // }

        int delta = 8;
        mx_device_render_texture(device, glyph_cache['A' - 33], (mx_rect_t){ .x = 5, .y = 5 });
        mx_device_render_texture(device, glyph_cache['W' - 33], (mx_rect_t){ .x = 5 + delta, .y = 5 });
        mx_device_render_texture(device, glyph_cache['b' - 33], (mx_rect_t){ .x = 5 + 2 * delta, .y = 5 });
        mx_device_render_texture(device, glyph_cache['c' - 33], (mx_rect_t){ .x = 5 + 3 * delta, .y = 5 });
        mx_device_render_texture(device, glyph_cache['!' - 33], (mx_rect_t){ .x = 5 + 4 * delta, .y = 5 });


        mx_device_show(device);
    }

    return 0;
}


int main(int argc, char *argv[]) {
    int status = SUCCES;

    gcalloc();
    do {
        if (mx_graphics_init() != 0) {
            status = FATAL;
            break;
        }
        gcdefer(mx_graphics_free, NULL);

        mx_window_t* window = mx_window_new(
            TITLE,
            MX_WINDOWPOS_CENTERED,
            MX_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            MX_WINDOW_RESIZABLE | MX_WINDOW_MAXIMIZED);

        if (!window) {
            status = FATAL;
            break;
        }
        gcdefer(mx_window_free, window);

        mx_device_t* device = mx_device_new(
            window,
            MX_DEVICE_SOFTWARE);

        if (!device) {
            status = FATAL;
            break;
        }
        gcdefer(mx_device_free, device);

        status = start(window, device);
    } while (0);
    gcclean();

    return status;
}
