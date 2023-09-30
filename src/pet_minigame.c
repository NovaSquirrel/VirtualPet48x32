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

#define COORD_BOTTOM_OF_SCREEN ((PET_SCREEN_H-1) * COORD_SCALE)

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
	MINIGAME_BALLOON_THROW_4,
	MINIGAME_BALLOON_THROW_6,
	MINIGAME_KNUCKLES,
	MINIGAME_BOUNCE_UP, // climb by bouncing on things in the air
	MINIGAME_JUMP_UP,   // climb by jumping
	MINIGAME_FALL_DOWN,
	MINIGAME_FOOTBALL, // From Pix
	MINIGAME_TIMED_JUMP,
	MINIGAME_BOWLING,
	MINIGAME_OSUMESU, // Flag game
	MINIGAME_BUS_DRIVING, // From v4
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
int minigame_pet_x; // Multiplied by COORD_SCALE
int minigame_pet_y;
int minigame_pet_hflip;
int minigame_pet_vspeed;
int minigame_pet_on_ground;

#define BUFFER_KEYS_FRAMES 3
uint16_t minigame_buffer_keys[BUFFER_KEYS_FRAMES];

// .-----------------------------------------------------------------
// | Utilities
// '-----------------------------------------------------------------

int game_coordinate_to_screen(int coord) {
	return (coord + COORD_SCALE/2) / COORD_SCALE;
}

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
	if(key_down & VPET_KEY_LEFT_PREV) {
		minigame_pet_x -= COORD_SCALE;
		if(minigame_pet_x < bound_l*COORD_SCALE)
			minigame_pet_x = bound_l*COORD_SCALE;
		minigame_pet_hflip = 0;
	}
	if(key_down & VPET_KEY_RIGHT_NEXT) {
		minigame_pet_x += COORD_SCALE;
		if(minigame_pet_x > bound_r*COORD_SCALE)
			minigame_pet_x = bound_r*COORD_SCALE;
		minigame_pet_hflip = 1;
	}
}

void vpet_minigame_set_frame_for_walk() {
	minigame_pet_frame = CF_IDLE;
	if(key_down & VPET_KEY_LEFT_RIGHT_PREV_NEXT) {
		minigame_pet_frame = (enum character_frame)(CF_RUNNING + ((vpet_minigame_ticks/8) & 1));
	}
	if(!minigame_pet_on_ground)
		minigame_pet_frame = CF_JUMP;
}

int vpet_apply_gravity(int gravity) {
	minigame_pet_vspeed += gravity;
	minigame_pet_y += minigame_pet_vspeed;
	minigame_pet_on_ground = 0;
	if(minigame_pet_y > COORD_BOTTOM_OF_SCREEN) {
		minigame_pet_y = COORD_BOTTOM_OF_SCREEN;
		minigame_pet_vspeed = 0;
		minigame_pet_on_ground = 1;
	}

	return minigame_pet_on_ground;
}

int entity_touching_entity(int i, int j) {
	return (abs(entities[i].x - entities[j].x) * 2 < ((entities[i].collide_width + entities[j].collide_width)*COORD_SCALE) ) &&
	       (abs(entities[i].y - entities[j].y) * 2 < ((entities[i].collide_height + entities[j].collide_height)*COORD_SCALE) );
}

