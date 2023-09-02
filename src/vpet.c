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
	//VpetBackground = LoadTexture("data/meadow.jpg", 0);

	init_game();

	vpet_clear_screen();
	/*
	vpet_set_pixel(0, 0, 1);
	vpet_set_pixel(1, 1, 1);
	vpet_set_pixel(2, 2, 1);
	vpet_set_pixel(3, 3, 1);
	vpet_set_pixel(3, 4, 1);
	vpet_set_pixel(3, 5, 1);
	vpet_set_pixel(3, 6, 1);
	vpet_set_pixel(3, 7, 1);
	*/

	unsigned char test[8] = {
		0b00111100,
		0b01000010,
		0b10100101,
		0b10000001,
		0b10100101,
		0b10011001,
		0b01000010,
		0b00111100
	};

	uint16_t pyon[] = {
		0b0000000000000000,
		0b0000000000000000,
		0b0011000000011000,
		0b0010100000101000,
		0b0010011111001000,
		0b0010100001101000,
		0b0001010010111000,
		0b0010010010010000,
		0b0010000000010000,
		0b0010011000010000,
		0b0001000000010000,
		0b0001010100011000,
		0b0000101000101000,
		0b0000100000110000,
		0b0000010101000000,
		0b0000001010000000,
	};
	vpet_sprite_16(15, 10, 0, 16, pyon, vpet_xor_8_pixels);
	vpet_sprite_8(18, 24, 0, 8, test, vpet_xor_8_pixels);

	vpet_draw_textf(2, 2, "Hello World");
	vpet_draw_textf(24, 10, "Mario");

	SDL_Event e;

	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT)
				quit = 1;
		}

		//SDL_SetRenderDrawColor(ScreenRenderer, 64, 64, 64, 255);
		//SDL_RenderClear(ScreenRenderer);

//		SDL_SetRenderDrawColor(ScreenRenderer, 0, 0, 0, 255);
//		rectfill(ScreenRenderer, 6*16, 0, 2, 144);
//		SDL_SetRenderDrawColor(ScreenRenderer, 205, 228, 255, 255);
//		rectfill(ScreenRenderer, 0, 0, 6*16, 144);

		SDL_SetRenderDrawColor(ScreenRenderer, 255, 255, 255, 255);
		SDL_RenderClear(ScreenRenderer);

//		SDL_RenderCopy(ScreenRenderer, VpetBackground, NULL, NULL);
		vpet_render_screen();

		//run_game();
		SDL_RenderPresent(ScreenRenderer);

		SDL_Delay(17);
		retraces++;
	}
	SDL_Quit();

	return 0;
}
