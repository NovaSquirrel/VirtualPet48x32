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
#include "minigame_sprites.h"
#define COORD_BOTTOM_OF_SCREEN ((PET_SCREEN_H-1) * COORD_SCALE)
#define PLAYFIELD_W 16
#define PLAYFIELD_H 16

// .-----------------------------------------------------------------
// | Import
// '-----------------------------------------------------------------
extern const uint8_t heading_ball_art[];
extern const uint8_t minigame_8x8_art[];
extern const uint8_t minigame_6x6_art[];

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
	MINIGAME_FROG_FLAP, // Like from WarioWare
	MINIGAME_CLIMB_TOWER,
};
enum minigame_id which_minigame = MINIGAME_CLIMB_TOWER;

// .-----------------------------------------------------------------
// | Variables
// '-----------------------------------------------------------------

unsigned int vpet_minigame_ticks;

// Generic state
struct entity entities[ENTITY_LEN];
uint8_t minigame_playfield[PLAYFIELD_W][PLAYFIELD_H];
int minigame_scroll_x, minigame_scroll_y;
int minigame_coord_scroll_x, minigame_coord_scroll_y;

union {
	struct {
		int a;
	} air_grind;

	struct {
		int distance;
		int distance_until_platform;
	} frogflap;

	struct {
	} heading;

