#include <SDL3/SDL.h>
#include "common.h"

#define res     2
#define SW      160*res
#define SH      120*res
#define HSW     SW/2
#define HSH     SH/2
#define FPS     30
#define FREQ    1000.0/FPS

static void draw(SDL *sdl, Player *p)
{
    int wx1, wx2, wy1, wy2, px, py, rx, ry;
    float dx;
    float dy;

    /* Define a wall */
    wx1 = -40;
    wx2 = 40;
    wy1 = 30;
    wy2 = 30;

    /* Player direction indicator */
    dx = sin(rad(p->a))*5.0;
    dy = cos(rad(p->a))*5.0;
    rx = p->x + dx;
    ry = p->y + dy;

    /* Convert to screen coordinares (center at 0x0)
     * Note that SDL has 0x0 in top left and y increases downwards */
    wx1 += HSW;
    wx2 += HSW;
    wy1 = 0 - wy1 + HSH;
    wy2 = 0 - wy2 + HSH;

    /* Convert player pos */
    px = p->x + HSW;
    py = 0 - p->y + HSH;

    /* Convert the direction indicator */
    rx += HSW;
    ry = 0 - ry + HSH;

    /* Render wall */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 0, 0, 255);
    SDL_RenderLine(sdl->renderer, wx1, wy1, wx2, wy2);

    /* Render player */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    SDL_RenderPoint(sdl->renderer, px, py);

    /* Render direction indicator */
    SDL_SetRenderDrawColor(sdl->renderer, 0, 255, 0, 255);
    SDL_RenderPoint(sdl->renderer, rx, ry);
}

int main(void)
{
    SDL sdl;
    sdl_init(&sdl, SW, SH, 4.0);

    bool quit = false;
    Player p = {0};
    p.x = 0;
    p.y = 0;
    Controller ctrl = {0};

    SDL_Event e;
    Uint64 start, end;
    float elapsed;
    while (!quit) {
        start = SDL_GetPerformanceCounter();
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            controller_input(&ctrl, &e);
        }

        player_move(&p, &ctrl);

        SDL_SetRenderDrawColor(sdl.renderer, 0, 0, 0, 255);
        SDL_RenderClear(sdl.renderer);

        draw(&sdl, &p);

        SDL_RenderPresent(sdl.renderer);

        end = SDL_GetPerformanceCounter();
        elapsed = (end - start)/(float)SDL_GetPerformanceFrequency() * 1000.0;
        if (elapsed < FREQ) {
            SDL_Delay(FREQ - elapsed);
        }
    }

    sdl_quit(&sdl);
}
