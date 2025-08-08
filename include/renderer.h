#ifndef RENDERER_H_
#define RENDERER_H_

#include <SDL2/SDL_ttf.h>

#include "./game.h"


void render_screen(Game *game, Player *player_l, Player *player_r, Ball *ball);

#endif //RENDERER_H_