	struct {
	} climb_tower;
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

int entity_touching_player_8x8(int i) {
	return (abs(entities[i].x - minigame_pet_x) * 2 < ((entities[i].collide_width + 8)*COORD_SCALE) ) &&
	       (abs(entities[i].y - minigame_pet_y) * 2 < ((entities[i].collide_height + 8)*COORD_SCALE) );
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
		int entity_draw_x = game_coordinate_to_screen(entities[i].x) - minigame_scroll_x;
		int entity_draw_y = game_coordinate_to_screen(entities[i].y) - minigame_scroll_y;

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

void vpet_minigame_draw_8x8_player(enum minigame_8x8_art frame) {
	int minigame_pet_draw_x = game_coordinate_to_screen(minigame_pet_x) - minigame_scroll_x;
	int minigame_pet_draw_y = game_coordinate_to_screen(minigame_pet_y) - minigame_scroll_y;
	if(minigame_pet_draw_y < 0) {
		minigame_pet_draw_y = 7;
		frame = MG8_ARROW_U;
	}
	vpet_sprite_8(minigame_pet_draw_x - 8/2, minigame_pet_draw_y - 7, 0, 8, &minigame_8x8_art[frame*8], vpet_or_8_pixels);
}

void vpet_minigame_draw_pet() {
	vpet_draw_entities(0);

	int minigame_pet_draw_x = game_coordinate_to_screen(minigame_pet_x) - minigame_scroll_x;
	int minigame_pet_draw_y = game_coordinate_to_screen(minigame_pet_y) - minigame_scroll_y;
	vpet_draw_pet(minigame_pet_draw_x-16/2, minigame_pet_draw_y-15, minigame_pet_hflip,  my_pet.profile.species, minigame_pet_frame);

	vpet_draw_entities(1);
}

int minigame_find_free_entity() {
	for(int slot=0; slot<ENTITY_LEN; slot++)
		if(entities[slot].type == 0) {
			memset(&entities[slot], 0, sizeof(entities[slot]));
			return slot;
		}
	return -1;
}

int minigame_create_entity(int type, int x, int y, int var1, int var2, int var3, int var4) {
	int slot = minigame_find_free_entity();
	if(slot == -1)
		return -1;
	entities[slot].type = type;
	entities[slot].x = x;
	entities[slot].y = y;
	entities[slot].var[0] = var1;
	entities[slot].var[1] = var2;
	entities[slot].var[2] = var3;
	entities[slot].var[3] = var4;
	return slot;
}

int minigame_create_entity_8x8(int type, int x, int y, int var1, int var2, int var3, int var4, enum minigame_8x8_art frame, int collide_height) {
	int slot = minigame_create_entity(type, x, y, var1, var2, var3, var4);
	if(slot == -1)
		return -1;
	entities[slot].frame.art_8    = &minigame_8x8_art[frame*8];
	entities[slot].draw_flags     = ENTITY_DRAW_8 | ENTITY_DRAW_BLACK;
	entities[slot].art_height     = 8;
	entities[slot].collide_width  = 8;
	entities[slot].collide_height = collide_height;
	return slot;
}

void minigame_draw_playfield(int tile_width, int tile_height, const uint8_t tiles_art[]) {
	int screen_tiles_right = PET_SCREEN_W / tile_width;
	int screen_tiles_down  = PET_SCREEN_H / tile_height;

	int camera_offset_x = minigame_scroll_x % tile_width;
	int camera_offset_y = minigame_scroll_y % tile_height;
	int camera_tile_x   = minigame_scroll_x / tile_width;
	int camera_tile_y   = minigame_scroll_y / tile_height;

	for(int y=0; y<=screen_tiles_down; y++) {
		for(int x=0; x<=screen_tiles_right; x++) {
			int real_x = camera_tile_x + x;
			int real_y = camera_tile_y + y;
			if(real_x < 0 || real_x >= PLAYFIELD_W || real_y < 0 || real_y >= PLAYFIELD_H)
				continue;
			vpet_sprite_8(x * tile_width - camera_offset_x, y * tile_height - camera_offset_y, 0, tile_height, &tiles_art[minigame_playfield[real_x][real_y]*tile_height], vpet_or_8_pixels);
		}
	}
}

void camera_slide_to(int target_x, int target_y) {
	int difference_x = target_x - minigame_coord_scroll_x;
	int difference_y = target_y - minigame_coord_scroll_y;
	minigame_coord_scroll_x += difference_x / 4;
	minigame_coord_scroll_y += difference_y / 4;
}

int mg6_solid_on_top(enum minigame_6x6_art tile) {
	return tile != 0;
}
int mg6_solid_on_bottom(enum minigame_6x6_art tile) {
	return tile != 0;
}
int mg6_solid_on_left(enum minigame_6x6_art tile) {
	return tile != 0;
}
int mg6_solid_on_right(enum minigame_6x6_art tile) {
	return tile != 0;
}

int mg6_playfield_get(int x, int y) {
	x = game_coordinate_to_screen(x) / 6;
	y = game_coordinate_to_screen(y) / 6;
	if(x < 0 || x >= PLAYFIELD_W || y < 0 || y >= PLAYFIELD_H)
		return 0;
	return minigame_playfield[x][y];
}

// .-----------------------------------------------------------------
// | Init
// '-----------------------------------------------------------------

void vpet_init_minigame() {
	vpet_minigame_ticks = 0;
	memset(&minigame_vars, 0, sizeof(minigame_vars));
	memset(entities, 0, sizeof(entities));
	memset(minigame_playfield, 0, sizeof(minigame_playfield));
	minigame_scroll_x = 0;
	minigame_scroll_y = 0;
	minigame_coord_scroll_x = 0;
	minigame_coord_scroll_y = 0;

	minigame_playfield[0][0] = MG6_CHECKER_BLOCK;
	minigame_playfield[1][0] = MG6_CHECKER_BLOCK;
	minigame_playfield[1][1] = MG6_CHECKER_BLOCK;
	minigame_playfield[2][1] = MG6_CHECKER_BLOCK;
	minigame_playfield[2][3] = MG6_CHECKER_BLOCK;

	minigame_playfield[0][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[1][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[2][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[3][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[4][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[5][15] = MG6_CHECKER_BLOCK;
	minigame_playfield[4][14] = MG6_CHECKER_BLOCK;
	minigame_playfield[5][14] = MG6_CHECKER_BLOCK;
	minigame_playfield[5][13] = MG6_CHECKER_BLOCK;

	// Set defaults
	minigame_pet_hflip = 0;
	minigame_pet_vspeed = 0;
	for(int i=0; i<ENTITY_LEN; i++) {
		entities[i].art_height = 8;
		entities[i].draw_flags = ENTITY_DRAW_8 | ENTITY_DRAW_BLACK;
		entities[i].collide_width  = 8;
		entities[i].collide_height = 8;
	}

	switch(which_minigame) {

		// --------------------------------------------------------------------
		case MINIGAME_HEADING:
			minigame_pet_x = PET_SCREEN_CENTER_X * COORD_SCALE;
			minigame_pet_y = COORD_BOTTOM_OF_SCREEN;

			for(int i=0; i<5; i++) {
				entities[i].type = 1;
				entities[i].x = (i+1) * 8 * COORD_SCALE;
				entities[i].y = -2 * COORD_SCALE;
				entities[i].frame.art_8 = heading_ball_art;
				entities[i].timer = Random(300);
			}
			break;

		// --------------------------------------------------------------------
		case MINIGAME_FROG_FLAP:
			minigame_pet_x = 8 * COORD_SCALE;
			minigame_pet_y = 0;

			minigame_create_entity_8x8(1, 8 * COORD_SCALE,                        COORD_BOTTOM_OF_SCREEN-8*COORD_SCALE, 0, 0, 0, 0, MG8_CLOUD_PLATFORM, 4);
			minigame_create_entity_8x8(1, PET_SCREEN_CENTER_X * COORD_SCALE,      COORD_BOTTOM_OF_SCREEN-8*COORD_SCALE, 0, 0, 0, 0, MG8_CLOUD_PLATFORM, 4);
			minigame_create_entity_8x8(1, (PET_SCREEN_CENTER_X+16) * COORD_SCALE, COORD_BOTTOM_OF_SCREEN-8*COORD_SCALE, 0, 0, 0, 0, MG8_CLOUD_PLATFORM, 4);
			break;

		// --------------------------------------------------------------------
		case MINIGAME_CLIMB_TOWER:
			minigame_pet_x = PET_SCREEN_CENTER_X * COORD_SCALE;
			minigame_pet_y = COORD_BOTTOM_OF_SCREEN;
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
		// --------------------------------------------------------------------
		case MINIGAME_FROG_FLAP:
			vpet_minigame_walk_lr(4, PET_SCREEN_W-4);
			int scroll_left_by = 0;
			if(minigame_pet_x > PET_SCREEN_CENTER_X * COORD_SCALE) {
				scroll_left_by = minigame_pet_x - PET_SCREEN_CENTER_X * COORD_SCALE;
				minigame_pet_x = PET_SCREEN_CENTER_X * COORD_SCALE;
				minigame_vars.frogflap.distance += scroll_left_by;
				minigame_vars.frogflap.distance_until_platform -= scroll_left_by;
				if(minigame_vars.frogflap.distance_until_platform < 0) {
					minigame_vars.frogflap.distance_until_platform = RandomMinMax(10 * COORD_SCALE, 40 * COORD_SCALE);
					minigame_create_entity_8x8(1, PET_SCREEN_W * COORD_SCALE, RandomMinMax(COORD_BOTTOM_OF_SCREEN-20*COORD_SCALE, COORD_BOTTOM_OF_SCREEN), 0, 0, 0, 0, MG8_CLOUD_PLATFORM, 4);
				}
			}

			for(int i=0; i<ENTITY_LEN; i++) {
				if(entities[i].type) {
					entities[i].x -= scroll_left_by;
					if(entities[i].x < (-8 * COORD_SCALE))
						entities[i].type = 0;
					if(entity_touching_player_8x8(i) && entities[i].y > minigame_pet_y && minigame_pet_vspeed > 0) {
						entities[i].type = 0;
						minigame_pet_vspeed = -(1*256 + 64);
					}
				}
			}

			if(vpet_apply_gravity(8)) {
				vpet_switch_state(STATE_DEFAULT);
				return;
			}

			vpet_draw_entities(0);
			vpet_minigame_draw_8x8_player(MG8_SMILE2);
			break;

		// --------------------------------------------------------------------
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

		// --------------------------------------------------------------------
		case MINIGAME_CLIMB_TOWER:
			vpet_minigame_walk_lr(8, PET_SCREEN_W-8);

			// Gravity
			minigame_pet_vspeed += 1*16;
			minigame_pet_y += minigame_pet_vspeed;
			minigame_pet_on_ground = 0;

			// Standing on stuff?
			if(mg6_solid_on_top(mg6_playfield_get(minigame_pet_x - 2*COORD_SCALE, minigame_pet_y + COORD_SCALE)) || mg6_solid_on_top(mg6_playfield_get(minigame_pet_x - 1*COORD_SCALE, minigame_pet_y + COORD_SCALE))) {
				minigame_pet_vspeed = 0;
				minigame_pet_y += COORD_SCALE;
				minigame_pet_y -= minigame_pet_y % (6 * COORD_SCALE);
				minigame_pet_y -= COORD_SCALE;
				minigame_pet_on_ground = 1;
			}

			if(minigame_pet_on_ground && (key_down & VPET_KEY_A)) {
				minigame_pet_vspeed = -(256 + 64);
			}

			vpet_minigame_set_frame_for_walk();

			camera_slide_to(minigame_pet_x - (PET_SCREEN_W/2 * COORD_SCALE), minigame_pet_y - (PET_SCREEN_H/2 * COORD_SCALE));
			minigame_scroll_x = game_coordinate_to_screen(minigame_coord_scroll_x);
			minigame_scroll_y = game_coordinate_to_screen(minigame_coord_scroll_y);

			minigame_draw_playfield(6, 6, minigame_6x6_art);
			vpet_minigame_draw_pet();
			break;

		default:
			break;
	}

	vpet_minigame_ticks++;
}
