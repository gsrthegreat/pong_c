#ifndef LOGIC_H_
#define LOGIC_H_

#include "game.h"
#include "sound_manager.h"

#include <unistd.h>

void update_game(Game *game, Player *player_l, Player *player_r, Ball *ball);

#endif //LOGIC_H_
