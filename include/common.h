#ifndef _COMMON_H_
#define _COMMON_H_

#include <SDL3/SDL.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b
#define clamp(x, u, l) max(min(x, u), l)
#define rad(deg) (deg/180.0 * M_PI)

typedef struct SDL {
    bool initiated;
    SDL_Window *window;
    SDL_Renderer *renderer;
} SDL;

typedef struct Controller {
    bool forward;
    bool backward;
    bool left;
    bool right;
    bool up;
    bool down;
    int xrot;
    int yrot;
} Controller;

typedef struct Math {
    float sin[360];
    float cos[360];
} Math;

typedef struct Player {
    int x,y,z;  /* Position */
    int a;      /* Angle of rotation */
    int l;      /* Look up down */
} Player;

void sdl_init(SDL* sdl, int sw, int sh, float scale);

void sdl_quit(SDL* sdl);

void sdl_perror(char *func);

void sdl_fatal(char *func);

void math_init(Math *math);

void controller_input(Controller *ctrl, SDL_Event *e);

void player_move(Player *player, Controller *ctrl);

#endif // _COMMON_H_

