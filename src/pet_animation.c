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

extern const uint16_t heart10_art[];
extern const uint8_t heart7_art[];
extern const uint16_t bath_art[];
extern const uint8_t bath_lines_art[];
extern const uint8_t poop_art[];

void vpet_draw_meter(int x, int y, int w, int h, unsigned int value, unsigned int max_value);

enum idle_animation current_idle_animation;
unsigned int pet_animation_timer;
int pet_animation_x, pet_animation_y;
int pet_animation_target_x, pet_animation_target_y;
enum character_frame pet_animation_frame;
int pet_animation_hflip;
enum food_id pet_food_to_eat;
int pet_animation_react_timer; // Reacting to a press and then going back to normal

int intclamp(int a, int min, int max) {
	if(a < min)
		return min;
	if(a > max)
		return max;
	return a;
}

void init_animation_for_state(enum game_state new_state) {
	pet_animation_frame = CF_IDLE;
	pet_animation_hflip = 0;
	pet_animation_x = PET_SCREEN_CENTER_X;
	pet_animation_y = PET_SCREEN_CENTER_Y;
	pet_animation_react_timer = 0;
	pet_animation_timer = 0;

	switch(new_state) {
		case STATE_DEFAULT:;
			vpet_set_idle_animation(current_idle_animation); // Re-set the idle animation
			pet_animation_target_x = PET_SCREEN_CENTER_X;
			pet_animation_target_y = PET_SCREEN_CENTER_Y;
			break;

		case STATE_EATING:
			pet_animation_frame = CF_EATING;
			pet_animation_x = PET_SCREEN_CENTER_X + 8;
			break;

		case STATE_NO_THANKS:
			pet_animation_frame = CF_NO_THANKS;
			break;

		default:
			break;
	}
}

void vpet_set_idle_animation(enum idle_animation new_idle_animation) {
	current_idle_animation = new_idle_animation;

	switch(new_idle_animation) {
		default:
			pet_animation_frame = CF_IDLE;
			break;
		case IDLE_ANIM_POOP:
			pet_animation_frame = CF_NO_THANKS;
			pet_animation_x = PET_SCREEN_CENTER_X;
			pet_animation_y = PET_SCREEN_CENTER_Y;
			break;
	}
}

void vpet_draw_pet_animation() {
	// Draw under
	switch(vpet_state) {
		case STATE_BRUSHING:
		case STATE_PETTING:
			vpet_draw_text(PET_SCREEN_CENTER_X - 9 * 2, PET_SCREEN_H-6, "B to stop");
			if(pet_animation_react_timer) {
				if(Random(2)) {
					for(int i=0; i<4; i++) {
						vpet_sprite_16(i*12, RandomMinMax(PET_SCREEN_CENTER_Y - 5 - 8, PET_SCREEN_CENTER_Y - 5 + 4), 0, 10, heart10_art, vpet_or_8_pixels);
					}
				} else {
					for(int i=0; i<6; i++) {
						vpet_sprite_8(i*8, RandomMinMax(PET_SCREEN_CENTER_Y - 3 - 8, PET_SCREEN_CENTER_Y - 3 + 4), 0, 7, heart7_art, vpet_or_8_pixels);
					}
				}
			}
			break;
		case STATE_BATHING:
			if(my_pet.stats[STAT_CLEAN] < (MAX_STAT - MAX_STAT/32))
				vpet_draw_meter(PET_SCREEN_CENTER_X-(4*5+2)/2, 0, 4*5+2, 5, my_pet.stats[STAT_CLEAN], MAX_STAT);
			vpet_draw_pet_crop(pet_animation_x-16/2, pet_animation_y-16/2, pet_animation_hflip,  my_pet.profile.species, pet_animation_frame, 13);
			vpet_sprite_16(PET_SCREEN_CENTER_X - 8, PET_SCREEN_CENTER_Y + 4, 0, 8, bath_art, vpet_or_8_pixels);

			vpet_sprite_8(PET_SCREEN_CENTER_X-4-12, PET_SCREEN_CENTER_Y-8, pet_animation_timer&1, 9, bath_lines_art, vpet_or_8_pixels);
			vpet_sprite_8(PET_SCREEN_CENTER_X-4+12, PET_SCREEN_CENTER_Y-8, (pet_animation_timer&1)^1, 9, bath_lines_art, vpet_or_8_pixels);

			return;
		case STATE_DEFAULT:
			if(my_pet.poops) {
				for(int i=0; i<my_pet.poops; i++) {
					vpet_sprite_8((i&1) ? 0 : PET_SCREEN_W-7, PET_SCREEN_H-8-((i/2)*8), 0, 7, &poop_art[(pet_animation_timer&1)*7], vpet_or_8_pixels);
				}
			}
			if(my_pet.pooping_timer == 1)
				vpet_sprite_8(pet_animation_x + (pet_animation_hflip ? -12-4 : 12-4), pet_animation_y+1, 0, 7, poop_art, vpet_or_8_pixels);
			if(my_pet.pooping_timer > 1) {
				vpet_sprite_8(PET_SCREEN_CENTER_X-4-12, PET_SCREEN_CENTER_Y-2, my_pet.pooping_timer&1, 9, bath_lines_art, vpet_or_8_pixels);
				vpet_sprite_8(PET_SCREEN_CENTER_X-4+12, PET_SCREEN_CENTER_Y-2, (my_pet.pooping_timer&1)^1, 9, bath_lines_art, vpet_or_8_pixels);
			}
			break;
		default:
			break;
	}

	// Assume screen was cleared
	vpet_draw_pet(pet_animation_x-16/2, pet_animation_y-16/2, pet_animation_hflip,  my_pet.profile.species, pet_animation_frame);
	if(pet_animation_frame == CF_JUMP)
		vpet_hline(pet_animation_x-4, pet_animation_y+16/2+HAPPY_JUMP_HEIGHT, 8);

	// Draw over
	switch(vpet_state) {
		case STATE_EATING:
			if(pet_animation_timer < 6)
				vpet_draw_food(PET_SCREEN_CENTER_X - 16, pet_animation_timer ? PET_SCREEN_CENTER_Y-16/2 : 0, pet_food_to_eat, pet_animation_timer / 2);
			break;
		default:
			break;
	}
}