int entity_touching_pet(int i) {
	return (abs(entities[i].x - minigame_pet_x) * 2 < ((entities[i].collide_width + 12)*COORD_SCALE) ) &&
	       (abs(entities[i].y - minigame_pet_y) * 2 < ((entities[i].collide_height + 14)*COORD_SCALE) );
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
		int entity_draw_x = game_coordinate_to_screen(entities[i].x);
		int entity_draw_y = game_coordinate_to_screen(entities[i].y);

		switch(entities[i].draw_flags & ENTITY_DRAW_SIZE) {
			case ENTITY_DRAW_8:
				vpet_sprite_8(entity_draw_x - (no_centering?0:(8/2)), entity_draw_y - entities[i].art_height + 1, entities[i].hflip, entities[i].art_height, entities[i].frame.art_8, operation_pointer);
				break;
			case ENTITY_DRAW_16:
				vpet_sprite_16(entity_draw_x - (no_centering?0:(16/2)), entity_draw_y - entities[i].art_height + 1, entities[i].hflip, entities[i].art_height, entities[i].frame.art_16, operation_pointer);
				break;
			case ENTITY_DRAW_24:
				vpet_sprite_24(entity_draw_x - (no_centering?0:(24/2)), entity_draw_y - entities[i].art_height + 1, entities[i].hflip, entities[i].art_height, entities[i].frame.art_24, operation_pointer);
				break;
			case ENTITY_DRAW_CUSTOM:
				entities[i].frame.custom(i);
				break;
		}
	}
}

void vpet_minigame_draw_pet() {
	vpet_draw_entities(0);

//	int minigame_pet_draw_height = game_coordinate_to_screen(minigame_pet_height);
	int minigame_pet_draw_x = game_coordinate_to_screen(minigame_pet_x);
	int minigame_pet_draw_y = game_coordinate_to_screen(minigame_pet_y);

	vpet_draw_pet(minigame_pet_draw_x-16/2, minigame_pet_draw_y-15, minigame_pet_hflip,  my_pet.profile.species, minigame_pet_frame);
//	if(minigame_pet_draw_height) {
//		vpet_hline_or(minigame_pet_draw_x-4, minigame_pet_draw_y, 8);
//	}
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
	minigame_pet_vspeed = 0;

	switch(which_minigame) {
		case MINIGAME_HEADING:
			minigame_pet_x = PET_SCREEN_CENTER_X * COORD_SCALE;
			minigame_pet_y = COORD_BOTTOM_OF_SCREEN;

			for(int i=0; i<5; i++) {
				entities[i].type = 1;
				entities[i].x = (i+1) * 8 * COORD_SCALE;
				entities[i].y = -2 * COORD_SCALE;
				entities[i].art_height = 8;
				entities[i].draw_flags = ENTITY_DRAW_8 | ENTITY_DRAW_BLACK;
				entities[i].frame.art_8 = heading_ball_art;
				entities[i].timer = Random(300);
			}
			break;
		default:
			break;
	}
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
			for(int i=0; i<ENTITY_LEN; i++) {
				if(entities[i].type) {
					if(entities[i].timer) {
						entities[i].timer--;
					} else {
						entities[i].vspeed += 1;
						entities[i].y += entities[i].vspeed;
					}
					if(entities[i].y > COORD_BOTTOM_OF_SCREEN) {
						//entities[i].y = COORD_BOTTOM_OF_SCREEN;
						//entities[i].vspeed = 0;
						vpet_switch_state(STATE_DEFAULT);
						return;
					}
					if(entity_touching_pet(i)) {
						entities[i].vspeed = -(128);
						entities[i].y -= 2 * COORD_SCALE;
					}
					if(entities[i].y < (-2*COORD_SCALE) && entities[i].vspeed < 0) {
						entities[i].timer = 60+Random(180);
						entities[i].vspeed = 0;
					}
					if(entities[i].y > 0)
						vpet_hline_or(game_coordinate_to_screen(entities[i].x)-4, PET_SCREEN_H-1, 8);
				}
			}

			//if(!minigame_pet_draw_height)
			vpet_minigame_walk_lr(8, PET_SCREEN_W-8);

			if(vpet_apply_gravity(3*16)) {
				if(key_down & VPET_KEY_A)
					minigame_pet_vspeed = -(2*256 + 128);
					//minigame_pet_vspeed = 3*256; //2*256 + 128;
			}

			vpet_minigame_set_frame_for_walk();
			vpet_minigame_draw_pet();
			if(!minigame_pet_on_ground)
				vpet_hline_or(game_coordinate_to_screen(minigame_pet_x)-4, PET_SCREEN_H-1, 8);
			break;
		default:
			break;
	}

	vpet_minigame_ticks++;
}
