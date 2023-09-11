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

#define HAPPY_JUMP_HEIGHT 5

enum idle_animation {
	IDLE_ANIM_WANDER
};

enum idle_animation current_idle_animation;
unsigned int pet_animation_timer;
int pet_animation_x, pet_animation_y;
int pet_animation_target_x, pet_animation_target_y;
enum character_frame pet_animation_frame;
int pet_animation_hflip;
enum food_id pet_food_to_eat;

int intclamp(int a, int min, int max) {
	if(a < min)
		return min;
	if(a > max)
		return max;
	return a;
}

void init_animation_for_state(enum game_state new_state) {
	pet_animation_timer = 0;
	pet_animation_frame = CF_IDLE;
	pet_animation_hflip = 0;
	pet_animation_x = PET_SCREEN_CENTER_X;
	pet_animation_y = PET_SCREEN_CENTER_Y;

	switch(new_state) {
		case STATE_DEFAULT:;
			current_idle_animation = IDLE_ANIM_WANDER;
			pet_animation_target_x = PET_SCREEN_CENTER_X;
			pet_animation_target_y = PET_SCREEN_CENTER_Y;
			break;
		case STATE_EATING:
			pet_animation_frame = CF_EATING;
			pet_animation_x = PET_SCREEN_CENTER_X + 8;
			break;
		default:
			break;
	}
}

void vpet_draw_pet_animation() {
	// Assume screen was cleared
	vpet_draw_pet(pet_animation_x-16/2, pet_animation_y-16/2, pet_animation_hflip,  my_pet.profile.species, pet_animation_frame);
	if(pet_animation_frame == CF_JUMP)
		vpet_hline(pet_animation_x-4, pet_animation_y+16/2+HAPPY_JUMP_HEIGHT, 8);

	switch(vpet_state) {
		case STATE_EATING:
			if(pet_animation_timer < 6)
				vpet_draw_food(PET_SCREEN_CENTER_X - 16, pet_animation_timer ? PET_SCREEN_CENTER_Y-16/2 : 0, pet_food_to_eat, (pet_animation_timer / 2) % 3);
			break;
		default:
			break;
	}
}

struct per_character_animation_params {
	int flip_when_moving;
	int flip_chance;
	int second_frame_chance;
	int jump_chance;
};

const struct per_character_animation_params character_anim_params[] = {
	{0, 2, 4, 31}, // Mimi
	{1, 4, 4, 19}, // Pyonko
	{1, 4, 4, 31}, // Terrier
};

// Runs every half second (if on an LCD) to update the animations.
void vpet_tick_animation() {
	const struct per_character_animation_params *animation_params = &character_anim_params[my_pet.profile.species];

	switch(vpet_state) {
		case STATE_DEFAULT:
			if(!Random(animation_params->jump_chance) && pet_animation_frame != CF_JUMP) { // Jump sometimes
				pet_animation_frame = CF_JUMP;
				pet_animation_y -= HAPPY_JUMP_HEIGHT; // Jump up
				goto draw_animation_frame;
			}
			if(pet_animation_frame == CF_JUMP) {
				pet_animation_y += HAPPY_JUMP_HEIGHT; // Fall back down
				pet_animation_frame = CF_IDLE;
				goto draw_animation_frame;
			}

			pet_animation_frame = CF_IDLE;
			if(!Random(animation_params->second_frame_chance)) { // Use the other animation frame
				pet_animation_frame = CF_IDLE2;
			}
			if(!Random(animation_params->flip_chance)) { // Flip
				pet_animation_hflip ^= 1;
			}
			if(!Random(2)) { // Move
				if(pet_animation_target_x > pet_animation_x) {
					pet_animation_x += Random(4) + 1;
					if(animation_params->flip_when_moving)
						pet_animation_hflip = 1;
				} else {
					pet_animation_x -= Random(4) + 1;
					if(animation_params->flip_when_moving)
						pet_animation_hflip = 0;
				}
				pet_animation_x = intclamp(pet_animation_x, 9, PET_SCREEN_W-9);

				if(pet_animation_target_y > pet_animation_y) {
					pet_animation_y += Random(2);
				} else {
					pet_animation_y -= Random(2);
				}
				pet_animation_y = intclamp(pet_animation_y, 10, PET_SCREEN_H-10);
			}

			if((pet_animation_timer & 15) == 0) {
				pet_animation_target_x = RandomMinMax(9, PET_SCREEN_W-9);
				pet_animation_target_y = RandomMinMax(10, PET_SCREEN_H-10);
			}
		draw_animation_frame:
			vpet_clear_screen();
			vpet_draw_pet_animation();
			break;
		case STATE_EATING:
			if(pet_animation_timer > 7) {
				vpet_switch_state(STATE_HAPPY_JUMP);
				goto draw_animation_frame;
			} else if(pet_animation_timer > 1) {
				if(pet_animation_frame == CF_EATING) {
					pet_animation_frame = CF_EATING2;
				} else if(pet_animation_frame == CF_EATING2) {
					pet_animation_frame = CF_EATING;
				}
			}
			goto draw_animation_frame;
		case STATE_HAPPY_JUMP:
			if(pet_animation_timer > 5) {
				vpet_switch_state(STATE_DEFAULT);
				return;
			}
			if(pet_animation_frame == CF_IDLE) {
				pet_animation_frame = CF_JUMP;
				pet_animation_y -= HAPPY_JUMP_HEIGHT;
			} else if(pet_animation_frame == CF_JUMP) {
				pet_animation_frame = CF_IDLE;
				pet_animation_y += HAPPY_JUMP_HEIGHT;
			}
			goto draw_animation_frame;			
		default:
			break;
	}
	pet_animation_timer++;
}
