#include "vpet.h"

uint8_t vpet_screen_buffer[PET_SCREEN_W / 8][PET_SCREEN_H];

void vpet_clear_screen() {
	memset(vpet_screen_buffer, 0, sizeof(vpet_screen_buffer));
}

void vpet_set_pixel(int x, int y, int value) {
	if(value) {
		vpet_screen_buffer[x/8][y] |= 0x80 >> (x & 7);
	} else {
		vpet_screen_buffer[x/8][y] &= ~(0x80 >> (x & 7));
	}
}

int vpet_get_pixel(int x, int y) {
	return vpet_screen_buffer[x/8][y] & 0x80 >> (x & 7);
}

void vpet_render_screen() {
	for(int y=0; y<PET_SCREEN_H; y++) {
		for(int x=0; x<PET_SCREEN_W/8; x++) {
			uint8_t pixels = vpet_screen_buffer[x][y];
			for(int i=7; i>=0; i--) {
				if(pixels & (1<<i)) {
					SDL_SetRenderDrawColor(ScreenRenderer, 0, 0, 0, 255);
					SDL_Rect Temp = {((x*8+(7-i))*ScreenZoom)+1, y*ScreenZoom+1, ScreenZoom-1, ScreenZoom-1};
					SDL_RenderFillRect(ScreenRenderer, &Temp);
				} else {
//					continue;
					SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_MOD);

					SDL_SetRenderDrawColor(ScreenRenderer, 0xf0, 0xf0, 0xf0, 255); // <-- with background
//					SDL_SetRenderDrawColor(ScreenRenderer, 0xc0, 0xc0, 0xc0, 255); // <-- no background
					SDL_Rect Temp = {((x*8+(7-i))*ScreenZoom)+1, y*ScreenZoom+1, ScreenZoom-1, ScreenZoom-1};
					SDL_RenderFillRect(ScreenRenderer, &Temp);

					SDL_SetRenderDrawBlendMode(ScreenRenderer, SDL_BLENDMODE_NONE);
				}
				//rectfill(ScreenRenderer, x*8+(7-i), y, 1, 1);
			}
		}
	}
}

// ----------------------------------------------

void vpet_or_8_pixels(int x, int y, uint8_t value) {
	if(x < 0 || x >= PET_SCREEN_W/8 || y < 0 || y >= PET_SCREEN_H)
		return;
	vpet_screen_buffer[x][y] |= value;
}

void vpet_xor_8_pixels(int x, int y, uint8_t value) {
	if(x < 0 || x >= PET_SCREEN_W/8 || y < 0 || y >= PET_SCREEN_H)
		return;
	vpet_screen_buffer[x][y] ^= value;
}

void vpet_and_8_pixels(int x, int y, uint8_t value) {
	if(x < 0 || x >= PET_SCREEN_W/8 || y < 0 || y >= PET_SCREEN_H)
		return;
	vpet_screen_buffer[x][y] &= value;
}

// ----------------------------------------------

// https://stackoverflow.com/a/2602885
unsigned char reverse8(unsigned char b) {
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
}

uint16_t reverse16(uint16_t w) {
	return (reverse8(w & 255) << 8) | reverse8((w >> 8) & 255);
}

uint32_t reverse24(uint32_t w) {
	return (reverse8(w & 255) << 16) | (reverse8((w >> 8) & 255) << 8) | reverse8((w >> 16) & 255);
}

// ----------------------------------------------

void vpet_sprite_8(int x, int y, int hflip, int h, const uint8_t rows[], void (*operation)(int, int, uint8_t)) {
	int shift = (8-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse8(pixels);
		operation(x,   y, (pixels >> 8) & 0xff);
		operation(x+1, y, (pixels     ) & 0xff);
		y++;
	}
}

void vpet_sprite_16(int x, int y, int hflip, int h, const uint16_t rows[], void (*operation)(int, int, uint8_t)) {
	int shift = (8-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse16(pixels);
		operation(x,   y, (pixels >> 16) & 0xff);
		operation(x+1, y, (pixels >> 8 ) & 0xff);
		operation(x+2, y, (pixels      ) & 0xff);
		y++;
	}
}

void vpet_sprite_24(int x, int y, int h, int hflip, const uint32_t rows[], void (*operation)(int, int, uint8_t)) {
	int shift = (8-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse24(pixels);
		operation(x,   y, (pixels >> 24) & 0xff);
		operation(x+1, y, (pixels >> 16) & 0xff);
		operation(x+2, y, (pixels >> 8 ) & 0xff);
		operation(x+3, y, (pixels      ) & 0xff);
		y++;
	}
}

// ----------------------------------------------

void vpet_sprite_overwrite_8(int x, int y, int hflip, int h, const uint8_t rows[]) {
	int shift = (8-(x & 7));
	unsigned int mask_pixels = ~(255 << shift);
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse8(pixels);
		vpet_and_8_pixels(x,   y, (mask_pixels >> 8)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 8     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels     )&0xff);
		vpet_or_8_pixels( x+1, y, (pixels          )&0xff);
		y++;
	}
}

