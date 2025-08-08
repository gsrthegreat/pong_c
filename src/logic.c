#include "logic.h"
#include "game.h"
#include "sound_manager.h"

void reset_ball(Ball *ball) {
	ball->ball_rect.x = BALL_STARTPOS_X;
	ball->ball_rect.y = BALL_STARTPOS_Y;
	ball->ball_vel = BALL_VELOCITY/3;


	int r = rand() % 4;
	switch (r) {
		case 0: ball->dir = TOP_LEFT; break;
		case 1: ball->dir = TOP_RIGHT; break;
		case 2: ball->dir = BOTTOM_LEFT; break;
		case 3: ball->dir = BOTTOM_RIGHT; break;
	}
}

void reset_players(Player *player_l, Player *player_r) {
	player_l->player_rect.x = PLAYER_L_STARTPOS_X;
	player_l->player_rect.y = PLAYER_L_STARTPOS_Y;
	player_l->score = 0;
	player_l->win = false;
	
	player_r->player_rect.x = PLAYER_R_STARTPOS_X;
	player_r->player_rect.y = PLAYER_R_STARTPOS_Y;
	player_r->score = 0;
	player_r->win = false;
}

void handle_collision(Game *game, Ball *ball, Player *player_l, Player *player_r) {
   
    int screen_width = SCREEN_WIDTH;
    int screen_height = SCREEN_HEIGHT;


    if (ball->ball_rect.y <= 0) {
        if (ball->dir == TOP_LEFT) {
            ball->dir = BOTTOM_LEFT;
        } else if (ball->dir == TOP_RIGHT) {
            ball->dir = BOTTOM_RIGHT;
        }
        play_sound("hit");
    }


    if (ball->ball_rect.y + ball->ball_rect.h >= screen_height) {
        if (ball->dir == BOTTOM_LEFT) {
            ball->dir = TOP_LEFT;
        } else if (ball->dir == BOTTOM_RIGHT) {
            ball->dir = TOP_RIGHT;
        }
        play_sound("hit");
    }


    if (SDL_HasIntersection(&ball->ball_rect, &player_l->player_rect)) {
        ball->ball_vel = BALL_VELOCITY;
        if (ball->dir == TOP_LEFT) {
            ball->dir = TOP_RIGHT;
        } else if (ball->dir == BOTTOM_LEFT) {
            ball->dir = BOTTOM_RIGHT;
        }
        play_sound("hit");
    }


    if (SDL_HasIntersection(&ball->ball_rect, &player_r->player_rect)) {
    	ball->ball_vel = BALL_VELOCITY;
        if (ball->dir == TOP_RIGHT) {
            ball->dir = TOP_LEFT;
        } else if (ball->dir == BOTTOM_RIGHT) {
            ball->dir = BOTTOM_LEFT;
        }
        play_sound("hit");
    }


    if (ball->ball_rect.x <= 0) {
        player_r->score++;
        reset_ball(ball);
      	play_sound("score");
    }


    if (ball->ball_rect.x + ball->ball_rect.w >= screen_width) {
        player_l->score++;
        reset_ball(ball);
        play_sound("score");
    }
}


void update_ball(Game *game, Player *player_l, Player *player_r, Ball *ball) {
	switch(ball->dir) {
		case TOP_LEFT:
			ball->ball_rect.x -= ball->ball_vel/2;
			ball->ball_rect.y -= ball->ball_vel/2;
			break;
		case TOP_RIGHT:
			ball->ball_rect.x += ball->ball_vel/2;
			ball->ball_rect.y -= ball->ball_vel/2;
			break;
		case BOTTOM_RIGHT:
			ball->ball_rect.x += ball->ball_vel/2;
			ball->ball_rect.y += ball->ball_vel/2;
			break;
		case BOTTOM_LEFT:
			ball->ball_rect.x -= ball->ball_vel/2;
			ball->ball_rect.y += ball->ball_vel/2;
	}
	handle_collision(game, ball, player_l, player_r);
}

void update_player(Game *game, Player *player_l, Player *player_r) {
	if(game->keystate[SDL_SCANCODE_W] && player_l->player_rect.y > 0) {
		player_l->player_rect.y -= game->player_vel;
	}
	if(game->keystate[SDL_SCANCODE_S] && (player_l->player_rect.y+PLAYER_HEIGHT) <= SCREEN_HEIGHT) {
		player_l->player_rect.y += game->player_vel;
	}
	if(game->keystate[SDL_SCANCODE_UP] && player_r->player_rect.y > 0) {
		player_r->player_rect.y -= game->player_vel;
	}
	if(game->keystate[SDL_SCANCODE_DOWN] && (player_r->player_rect.y+PLAYER_HEIGHT) <= SCREEN_HEIGHT) {
		player_r->player_rect.y += game->player_vel;
	}
	
	if((player_l->score - player_r->score) > 1 || player_l->score >= 3) {
		player_l->win = true;
		game->state = GAME_OVER_STATE;	
		play_sound("end");
	}
	if((player_r->score - player_l->score) > 1 || player_r->score >= 3) {
		player_r->win = true;
		game->state = GAME_OVER_STATE;
		play_sound("end");
	}
}

void start_game(Game *game) {
	if(game->keystate[SDL_SCANCODE_RETURN] && game->state == START_STATE) {
		//sleep(1);
		game->state = RUNNING_STATE;
		play_sound("start");
	}
}

void update_game(Game *game, Player *player_l, Player *player_r, Ball *ball) {
	switch(game->state) {
		case RUNNING_STATE:
			update_ball(game, player_l, player_r, ball);
			update_player(game, player_l, player_r);
			break;
		case START_STATE:
			start_game(game);
			break;
		case GAME_OVER_STATE:
			if(game->keystate[SDL_SCANCODE_RETURN]) {
				reset_ball(ball);
				reset_players(player_l, player_r);
				sleep(1);
				game->state = START_STATE;
			}
	}
}
