#include "vpet.h"
int ScreenWidth, ScreenHeight, ScreenZoom = 6;

SDL_Window *window = NULL;
SDL_Renderer *ScreenRenderer = NULL;
int quit = 0;
int retraces = 0;
SDL_Texture *GameSheet = NULL;
SDL_Texture *VpetBackground = NULL;

void run_game();
void init_game();

int main(int argc, char *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	ScreenWidth = 48 * ScreenZoom;
	ScreenHeight = 32 * ScreenZoom;

	window = SDL_CreateWindow("Virtual pet!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
	if(!window) {
		 SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "Window could not be created! SDL_Error: %s", SDL_GetError());
		 return -1;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
		return -1;
	}
	if( TTF_Init() == -1 ) {
		SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "SDL_ttf could not initialize! SDL_ttf Error: %s", TTF_GetError());
		return -1;
	}
	ScreenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	// ------------------------------------------------------

	//GameSheet = LoadTexture("data/puzzle.png", 0);
	VpetBackground = LoadTexture("data/background.png", 0);

	init_game();

	vpet_clear_screen();

	vpet_draw_pet(0, 10, 0,  CHARACTER_MIMI,    CF_HAPPY);
	vpet_draw_pet(16, 10, 0, CHARACTER_PYONKO,  CF_HAPPY);
	vpet_draw_pet(32, 10, 0, CHARACTER_TERRIER, CF_HAPPY);

	vpet_draw_textf(17, 2, "Cute");

	SDL_Event e;

	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT)
				quit = 1;
		}

//		SDL_SetRenderDrawColor(ScreenRenderer, 255, 255, 255, 255);
//		SDL_RenderClear(ScreenRenderer);
		SDL_RenderCopy(ScreenRenderer, VpetBackground, NULL, NULL);
		vpet_render_screen();

		//run_game();
		SDL_RenderPresent(ScreenRenderer);

		SDL_Delay(17);
		retraces++;
	}
	SDL_Quit();

	return 0;
}
