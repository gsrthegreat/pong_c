#ifndef GAME_H_
#define GAME_H_

#include <SDL2/SDL.h>
#include <time.h>
#include <stdbool.h>

#define TIMER 60
#define WINDOW_TITLE "pong"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800

#define PLAYER_L 0
#define PLAYER_R 1
#define PLAYER_HEIGHT 150
#define PLAYER_WIDTH 20
#define PLAYER_VELOCITY 10
#define PLAYER_L_STARTPOS_X 0
#define PLAYER_L_STARTPOS_Y (SCREEN_HEIGHT/2)-(PLAYER_HEIGHT/2)
#define PLAYER_R_STARTPOS_X SCREEN_WIDTH - PLAYER_WIDTH
#define PLAYER_R_STARTPOS_Y (SCREEN_HEIGHT/2)-(PLAYER_HEIGHT/2)

#define BALL_WIDTH 15
#define BALL_HEIGHT 15
#define BALL_VELOCITY 21
#define BALL_STARTPOS_X (SCREEN_WIDTH/2) - (BALL_WIDTH/2)
#define BALL_STARTPOS_Y (SCREEN_HEIGHT/2) - (BALL_HEIGHT/2) 

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT 3

#define RUNNING_STATE 0
#define START_STATE 1
#define GAME_OVER_STATE 2

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	int state;
	const Uint8 *keystate;
	int player_vel;
} Game;

typedef struct {
	SDL_Rect ball_rect;
	int ball_vel;
	int dir;
} Ball;

typedef struct {
	//int player;
	SDL_Rect player_rect;
	int score;
	bool win;
} Player;

#endif //GAME_H_
