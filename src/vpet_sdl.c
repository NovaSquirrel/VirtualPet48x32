/*
 * Monochrome LCD Virtual Pet Project
 *
 * Copyright (C) 2023 NovaSquirrel
 *
 * This program is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "vpet.h"
#define FRAME_LENGTH (16.0+2.0/3.0)

#ifdef PLATFORM_PC
int ScreenWidth, ScreenHeight, ScreenZoom = 6;

SDL_Window *window = NULL;
SDL_Renderer *ScreenRenderer = NULL;
SDL_Texture *VpetBackground = NULL;
int quit = 0;
int framecounter = 0;

// Keys
const Uint8 *keyboard;
uint16_t key_down = 0, key_new = 0, key_last = 0, key_new_or_repeat = 0;
int key_repeat = 0;

void init_game() {
	keyboard = SDL_GetKeyboardState(NULL);
	RandomSeed();

	void vpet_init();
	vpet_init();
}

int main(int argc, char *argv[]) {
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	ScreenWidth = PET_SCREEN_W * ScreenZoom;
	ScreenHeight = PET_SCREEN_H * ScreenZoom;

	window = SDL_CreateWindow("Virtual pet!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, ScreenWidth, ScreenHeight, SDL_WINDOW_SHOWN);
	if(!window) {
		 SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "Window could not be created! SDL_Error: %s", SDL_GetError());
		 return -1;
	}
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
		SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", NULL, "SDL_image could not initialize! SDL_image Error: %s", IMG_GetError());
		return -1;
	}
	ScreenRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	// ------------------------------------------------------

	VpetBackground = LoadTexture("data/background.png", 0);

	init_game();

	// Run the SDL event loop
	unsigned int last_time = SDL_GetTicks();
	double frametimer = 0; 

	SDL_Event e;
	while(!quit) {
		while(SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_QUIT)
				quit = 1;
		}

		SDL_RenderCopy(ScreenRenderer, VpetBackground, NULL, NULL);
		vpet_render_screen();
		if(SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) {
			SDL_Delay(16);
		} else {
			SDL_RenderPresent(ScreenRenderer);
		}

		unsigned int this_time = SDL_GetTicks();
		frametimer += this_time - last_time;
		last_time = this_time;
		while(frametimer >= FRAME_LENGTH) {
			frametimer -= FRAME_LENGTH;

			// Update keys
			key_last = key_down;
			key_down =   (keyboard[SDL_SCANCODE_LEFT]  << 0) |
						(keyboard[SDL_SCANCODE_RIGHT] << 1) |
						(keyboard[SDL_SCANCODE_UP]    << 2) |
						(keyboard[SDL_SCANCODE_DOWN]  << 3) |
						(keyboard[SDL_SCANCODE_X]     << 4) |
						(keyboard[SDL_SCANCODE_Z]     << 5) |
						(keyboard[SDL_SCANCODE_M]     << 6);
			key_new = key_down & (~key_last);
			key_new_or_repeat = key_new;

			if(key_new & VPET_KEY_RESET)
				init_game();

			if((key_down&(VPET_KEY_LEFT|VPET_KEY_RIGHT|VPET_KEY_UP|VPET_KEY_DOWN)) ==
			   (key_last&(VPET_KEY_LEFT|VPET_KEY_RIGHT|VPET_KEY_UP|VPET_KEY_DOWN)) ) {
				key_repeat++;
				if(key_repeat > 15) {
					key_repeat = 12;
					key_new_or_repeat |= key_down & (VPET_KEY_LEFT|VPET_KEY_RIGHT|VPET_KEY_UP|VPET_KEY_DOWN);
				}
			} else {
				key_repeat = 0;
			}

			// Update vpet
			void vpet_run();
			vpet_run();

			framecounter++;
		}
	}
	IMG_Quit();
	SDL_Quit();

	return 0;
}
#endif
