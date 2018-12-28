#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH  800

#define FONT_PATH "fonts/SF-Pro-Display-Regular.otf"
#define FONT_SIZE 13

static const SDL_Color FG = { 0x93, 0xa1, 0xa1, 0xFF };
static const SDL_Color BG = { 0x00, 0x2b, 0x36, 0xFF };


static void render_text(SDL_Surface* screen, const char* text, SDL_Rect rect) {
    TTF_Font* font = NULL;
    SDL_Surface* src = NULL;

    do {
        font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
        if (!font) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            break;
        }

        TTF_SetFontKerning(font, 0);
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);

        src = TTF_RenderText_Blended(font, text, FG);
        if (!src) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            break;
        }

        SDL_BlitSurface(src, NULL, screen, &rect);
    } while (0);

    if (src) {
        SDL_FreeSurface(src);
    }
    if (font) {
        TTF_CloseFont(font);
    }
}


static int demo(SDL_Window* window, SDL_Renderer* renderer) {
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);

    SDL_Surface* screen = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
    if (!screen) {
        return 1;
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, BG.r, BG.g, BG.b));

    int n = 0;

    render_text(screen, "SDL demo text!", (SDL_Rect) {
        .x = 2, .y = 2, w = 10, h = 10
    });

    bool running = true;

    SDL_Event e;
    while (SDL_WaitEvent(&e)) {
        if (e.type == SDL_QUIT) {
            break;
        }
        if (e.type == SDL_KEYDOWN) {
            n += FONT_SIZE;
            render_text(screen, "SDL demo text!", (SDL_Rect) {
                .x = 2, .y = 2 + n, w = 10, h = 10
            });
        }

        SDL_Texture* scene = SDL_CreateTextureFromSurface(renderer, screen);

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, scene, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(scene);
    }

    SDL_FreeSurface(screen);

    return 0;
}


int main(int argc, char *argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    int status = 0;
    do {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            break;
        }
        if (TTF_Init() != 0) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            break;
        }
        window = SDL_CreateWindow(
            "Demo Fonts",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_RESIZABLE);

        if (!window) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            status = 1;
            break;
        }

        renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_SOFTWARE);

        if (!renderer) {
            fprintf(stderr, "error: %s\n", SDL_GetError());
            status = 1;
            break;
        }

        status = demo(window, renderer);
    } while (0);

    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }

    TTF_Quit();
    SDL_Quit();

    return status;
}