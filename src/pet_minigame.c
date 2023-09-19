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

// .-----------------------------------------------------------------
// | Import
// '-----------------------------------------------------------------
extern const uint8_t heading_ball_art[];

// .-----------------------------------------------------------------
// | Enums
// '-----------------------------------------------------------------

enum minigame_id {
	MINIGAME_AIR_GRIND,
	MINIGAME_BOMB_RALLY,
	MINIGAME_PIPES,
	MINIGAME_HEADING,
	MINIGAME_MAZE, // Like Entombed
	MINIGAME_REVERSI,
	MINIGAME_GET_NOTES,
	MINIGAME_SHOOT,
	MINIGAME_BALLOON_THROW,
	MINIGAME_KNUCKLES,
	MINIGAME_BOUNCE_UP, // climb by bouncing on things in the air
	MINIGAME_JUMP_UP,   // climb by jumping
	MINIGAME_FALL_DOWN,
};
enum minigame_id which_minigame = MINIGAME_HEADING;

// .-----------------------------------------------------------------
// | Variables
// '-----------------------------------------------------------------

unsigned int vpet_minigame_ticks;

// Generic state
struct entity entities[ENTITY_LEN];

union {
	struct {
		int a;
	} air_grind;

	struct {
	} heading;
} minigame_vars;

// Reusable animation variables
enum character_frame minigame_pet_frame = CF_IDLE;
int minigame_pet_draw_x;
int minigame_pet_draw_y;
int minigame_pet_hflip;
int minigame_pet_draw_height;

int minigame_pet_real_height;
int minigame_pet_real_vspeed;

#define BUFFER_KEYS_FRAMES 3
uint16_t minigame_buffer_keys[BUFFER_KEYS_FRAMES];

// .-----------------------------------------------------------------
// | Utilities
// '-----------------------------------------------------------------

int get_buffered_key_new(uint16_t key) {
	if(minigame_buffer_keys[0] & key) {
		for(int i=0; i<BUFFER_KEYS_FRAMES; i++) {
			minigame_buffer_keys[i] &= ~key;
		}
		return 1;
	}
	return 0;
}

void vpet_minigame_walk_lr(int bound_l, int bound_r) {
	if(key_down & KEY_LEFT_PREV) {
		minigame_pet_draw_x--;
		if(minigame_pet_draw_x < bound_l)
			minigame_pet_draw_x = bound_l;
		minigame_pet_hflip = 0;
	}
	if(key_down & KEY_RIGHT_NEXT) {
		minigame_pet_draw_x++;
		if(minigame_pet_draw_x > bound_r)
			minigame_pet_draw_x = bound_r;
		minigame_pet_hflip = 1;
	}
}

void vpet_minigame_set_frame_for_walk() {
	minigame_pet_frame = CF_IDLE;
	if(key_down & (KEY_LEFT | KEY_RIGHT)) {
		minigame_pet_frame = CF_RUNNING + ((vpet_minigame_ticks/8) & 1);
	}
	if(minigame_pet_draw_height)
		minigame_pet_frame = CF_JUMP;
}

int vpet_apply_gravity(int gravity) {
	minigame_pet_real_vspeed -= gravity;
	minigame_pet_real_height += minigame_pet_real_vspeed;
	int on_ground = 0;
	if(minigame_pet_real_height < 0) {
		minigame_pet_real_height = 0;
		minigame_pet_real_vspeed = 0;
		on_ground = 1;
	}

	minigame_pet_draw_height = (minigame_pet_real_height + 128) / 256;
	return on_ground;
}

