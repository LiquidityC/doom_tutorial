#include <SDL3/SDL.h>
#include "common.h"

#define res     2
#define SW      160*res
#define SH      120*res
#define HSW     SW/2.0
#define HSH     SH/2.0
#define FPS     30
#define FREQ    1000.0/FPS

static void draw(SDL *sdl, Player *p)
{
    int wx1, wx2, wy1, wy2;
    int wx[2], wy[2];
    float CS = cos(rad(p->a));
    float SN = sin(rad(p->a));

    /* Define a wall */
    wx1 = 0;
    wy1 = 20;
    wx2 = 40;
    wy2 = 20;

    /* Offset the wall according to player pos */
    wx1 -= p->x;
    wx2 -= p->x;
    wy1 -= p->y;
    wy2 -= p->y;

    /* Rotate wall according to player pos */
    wx[0] = wx1*CS-wy1*SN;
    wy[0] = wy1*CS+wx1*SN;

    wx[1] = wx2*CS-wy2*SN;
    wy[1] = wy2*CS+wx2*SN;

    /* Convert to screen coordinares (center at 0x0)
     * Note that SDL has 0x0 in top left and y increases downwards */
    wx[0] += HSW;
    wx[1] += HSW;
    wy[0] = 0 - wy[0] + HSH;
    wy[1] = 0 - wy[1] + HSH;

    /* Render wall */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 0, 0, 255);
    SDL_RenderLine(sdl->renderer, wx[0], wy[0], wx[1], wy[1]);

    /* Render player (static) */
    SDL_SetRenderDrawColor(sdl->renderer, 255, 255, 255, 255);
    SDL_RenderPoint(sdl->renderer, HSW, HSH);

    /* Render direction indicator (static) */
    SDL_SetRenderDrawColor(sdl->renderer, 0, 255, 0, 255);
    SDL_RenderPoint(sdl->renderer, HSW, HSH - 5);
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
        ctrl.xrot = 0;
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
