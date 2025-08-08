#include "renderer.h"
#include "game.h"

void render_map(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);
}

void render_player(SDL_Renderer *renderer, SDL_Rect player_rect) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &player_rect);
}

void render_ball(SDL_Renderer *renderer, SDL_Rect ball_rect) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);
}

void render_text(Game *game, TTF_Font *font, char *text, int x, int y, int size_x, int size_y) {
	SDL_Color white = {255, 255, 255};
	
	SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, white);
	if(!text_surface) {
		fprintf(stderr, "Text render error: %s\n", TTF_GetError());
	}
	SDL_Texture *text_texture = SDL_CreateTextureFromSurface(game->renderer, text_surface);
	SDL_FreeSurface(text_surface);
	
	SDL_Rect text_rect = {x, y, size_x, size_y};
	SDL_QueryTexture(text_texture, NULL, NULL, &text_rect.w, &text_rect.h);
	SDL_RenderCopy(game->renderer, text_texture, NULL, &text_rect);
	
	SDL_DestroyTexture(text_texture);
}

void render_game_over_state(Game *game, Player *player_l, Player *player_r) {
	TTF_Font *font = TTF_OpenFont("assets/texts/arial.ttf", 24);
	if(!font) {
		fprintf(stderr, "Failed to get font: %s\n", TTF_GetError());
	}

	render_text(game, font, "GAME OVER", SCREEN_WIDTH/2-PLAYER_HEIGHT/2, SCREEN_HEIGHT/2-PLAYER_WIDTH/2, 5, 5);
	
	if(player_l->win) {
			render_text(game, font, "PLAYER LEFT WINS", SCREEN_WIDTH*1/4, SCREEN_HEIGHT*3/4, 5, 5);
	}
	if(player_r->win) {
			render_text(game, font, "PLAYER RIGHT WINS", SCREEN_WIDTH*1/4, SCREEN_HEIGHT*3/4, 5, 5);
	}
	render_text(game, font, "Press ENTER to continue...", SCREEN_WIDTH*1/2, SCREEN_HEIGHT*9/10, 5, 5);
	TTF_CloseFont(font);	
}

void render_running_state(Game *game, Player *player_l, Player *player_r, Ball *ball) {
	render_map(game->renderer);
	render_ball(game->renderer, ball->ball_rect);
	render_player(game->renderer, player_l->player_rect);
	render_player(game->renderer, player_r->player_rect);
	
	TTF_Font *font = TTF_OpenFont("assets/texts/arial.ttf", 24);
	if (!font) {
		fprintf(stderr, "Failed to load font: %s\n", TTF_GetError());
		return;
	}

	char score_l[10];
	char score_r[10];
	snprintf(score_l, sizeof(score_l), "%d", player_l->score);
	snprintf(score_r, sizeof(score_r), "%d", player_r->score);


	render_text(game, font, score_l, SCREEN_WIDTH / 4, 20, 50, 50);


	render_text(game, font, score_r, SCREEN_WIDTH * 3 / 4, 20, 50, 50);

	TTF_CloseFont(font);
}

void render_start_state(Game *game) {
	TTF_Font *font = TTF_OpenFont("assets/texts/arial.ttf", 24);
	if(!font) {
		fprintf(stderr, "Failed to get font: %s\n", TTF_GetError());
	}

	render_text(game, font, "PONG", SCREEN_WIDTH/2-PLAYER_HEIGHT/2, SCREEN_HEIGHT/2-PLAYER_WIDTH/2, 5, 5);
	render_text(game, font, "Press ENTER to continue...", SCREEN_WIDTH*1/2, SCREEN_HEIGHT*3/4, 5, 5);
	
	TTF_CloseFont(font);
}

void render_screen(Game *game, Player *player_l, Player *player_r, Ball *ball) {
	switch(game->state) {
		case RUNNING_STATE:
			render_running_state(game, player_l, player_r, ball);
			break;
		case START_STATE:
			render_start_state(game);
			break;
		case GAME_OVER_STATE:
			render_game_over_state(game, player_l, player_r);
	}
}
