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
int my_currency = 12345;
int second_ticks = 0; // Ticks up to 59 and then back to zero
enum game_state vpet_state = STATE_DEFAULT;

// Menu options and stuff?
int menu_cursor = 0;
int menu_page = 0;

// ----------------------------------------------

const char* gender_names[] = {"None", "Other", "Girl", "Boy", "Nonbinary"};
const char* personality_names[] = {"?", "Kind", "Energetic", "Laid back", "Cool", "Silly", "Stubborn", "Cautious"};

// ----------------------------------------------

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

// Redraw the screen, like in response to changing between states
void vpet_refresh_screen() {
	char line_buffer[13];
	struct tm *time_info;

	vpet_clear_screen();
	switch(vpet_state) {
		case STATE_DEFAULT:
			vpet_draw_pet_animation();
			break;

		case STATE_MAIN_MENU:
			vpet_draw_text(4, 1, "Main Menu");
			vpet_hline(2, 6, PET_SCREEN_W-4);
			if(menu_cursor < 4) {
				vpet_draw_text(4, 6*1+2, "Status");
				vpet_draw_text(4, 6*2+2, "Feed");
				vpet_draw_text(4, 6*3+2, "Play");
				vpet_draw_text(4, 6*4+2, "Clean");
			} else {
				vpet_draw_text(4, 6*1+2, "Travel");
				vpet_draw_text(4, 6*2+2, "Records");
				vpet_draw_text(4, 6*3+2, "Options");
				vpet_draw_text(4, 6*4+2, "Pause");
			}
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;

		case STATE_STATUS:
			switch(menu_cursor) {
				case 0:
					vpet_draw_text(1,  6*0, my_pet.profile.name);
					vpet_hline(1, 6, PET_SCREEN_W-2);
					vpet_draw_text(2,  6*1+1, "Belly:99999");
					vpet_draw_text(2,  6*2+1, "Happy:99999");
					vpet_draw_text(2,  6*3+1, "Clean:99999");
					vpet_draw_text(2,  6*4+1, "Heavy:99999");
					break;
				case 1:
					vpet_draw_text(1,  6*0, my_pet.profile.name);
					vpet_hline(1, 6, PET_SCREEN_W-2);
					vpet_draw_text(2,   6*1+1, "Money:");
					vpet_draw_textf(4,  6*2+1, "%d", my_currency);
					vpet_draw_text(2,   6*3+1, "Gratitude:");
					vpet_draw_textf(4,  6*4+1, "%d", my_pet.gratitude);
					break;
				case 2:
					vpet_draw_text(2, 6*0+1,  " Cool:99999");
					vpet_draw_text(2, 6*1+1,  "Style:99999");
					vpet_draw_text(2, 6*2+1,  " Cute:99999");
					vpet_draw_text(2, 6*3+1,  "Smart:99999");
					vpet_draw_text(2, 6*4+1,  "Tough:99999");
					break;
				case 3:
					vpet_draw_text(1,  6*0, my_pet.profile.name);
					vpet_hline(1, 6, PET_SCREEN_W-2);
					vpet_draw_text(2,  6*1+1, "Personality:");
					vpet_draw_text(4,  6*2+1, personality_names[my_pet.profile.personality]);
					vpet_draw_text(2,  6*3+1, "Gender:");
					vpet_draw_text(4,  6*4+1, gender_names[my_pet.profile.gender]);
					break;
				case 4:
					time_info = localtime(&my_pet.profile.created_at);
					strftime(line_buffer, sizeof(line_buffer), "%Y-%m-%d", time_info);

					vpet_draw_text(1,  6*0, my_pet.profile.name);
					vpet_hline(1, 6, PET_SCREEN_W-2);
					vpet_draw_text(2,  6*1+1, "Met on:");
					vpet_draw_text(4,  6*2+1, line_buffer);
					vpet_draw_text(2,  6*3+1, "Days active:");
					vpet_draw_textf(4,  6*4+1, "%d", my_pet.seconds / 86400);
			}
			break;
		case STATE_PAUSED:
			vpet_draw_text(PET_SCREEN_CENTER_X - 2*6, 2, "Paused");
			vpet_draw_text(PET_SCREEN_CENTER_X - 2*9, 2+6, "Press A+B");
			vpet_draw_pet(PET_SCREEN_CENTER_X-16/2, 14, 0,  my_pet.profile.species, CF_IDLE);
			break;
		default:
			break;
	}
}

void vpet_switch_state(enum game_state new_state) {
	menu_cursor = 0;
	menu_page = 0;
	vpet_state = new_state;
	second_ticks = 0;
	vpet_refresh_screen();
}

void move_through_menu_generic(int *value, int item_count, uint16_t keys_back, uint16_t keys_forward, int allow_repeat, enum game_state exit_state) {
	if((allow_repeat ? key_new_or_repeat : key_new) & keys_back) {
		(*value)--;
		if((*value) < 0)
			(*value) = item_count - 1;
	}
	if((allow_repeat ? key_new_or_repeat : key_new) & keys_forward) {
		(*value)++;
		if((*value) >= item_count)
			(*value) = 0;
	}
	if(key_new & KEY_B) {
		vpet_switch_state(exit_state);
	}
}

void move_through_menu(int item_count, enum game_state exit_state) {
	move_through_menu_generic(&menu_cursor, item_count, KEY_UP, KEY_DOWN, 1, exit_state);
}

// Tick because of a button press
void vpet_tick_button_press() {
	switch(vpet_state) {
		case STATE_DEFAULT:
			vpet_switch_state(STATE_MAIN_MENU);
			break;
		case STATE_MAIN_MENU:
			move_through_menu(8, STATE_DEFAULT);
			if(key_new & KEY_A) {
				static const enum game_state new_state[] = {STATE_STATUS, STATE_FEED_MENU, STATE_PLAY_MENU, STATE_CLEAN_MENU, STATE_TRAVEL_MENU, STATE_RECORDS_MENU, STATE_OPTIONS_MENU, STATE_PAUSED};
				vpet_switch_state(new_state[menu_cursor]);
			}
			if(key_new_or_repeat & (KEY_UP | KEY_DOWN))
				vpet_refresh_screen();
			break;
		case STATE_PAUSED:
			if( (key_down & (KEY_A | KEY_B)) == (KEY_A | KEY_B) )
				vpet_switch_state(STATE_DEFAULT);
			break;
		case STATE_STATUS:
			move_through_menu_generic(&menu_cursor, 5, KEY_LEFT, KEY_RIGHT|KEY_A, 0, STATE_DEFAULT);
			if(key_new)
				vpet_refresh_screen();
			break;
		default:
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
	if(key_new_or_repeat) {
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
