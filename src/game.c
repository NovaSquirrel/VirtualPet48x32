#include "vpet.h"

int PlayerX = 0;
int PlayerY = 3;
const Uint8 *keyboard;
int framecounter = 0;

enum KeyCode {
  KEY_LEFT  = 0x0001,
  KEY_RIGHT = 0x0002,
  KEY_UP    = 0x0004,
  KEY_DOWN  = 0x0008,
  KEY_A     = 0x0010,
  KEY_B     = 0x0020,
  KEY_RESET = 0x0040,
};

uint16_t KeyDown = 0, KeyNew = 0, KeyLast = 0;
int KeyRepeat = 0;

void init_game() {
	keyboard = SDL_GetKeyboardState(NULL);
	RandomSeed();
}

void run_game() {
	KeyLast = KeyDown;
	KeyDown =   (keyboard[SDL_SCANCODE_LEFT]  << 0) |
				(keyboard[SDL_SCANCODE_RIGHT] << 1) |
				(keyboard[SDL_SCANCODE_UP]    << 2) |
				(keyboard[SDL_SCANCODE_DOWN]  << 3) |
				(keyboard[SDL_SCANCODE_X]     << 4) |
				(keyboard[SDL_SCANCODE_Z]     << 5) |
				(keyboard[SDL_SCANCODE_C]     << 6);
	KeyNew = KeyDown & (~KeyLast);

	if(KeyNew & KEY_RESET)
		init_game();

	if((KeyDown&(KEY_LEFT|KEY_RIGHT|KEY_UP|KEY_DOWN)) ==
	   (KeyLast&(KEY_LEFT|KEY_RIGHT|KEY_UP|KEY_DOWN)) ) {
		KeyRepeat++;
		if(KeyRepeat > 15) {
			KeyRepeat = 12;
			KeyNew |= KeyDown & (KEY_LEFT|KEY_RIGHT|KEY_UP|KEY_DOWN);
		}
	} else {
		KeyRepeat = 0;
	}

	framecounter++;
}
