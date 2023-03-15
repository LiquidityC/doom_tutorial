#include "common.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_video.h"
#include <stdlib.h>

void sdl_perror(char *func)
{
    fprintf(stderr, "%s: %s", func, SDL_GetError());
    exit(1);
}

void sdl_fatal(char *func)
{
    sdl_perror(func);
    exit(EXIT_FAILURE);
}

void sdl_init(SDL *sdl, int sw, int sh, float scale)
{
    memset(sdl, 0, sizeof(SDL));
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        sdl_perror("SDL_Init");
    }

    sdl->window = SDL_CreateWindow("Doom", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sw*scale, sh*scale, 0);
    if (!sdl->window) {
        sdl_fatal("SDL_CreateWindow");
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl->renderer) {
        sdl_fatal("SDL_CreateWindow");
    }
    SDL_RenderSetScale(sdl->renderer, scale, scale);
    SDL_SetRelativeMouseMode(true);

    sdl->initiated = true;
}

void sdl_quit(SDL *sdl)
{
    if (sdl->initiated) {
        SDL_DestroyRenderer(sdl->renderer);
        SDL_DestroyWindow(sdl->window);
        SDL_Quit();
    }
}

void math_init(Math *math)
{
    memset(math, 0, sizeof(Math));
    for (size_t i = 0; i < 360; ++i) {
        math->sin[i] = sin(i/180.0*M_PI);
        math->cos[i] = cos(i/180.0*M_PI);
    }
}

void controller_input(Controller *ctrl, SDL_Event *e)
{
    ctrl->xrot = 0;
    ctrl->yrot = 0;

    if (e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        bool val = e->type == SDL_KEYDOWN;
        switch (e->key.keysym.sym) {
            case SDLK_w:
                ctrl->forward = val;
                break;
            case SDLK_s:
                ctrl->backward = val;
                break;
            case SDLK_e:
                ctrl->right = val;
                break;
            case SDLK_a:
                ctrl->left = val;
                break;
            case SDLK_d:
                ctrl->right = val;
                break;
            case SDLK_SPACE:
                ctrl->up = val;
                break;
            case SDLK_LCTRL:
                ctrl->down = val;
                break;
        }
    }
    if (e->type == SDL_MOUSEMOTION) {
        ctrl->xrot = e->motion.xrel * 2;
        ctrl->yrot = -e->motion.yrel * 2;
    }
}

void player_move(Player *player, Controller *ctrl)
{
    player->a += ctrl->xrot;

    /* Ensure that angle stays between 0-359 */
    if (player->a < 0)
        player->a += 360;
    if (player->a >= 360)
        player->a -= 360;

    /* Calculate movement vector from current angle */
    float dx = sin(rad(player->a))*5.0;
    float dy = cos(rad(player->a))*5.0;

    /* Move back and forth */
    if (ctrl->forward) { player->x += dx; player->y += dy; }
    if (ctrl->backward) { player->x -= dx; player->y -= dy; }

    /* Strafe left/right */
    if (ctrl->right) { player->x += dy; player->y -= dx; }
    if (ctrl->left) { player->x -= dy; player->y += dx; }

    player->l = clamp(player->l+ctrl->yrot, -120, 120);
    if (ctrl->up) { player->z -= 4; }
    if (ctrl->down) { player->z += 4; }
}