void vpet_draw_pet_animation_and_clear() {
	vpet_clear_screen();
	vpet_draw_pet_animation();
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

	if(vpet_state == STATE_DEFAULT) {
		switch(current_idle_animation) {
			case IDLE_ANIM_WANDER:
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
				goto draw_animation_frame;
			case IDLE_ANIM_POOP:
				if(my_pet.pooping_timer == 1)
					pet_animation_frame = CF_EATING2;
				else if((pet_animation_timer & 1) ^ pet_animation_hflip)
					pet_animation_x--;
				else
					pet_animation_x++;
				goto draw_animation_frame;
			default:
				break;
		}
	}

	switch(vpet_state) {
		draw_animation_frame:
			vpet_clear_screen();
			vpet_draw_pet_animation();
			break;

		case STATE_EATING:
			if(pet_animation_timer > 7) {
				vpet_switch_state(STATE_DEFAULT);
				return;
			} else if(food_infos[pet_food_to_eat].flags & FOOD_DONT_EAT_PLATE && pet_animation_timer >= 6) {
				pet_animation_frame = CF_EATING;
				if(pet_animation_timer == 7) {
					vpet_switch_state(STATE_DEFAULT);
					return;
				}
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

		case STATE_NO_THANKS:
			if(pet_animation_timer > 4) {
				vpet_switch_state(STATE_DEFAULT);
				return;
			}
			pet_animation_hflip ^= 1;
			goto draw_animation_frame;

		case STATE_BATHING:
			if(pet_animation_timer % 2 == 0) {
				if(pet_animation_frame == CF_IDLE) {
					pet_animation_frame = CF_HAPPY;
					pet_animation_y--;
				} else if(pet_animation_frame == CF_HAPPY) {
					pet_animation_frame = CF_IDLE;
					pet_animation_y++;
				}
			}
			goto draw_animation_frame;

		case STATE_BRUSHING:
		case STATE_PETTING:
			if(pet_animation_react_timer) {
				pet_animation_react_timer--;
			} else {
				pet_animation_frame = CF_IDLE;
				if(!Random(animation_params->second_frame_chance)) { // Use the other animation frame
					pet_animation_frame = CF_IDLE2;
				}
			}
			if(!Random(animation_params->flip_chance)) { // Flip
				pet_animation_hflip ^= 1;
			}
			goto draw_animation_frame;

		default:
			break;
	}
	pet_animation_timer++;
}

// ----------------------------------------------------------------------------

void vpet_animation_press_brushing() {
	pet_animation_react_timer = 1;
	pet_animation_frame = CF_HAPPY;
}

void vpet_animation_press_petting() {
	pet_animation_react_timer = 1;
	pet_animation_frame = CF_HAPPY;
}
