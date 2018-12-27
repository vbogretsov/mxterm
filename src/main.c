#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gc/gc.h>

#include "rend/rend.h"

#define TITLE "MxTerm"

#define SUCCES 0
#define FATAL  1

#define FONT_FILE "/Users/vova/Library/Fonts/Hack-Regular.ttf"
#define FONT_SIZE 16

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600

const mx_color_t FG = { 255, 255, 255, 255 };


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


static int start(mx_window_t* window, mx_device_t* device) {
    bool running = true;

    mx_texture_t* text = render_text(device, "Hello, world!");
    mx_rect_t rect = { .x = 0, .y = 0 };

    mx_event_t e;
    while (running) {
        if (mx_event_wait(&e)) {
            // fprintf(stderr, "event: %d\n", mx_event_type(&e));
            switch (mx_event_type(&e)) {
                case MX_EVENT_QUIT:
                    running = false;
                    break;
                default:
                    break;
            }
        }

        mx_device_clear(device);
        mx_device_render_texture(device, text, rect);
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
            MX_WINDOW_RESIZABLE);

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