void vpet_draw_entities(int over) {
	static void (*const operations[3])(int, int, uint8_t) = {vpet_xor_8_pixels, vpet_or_8_pixels, vpet_andnot_8_pixels};

	for(int i=0; i<ENTITY_LEN; i++) {
		if(!entities[i].type)
			continue;
		if( ((entities[i].draw_flags & ENTITY_DRAW_ABOVE) != 0) != over)
			continue;

		int no_centering = entities[i].draw_flags & ENTITY_NO_CENTERING;
		void (*operation_pointer)(int, int, uint8_t) = operations[(entities[i].draw_flags & ENTITY_DRAW_COLOR) >> 2];

		switch(entities[i].draw_flags & ENTITY_DRAW_SIZE) {
			case ENTITY_DRAW_8:
				vpet_sprite_8(entities[i].draw_x - (no_centering?0:(8/2)), entities[i].draw_y - entities[i].draw_height + 1, entities[i].hflip, entities[i].draw_height, entities[i].frame.art_8, operation_pointer);
				break;
			case ENTITY_DRAW_16:
				vpet_sprite_16(entities[i].draw_x - (no_centering?0:(16/2)), entities[i].draw_y - entities[i].draw_height + 1, entities[i].hflip, entities[i].draw_height, entities[i].frame.art_16, operation_pointer);
				break;
			case ENTITY_DRAW_24:
				vpet_sprite_24(entities[i].draw_x - (no_centering?0:(24/2)), entities[i].draw_y - entities[i].draw_height + 1, entities[i].hflip, entities[i].draw_height, entities[i].frame.art_24, operation_pointer);
				break;
			case ENTITY_DRAW_CUSTOM:
				entities[i].frame.custom(i);
				break;
		}
	}
}

void vpet_minigame_draw_pet() {
	vpet_draw_entities(0);
	vpet_draw_pet(minigame_pet_draw_x-16/2, minigame_pet_draw_y-15 - minigame_pet_draw_height, minigame_pet_hflip,  my_pet.profile.species, minigame_pet_frame);
	if(minigame_pet_draw_height) {
		vpet_hline_or(minigame_pet_draw_x-4, minigame_pet_draw_y, 8);
	}
	vpet_draw_entities(1);
}

// .-----------------------------------------------------------------
// | Init
// '-----------------------------------------------------------------

void vpet_init_minigame() {
	vpet_minigame_ticks = 0;
	memset(&minigame_vars, 0, sizeof(minigame_vars));
	memset(entities, 0, sizeof(entities));

	minigame_pet_hflip = 0;
	minigame_pet_draw_height = 0;
	minigame_pet_real_height = 0;
	minigame_pet_real_vspeed = 0;

	switch(which_minigame) {
		case MINIGAME_HEADING:
			minigame_pet_draw_x = PET_SCREEN_CENTER_X;
			minigame_pet_draw_y = PET_SCREEN_H-1;
			break;
		default:
			break;
	}

	entities[0].type = 1;
	entities[0].draw_x = 16;
	entities[0].draw_y = 8;
	entities[0].draw_height = 8;
	entities[0].draw_flags = ENTITY_DRAW_8;
	entities[0].frame.art_8 = heading_ball_art;
}

// .-----------------------------------------------------------------
// | Game logic
// '-----------------------------------------------------------------

void vpet_tick60fps_minigame() {
	// Update minigame key buffering
	for(int i=0; i<BUFFER_KEYS_FRAMES-1; i++) {
		minigame_buffer_keys[i] = minigame_buffer_keys[i+1];
	}
	for(int i=0; i<6; i++) {
		if(key_new & (1<<i)) {
			for(int j=0; j<BUFFER_KEYS_FRAMES; j++)
				minigame_buffer_keys[j] |= 1<<i;
		}
	}

	vpet_clear_screen();

	// Do each minigame's logic
	switch(which_minigame) {
		case MINIGAME_HEADING:
			//if(!minigame_pet_draw_height)
			vpet_minigame_walk_lr(8, PET_SCREEN_W-8);

			if(vpet_apply_gravity(3*16)) {
				if(key_down & KEY_A)
					minigame_pet_real_vspeed = 2*256 + 128;
					//minigame_pet_real_vspeed = 3*256; //2*256 + 128;
			}

			vpet_minigame_set_frame_for_walk();
			vpet_minigame_draw_pet();
			break;
		default:
			break;
	}

	vpet_minigame_ticks++;
}