void vpet_sprite_overwrite_16(int x, int y, int hflip, int h, const uint16_t rows[]) {
	int shift = (16-(x & 7));
	unsigned int mask_pixels = ~(255 << shift);
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse16(pixels);
		vpet_and_8_pixels(x,   y, (mask_pixels >> 16)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 16     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels >> 8 )&0xff);
		vpet_or_8_pixels( x+1, y, (pixels >> 8      )&0xff);
		vpet_and_8_pixels(x+2, y, (mask_pixels      )&0xff);
		vpet_or_8_pixels( x+2, y, (pixels           )&0xff);
		y++;
	}
}

void vpet_sprite_overwrite_24(int x, int y, int hflip, int h, const uint32_t rows[]) {
	int shift = (24-(x & 7));
	unsigned int mask_pixels = ~(255 << shift);
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		if(hflip)
			pixels = reverse24(pixels);
		vpet_and_8_pixels(x,   y, (mask_pixels >> 16)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 16     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels >> 8 )&0xff);
		vpet_or_8_pixels( x+1, y, (pixels >> 8      )&0xff);
		vpet_and_8_pixels(x+2, y, (mask_pixels      )&0xff);
		vpet_or_8_pixels( x+2, y, (pixels           )&0xff);
		y++;
	}
}

// ----------------------------------------------

void vpet_sprite_mask_8(int x, int y, int hflip, int h, const uint8_t rows[], const uint8_t mask_rows[]) {
	int shift = (8-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		unsigned int mask_pixels = mask_rows[r] << shift;
		if(hflip) {
			pixels = reverse8(pixels);
			mask_pixels = reverse8(pixels);
		}
		vpet_and_8_pixels(x,   y, (mask_pixels >> 8)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 8     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels     )&0xff);
		vpet_or_8_pixels( x+1, y, (pixels          )&0xff);
		y++;
	}
}

void vpet_sprite_mask_16(int x, int y, int hflip, int h, const uint16_t rows[], const uint16_t mask_rows[]) {
	int shift = (16-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		unsigned int mask_pixels = mask_rows[r] << shift;
		if(hflip) {
			pixels = reverse16(pixels);
			mask_pixels = reverse16(pixels);
		}
		vpet_and_8_pixels(x,   y, (mask_pixels >> 16)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 16     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels >> 8 )&0xff);
		vpet_or_8_pixels( x+1, y, (pixels >> 8      )&0xff);
		vpet_and_8_pixels(x+2, y, (mask_pixels      )&0xff);
		vpet_or_8_pixels( x+2, y, (pixels           )&0xff);
		y++;
	}
}

void vpet_sprite_mask_24(int x, int y, int hflip, int h, const uint32_t rows[], const uint32_t mask_rows[]) {
	int shift = (24-(x & 7));
	x /= 8;
	for(size_t r=0; r<h; r++) {
		unsigned int pixels = rows[r] << shift;
		unsigned int mask_pixels = mask_rows[r] << shift;
		if(hflip) {
			pixels = reverse24(pixels);
			mask_pixels = reverse24(pixels);
		}
		vpet_and_8_pixels(x,   y, (mask_pixels >> 24)&0xff);
		vpet_or_8_pixels( x,   y, (pixels >> 24     )&0xff);
		vpet_and_8_pixels(x+1, y, (mask_pixels >> 16)&0xff);
		vpet_or_8_pixels( x+1, y, (pixels >> 16     )&0xff);
		vpet_and_8_pixels(x+2, y, (mask_pixels >> 8 )&0xff);
		vpet_or_8_pixels( x+2, y, (pixels >> 8      )&0xff);
		vpet_and_8_pixels(x+3, y, (mask_pixels      )&0xff);
		vpet_or_8_pixels( x+3, y, (pixels           )&0xff);
		y++;
	}
}

// ----------------------------------------------

const uint8_t *vpet_current_font = font_4x6;
int vpet_current_font_width = 4;
int vpet_current_font_height = 6;

void vpet_set_font(const uint8_t font[], int width, int height) {
	vpet_current_font = font;
	vpet_current_font_width = width;
	vpet_current_font_height = height;
}

void vpet_draw_text(int x, int y, const char *buffer) {
	for(const char *text = buffer; *text; text++) {
		char c = *text;
		vpet_sprite_xor_8(x, y, 0, 6, font_4x6 + (c-' ')*vpet_current_font_height);
		x += vpet_current_font_width;
	}
}

void vpet_draw_textf(int x, int y, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	char buffer[512];
	vsprintf(buffer, fmt, argp);
	vpet_draw_text(x, y, buffer);
	va_end(argp);
}

// ----------------------------------------------

extern const uint16_t mimi_art[];
extern const uint16_t pyonko_art[];
extern const uint16_t terrier_art[];

const uint16_t *character_spritesheets[] = {
	mimi_art, pyonko_art, terrier_art
};

void vpet_draw_pet_crop(int x, int y, int hflip, enum character_id character, enum character_frame frame, int rows) {
	vpet_sprite_xor_16(x, y, hflip, rows, character_spritesheets[character] + frame*16);
}

void vpet_draw_pet(int x, int y, int hflip, enum character_id character, enum character_frame frame) {
	vpet_draw_pet_crop(x, y, hflip, character, frame, 16);
} 
