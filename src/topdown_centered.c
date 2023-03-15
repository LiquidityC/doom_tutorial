#include "SDL_render.h"
#include "SDL_timer.h"
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
    int wx1, wx2, wy1, wy2, rx, ry;

    /* Define a wall */
    wx1 = 0; wy1 = 30;
    wx2 = 40; wy2 = 30;

    /* Offset the wall according to player pos */
    wx1 -= p->x;
    wx2 -= p->x;
    wy1 -= p->y;
    wy2 -= p->y;

    /* Player direction indicator */
    rx = sin(rad(p->a))*5.0;
    ry = cos(rad(p->a))*5.0;

    /* Convert to screen coordinares (center at 0x0)
     * Note that SDL has 0x0 in top left and y increases downwards */
    wx1 += HSW;
    wx2 += HSW;
    wy1 = 0 - wy1 + HSH;
    wy2 = 0 - wy2 + HSH;

    /* Convert the direction indicator */
    rx += HSW;
    ry = 0 - ry + HSH;

    /* Render wall */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 0, 0, 255);
    SDL_RenderDrawLine(sdl->renderer, wx1, wy1, wx2, wy2);

    /* Render player */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    SDL_RenderDrawPoint(sdl->renderer, HSW, HSH);

    /* Render direction indicator */
    SDL_SetRenderDrawColor(sdl->renderer, 0, 255, 0, 255);
    SDL_RenderDrawPoint(sdl->renderer, rx, ry);
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
            if (e.type == SDL_QUIT) {
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
