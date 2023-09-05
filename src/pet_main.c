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
#include <time.h>

// Virtual pet game variables
struct vpet_status my_pet;
int second_ticks = 0; // Ticks up to 59 and then back to zero

enum game_state vpet_state = STATE_DEFAULT;

void vpet_init() {
	// Set up a virtual pet
	memset(&my_pet, 0, sizeof(my_pet));
	my_pet.profile.species     = CHARACTER_PYONKO;
	my_pet.profile.gender      = GENDER_GIRL;
	my_pet.profile.personality = PERSONALITY_KIND;
	my_pet.profile.random[0]   = RandomRaw();
	my_pet.profile.random[1]   = RandomRaw();
	my_pet.profile.random[2]   = RandomRaw();
	my_pet.profile.random[3]   = RandomRaw();
	time(&my_pet.profile.created_at);
	strcpy(my_pet.profile.name, "Pyonkotchi");

	my_pet.stats[STAT_BELLY] = MAX_STAT;
	my_pet.stats[STAT_HAPPY] = MAX_STAT;
	my_pet.stats[STAT_CLEAN] = MAX_STAT;

	my_pet.stats[STAT_POOP]           = MAX_STAT;
	my_pet.stats[STAT_ATTENTION]      = MAX_STAT;
	my_pet.stats[STAT_COMMON_EVENT]   = MAX_STAT;
	my_pet.stats[STAT_UNCOMMON_EVENT] = MAX_STAT;

	void vpet_switch_state(enum game_state new_state);
	vpet_switch_state(STATE_DEFAULT);
}

void vpet_switch_state(enum game_state new_state) {
	vpet_state = new_state;

	switch(vpet_state) {
		case STATE_DEFAULT:
			break;
		case STATE_ACTION_MENU:
			vpet_clear_screen();
			vpet_draw_textf(4, 6*0, "Status");
			vpet_draw_textf(4, 6*1, "Feed");
			vpet_draw_textf(4, 6*2, "Play");
			vpet_draw_textf(4, 6*3, "Clean");
			vpet_draw_textf(4, 6*4, "Options");
			break;
		case STATE_STATUS:
			vpet_clear_screen();

			vpet_draw_pet(0, 10, 0,  CHARACTER_MIMI,    CF_HAPPY);
			vpet_draw_pet(16, 10, 0, CHARACTER_PYONKO,  CF_HAPPY);
			vpet_draw_pet(32, 10, 0, CHARACTER_TERRIER, CF_HAPPY);

			vpet_draw_textf(17, 2, "Cute");
			break;
	}
	second_ticks = 59;
}

// Tick because of a button press
void vpet_tick_button_press() {
	switch(vpet_state) {
		case STATE_DEFAULT:
			vpet_switch_state(STATE_ACTION_MENU);
			break;
		case STATE_ACTION_MENU:
			vpet_switch_state(STATE_DEFAULT);
			break;
		case STATE_STATUS:
			break;
	}
}

// Tick continuously
void vpet_tick_60fps() {
	switch(vpet_state) {
		default:
			break;
	}
}

// Simulates the interrupt system a device would have
void vpet_run() {
	void vpet_tick_second();
	void vpet_tick_half_second();

	// Tick from button presses
	if(KeyNew) {
		vpet_tick_button_press();
	}

	// Tick every frame
	vpet_tick_60fps();

	// Tick very half second or full second
	second_ticks++;
	if(second_ticks == 30 || second_ticks == 60)
		vpet_tick_half_second();
	if(second_ticks >= 60) {
		second_ticks = 0;
		vpet_tick_second();
	}
}
