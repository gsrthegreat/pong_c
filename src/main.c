#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "renderer.h"
#include "logic.h"
#include "sound_manager.h"

int sdl_initialize(Game *game) {
	if(SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
		return 1;
	}
	game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(!game->window) {
		fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
		return 1;
	}
	game->renderer = SDL_CreateRenderer(game->window, -1, 0);
	if(!game->renderer) {
		fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
		return 1;
	}
	if(TTF_Init()) {
		fprintf(stderr, "Error initializing SDL font library: %s\n", TTF_GetError());
		return 1;
	}
	if(!init_sound_manager()) {
		return 1;
	}
	load_sound("start", "assets/music/game-start.wav");
	load_sound("end", "assets/music/game-over.mp3");
	load_sound("hit", "assets/music/hit.mp3");
	load_sound("score", "assets/music/score.wav");
	return 0;
}

void game_cleanup(Game *game, int EXIT_STATUS) {
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	TTF_Quit();
	close_sound_manager();
	SDL_Quit();
	exit(EXIT_STATUS);
}

int main() {
	Game game = {
		.window = NULL,
		.renderer = NULL,
		.state = START_STATE,
		.keystate = SDL_GetKeyboardState(NULL),
		.player_vel = PLAYER_VELOCITY,
	};
	Player player_l = {
		.player_rect = {PLAYER_L_STARTPOS_X, PLAYER_L_STARTPOS_Y, PLAYER_WIDTH, PLAYER_HEIGHT},
		.score = 0,
		.win = false
	};
	Player player_r = {
		.player_rect = {PLAYER_R_STARTPOS_X, PLAYER_R_STARTPOS_Y, PLAYER_WIDTH, PLAYER_HEIGHT},
		.score = 0,
		.win = false
	};
	Ball ball = {
		.ball_rect = {BALL_STARTPOS_X, BALL_STARTPOS_Y, BALL_WIDTH, BALL_HEIGHT},
		.ball_vel = BALL_VELOCITY/3,
		.dir = TOP_RIGHT
	}; 
	int r = rand() % 4;
	switch (r) {
		case 0: ball.dir = TOP_LEFT; break;
		case 1: ball.dir = TOP_RIGHT; break;
		case 2: ball.dir = BOTTOM_LEFT; break;
		case 3: ball.dir = BOTTOM_RIGHT; break;
	}
	if(sdl_initialize(&game)) {
		game_cleanup(&game, EXIT_FAILURE);
	}
	srand(time(NULL));
	while(1) {
		SDL_Event e;
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT) {
				game_cleanup(&game, EXIT_SUCCESS);
			}
		}
		update_game(&game, &player_l, &player_r, &ball);
		SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
		SDL_RenderClear(game.renderer);
		render_screen(&game, &player_l, &player_r, &ball);
		SDL_RenderPresent(game.renderer);
		SDL_Delay(16);
	}	
	return 0;
}
