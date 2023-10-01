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
int my_currency = 0;
uint8_t food_inventory[MAX_FOOD_INVENTORY_SLOTS] = {0};

uint8_t filtered_menu_options[MAX_FOOD_INVENTORY_SLOTS];
size_t filtered_menu_option_count;

// Pet state
int second_ticks = 0; // Ticks up to 59 and then back to zero
enum game_state vpet_state = STATE_DEFAULT;
int pet_paused = 0;

// Miscellaneous state things
int state_variable; // Button to press in STATE_BRUSHING

// Menu options and stuff?
int menu_cursor = 0;
int menu_page = 0;
int menu_option; // For holding onto a menu option, like a filter or something

// External variables
extern enum food_id pet_food_to_eat;

// ----------------------------------------------

const char* gender_names[] = {"None", "Other", "Girl", "Boy", "Nonbinary"};
const char* personality_names[] = {"?", "Kind", "Energetic", "Laid back", "Cool", "Silly", "Stubborn", "Cautious"};

const uint16_t main_menu_icons[] = {0, 8184, 8196, 17346, 17442, 17442, 16418, 16578, 16770, 16386, 16770, 8196, 7224, 576, 1152, 1792, 8188, 4116, 4116, 4116, 4100, 8188, 4100, 4116, 4116, 4116, 4100, 4100, 4100, 4100, 8188, 3640, 256, 128, 256, 256, 128, 32766, 16386, 22530, 31758, 31854, 22626, 16386, 32766, 0, 0, 0, 0, 0, 480, 1296, 2360, 4596, 9228, 18698, 4618, 9226, 18442, 4106, 8202, 10, 10, 0, 0, 0, 12384, 10448, 9544, 8772, 8772, 12916, 10964, 12116, 8788, 8780, 12868, 2724, 1820, 0, 0, 16380, 8196, 8198, 13130, 11412, 8236, 8276, 13476, 10948, 8196, 8196, 8196, 8196, 16380, 0, 3584, 4352, 3328, 17664, 42240, 47360, 16512, 14400, 1056, 528, 264, 132, 66, 36, 24, 0, 0, 5120, 5120, 16640, 23808, 15872, 15872, 0, 0, 40, 40, 130, 186, 124, 124, 0};

// ----------------------------------------------

void vpet_reroll_stat_drop_rates() {
	my_pet.stat_drop_rate[STAT_BELLY] = MAX_STAT / RandomMinMax(4*60*50, 4*60*100);
	my_pet.stat_drop_rate[STAT_HAPPY] = MAX_STAT / RandomMinMax(4*60*50, 4*60*100);
	my_pet.stat_drop_rate[STAT_CLEAN] = MAX_STAT / RandomMinMax(4*60*50, 4*60*100);

	my_pet.stat_drop_rate[STAT_POOP]           = MAX_STAT / RandomMinMax(60*160, 60*200); // P1 uses 180 minutes, or 25 for babies
	my_pet.stat_drop_rate[STAT_ATTENTION]      = MAX_STAT / RandomMinMax(60*60*3, 60*60*5);
	my_pet.stat_drop_rate[STAT_COMMON_EVENT]   = MAX_STAT / RandomMinMax(60*60*3, 60*60*5);
	my_pet.stat_drop_rate[STAT_UNCOMMON_EVENT] = MAX_STAT / RandomMinMax(60*60*5, 60*60*10);
}

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

	my_pet.stats[STAT_BELLY] =          0; //MAX_STAT;
	my_pet.stats[STAT_HAPPY] =          0; //MAX_STAT;
	my_pet.stats[STAT_CLEAN] =          0; //MAX_STAT;
	my_pet.stats[STAT_POOP]           = MAX_STAT;
	my_pet.stats[STAT_ATTENTION]      = MAX_STAT;
	my_pet.stats[STAT_COMMON_EVENT]   = MAX_STAT;
	my_pet.stats[STAT_UNCOMMON_EVENT] = MAX_STAT;
	vpet_reroll_stat_drop_rates();

	memset(food_inventory, 99, sizeof(food_inventory));
	memset(food_inventory, 255, 8);

	vpet_set_idle_animation(IDLE_ANIM_WANDER);
	void vpet_switch_state(enum game_state new_state);
	vpet_switch_state(STATE_DEFAULT);
}

void vpet_draw_meter(int x, int y, int w, int h, unsigned int value, unsigned int max_value) {
	vpet_rect(x, y, w, h);
	int per_pixel = max_value/(w-2);
	vpet_rectfill(x+1, y+1, (value+(per_pixel/2))/per_pixel, h-2);
}


void vpet_draw_header(const char *text) {
	int x = 4;
	int y = 1;
	for(const char *c = text; *c; c++) {
		if(*c == 'j' || *c == 'p' || *c == 'q' || *c == 'y') { // Detect descenders
			y = 0;
			break;
		}
	}
	if(strlen(text) >= 12)
		x = 1;
	vpet_draw_text(x, y, text);
	vpet_hline(2, 6, PET_SCREEN_W-4);
}

void vpet_draw_four_menu_items(const char *item1, const char *item2, const char *item3, const char *item4) {
	if(item1)
		vpet_draw_text(4, 6*1+2, item1);
	if(item2)
		vpet_draw_text(4, 6*2+2, item2);
	if(item3)
		vpet_draw_text(4, 6*3+2, item3);
	if(item4)
		vpet_draw_text(4, 6*4+2, item4);
}

int wrap_within_limit(int value, int max) {
	if(value < 0)
		return max+value;
	if(value >= max)
		return max-value;
	return value;
}

// Redraw the screen, like in response to changing between states
void vpet_refresh_screen() {
	char line_buffer[13];
	struct tm *time_info;

	vpet_clear_screen();
	switch(vpet_state) {
		case STATE_MAIN_MENU:
			vpet_draw_header("Main Menu");
			if(menu_cursor < 4) {
				vpet_draw_four_menu_items("Status", "Feed", "Play", "Clean");
			} else {
				vpet_draw_four_menu_items("Travel", "Records", "Options", "Pause");
			}
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			vpet_sprite_xor_16(PET_SCREEN_W-16, PET_SCREEN_CENTER_Y-16/2, 0, 16, &main_menu_icons[menu_cursor*16]);
			break;

		case STATE_STATUS:
			switch(menu_cursor) {
				case 0:
					vpet_draw_header(my_pet.profile.name);
					vpet_draw_text(1,  6*1+2, "Belly:");
					vpet_draw_meter(2+4*6-1, 6*1+2, 4*5+2, 5, my_pet.stats[STAT_BELLY], MAX_STAT);
					vpet_draw_text(1,  6*2+2, "Happy:");
					vpet_draw_meter(2+4*6-1, 6*2+2, 4*5+2, 5, my_pet.stats[STAT_HAPPY], MAX_STAT);
					vpet_draw_text(1,  6*3+2, "Clean:");
					vpet_draw_meter(2+4*6-1, 6*3+2, 4*5+2, 5, my_pet.stats[STAT_CLEAN], MAX_STAT);
					vpet_draw_text(1,  6*4+2, "Heavy:");
					vpet_draw_meter(2+4*6-1, 6*4+2, 4*5+2, 5, my_pet.stats[STAT_HEAVY], MAX_STAT);
					break;
				case 1:
					vpet_draw_header(my_pet.profile.name);
					vpet_draw_text(1,   6*1+2, "Money:");
					vpet_draw_textf(4,  6*2+2, "%d", my_currency);
					vpet_draw_text(1,   6*3+2, "Gratitude:");
					vpet_draw_textf(4,  6*4+2, "%d", my_pet.gratitude);
					break;
				case 2:
					vpet_draw_textf(1, 6*0+2,  " Cool:%d", my_pet.stats[STAT_COOL]);
					vpet_draw_textf(1, 6*1+2,  "Style:%d", my_pet.stats[STAT_BEAUTIFUL]);
					vpet_draw_textf(1, 6*2+2,  " Cute:%d", my_pet.stats[STAT_CUTE]);
					vpet_draw_textf(1, 6*3+2,  "Smart:%d", my_pet.stats[STAT_CLEVER]);
					vpet_draw_textf(1, 6*4+2,  "Tough:%d", my_pet.stats[STAT_TOUGH]);
					break;
				case 3:
					vpet_draw_header(my_pet.profile.name);
					vpet_draw_text(1,  6*1+2, "Personality:");
					vpet_draw_text(4,  6*2+2, personality_names[my_pet.profile.personality]);
					vpet_draw_text(1,  6*3+2, "Gender:");
					vpet_draw_text(4,  6*4+2, gender_names[my_pet.profile.gender]);
					break;
				case 4:
					time_info = localtime(&my_pet.profile.created_at);
					strftime(line_buffer, sizeof(line_buffer), "%Y-%m-%d", time_info);

					vpet_draw_header(my_pet.profile.name);
					vpet_draw_text(1,  6*1+2, "Met on:");
					vpet_draw_text(4,  6*2+2, line_buffer);
					vpet_draw_text(1,  6*3+2, "Days active:");
					vpet_draw_textf(4, 6*4+2, "%d", my_pet.seconds / 86400);
			}
			break;

		case STATE_FEED_MENU:
			vpet_draw_header("Feed");
			if(menu_cursor < 4) {
				vpet_draw_four_menu_items("Grains", "Protein", "Produce", "Cooked");
			} else {
				vpet_draw_four_menu_items("Dessert", "Frozen", "Drink", "All");
			}
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;
		case STATE_PLAY_MENU:
			vpet_draw_header("Play!");
			vpet_draw_four_menu_items("Game", "Item", "Petting", "Explore");
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;
		case STATE_CLEAN_MENU:
			vpet_draw_header("Clean");
			vpet_draw_four_menu_items("Brushing", "Bath", NULL, NULL);
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;
		case STATE_TRAVEL_MENU:
			vpet_draw_header("Travel");
			if(menu_cursor < 4) {
				vpet_draw_four_menu_items("Home", "Shop", "Library", "Park");
			} else {
				vpet_draw_four_menu_items("Forest", "Beach", "Lake", "Mountains");
			}
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;
		case STATE_RECORDS_MENU:
			vpet_draw_header("Records");
			vpet_draw_four_menu_items("Items", "Pets", "?", "?");
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;
		case STATE_OPTIONS_MENU:
			vpet_draw_header("Options");
			vpet_draw_four_menu_items("Device", "Sound", "About", "Reset");
			vpet_draw_text(0, 6*((menu_cursor%4)+1)+2, ">");
			break;

		case STATE_PAUSED:
			pet_paused = 1;
			vpet_draw_text(PET_SCREEN_CENTER_X - 2*6, 2, "Paused");
			vpet_draw_text(PET_SCREEN_CENTER_X - 2*9, 2+6, "Press " PET_KEY_NAME_A "+" PET_KEY_NAME_B);
			vpet_draw_pet(PET_SCREEN_CENTER_X-16/2, 14, 0,  my_pet.profile.species, CF_IDLE);
			break;

		case STATE_WHICH_FOOD:
			{
				enum food_id which_food = (enum food_id)filtered_menu_options[menu_cursor];
				vpet_draw_food(PET_SCREEN_CENTER_X-(16/2)-4-16, 0,   (enum food_id)filtered_menu_options[wrap_within_limit(menu_cursor-1, filtered_menu_option_count)], 0);
				vpet_draw_food(PET_SCREEN_CENTER_X-(16/2),        0, which_food, 0);
				vpet_draw_food(PET_SCREEN_CENTER_X-(16/2)+4+16,   0, (enum food_id)filtered_menu_options[wrap_within_limit(menu_cursor+1, filtered_menu_option_count)], 0);
				vpet_draw_text(PET_SCREEN_CENTER_X - strlen(food_infos[which_food].name) * 2, 17, food_infos[which_food].name);

				int amount = food_inventory[which_food];
				if(amount == 255)
					strcpy(line_buffer, "Inf");
				else
					sprintf(line_buffer, "x%-2d", amount);
				vpet_draw_textf(0, 17+6+2, "%2d/%d", menu_cursor+1, filtered_menu_option_count);
				vpet_draw_text(PET_SCREEN_W - strlen(line_buffer) * 4, 17+6+2, line_buffer);
			}
			break;

		case STATE_WHICH_GAME:
			break;

		case STATE_WHICH_ITEM:
			break;

		case STATE_BRUSHING:
		case STATE_PETTING:
		case STATE_BATHING:
		case STATE_EXPLORE:
		case STATE_EATING:
		case STATE_DEFAULT:
		case STATE_NO_THANKS:
		case STATE_HAPPY_JUMP:
		case STATE_ON_TOILET:
			vpet_draw_pet_animation();
			break;

		default:
			break;
	}
}

void back_out_of_menu(enum game_state new_state, int new_cursor) {
	menu_cursor = new_cursor;
	menu_page = 0;
	vpet_state = new_state;
	second_ticks = 0;

	void init_animation_for_state(enum game_state state);
	init_animation_for_state(new_state);
	vpet_refresh_screen();
}

void vpet_switch_state(enum game_state new_state) {
	switch(new_state) {
		case STATE_BRUSHING:
			#ifdef KEY_LAYOUT_HORIZ_5
			switch(Random(4)) {
				case 0: state_variable = 4; break;
				case 1: state_variable = 6; break;
				case 2: state_variable = 7; break;
				case 3: state_variable = 8; break;
			}
			#else
			state_variable = Random(5);
			#endif
			break;
		case STATE_MINIGAME:;
			void vpet_init_minigame();
			vpet_init_minigame();
			break;
		default:
			break;
	}

	back_out_of_menu(new_state, 0);
}

void move_through_menu_generic(int *value, int item_count, uint16_t keys_back, uint16_t keys_forward, int allow_repeat, enum game_state exit_state, int exit_cursor) {
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
	if(key_new & VPET_KEY_B) {
		back_out_of_menu(exit_state, exit_cursor);
	}
}

void move_through_menu(int item_count, enum game_state exit_state, int exit_cursor) {
	move_through_menu_generic(&menu_cursor, item_count, VPET_KEY_UP_PREV, VPET_KEY_DOWN_NEXT, 1, exit_state, exit_cursor);
}

void add_to_pet_stat(int which_stat, unsigned int value) {
	my_pet.stats[which_stat] += value;
	if(my_pet.stats[which_stat] > MAX_STAT)
		my_pet.stats[which_stat] = MAX_STAT;
}

void sub_from_pet_stat(int which_stat, unsigned int value) {
	if(!my_pet.stats[which_stat])
		return;
	if(my_pet.stats[which_stat] <= value) {
		my_pet.stats[which_stat] = 1;
		return;
	}
	my_pet.stats[which_stat] -= value;
}

// Tick because of a button press
void vpet_tick_button_press() {
	switch(vpet_state) {
		case STATE_DEFAULT:
			if(key_new & VPET_KEY_A)
				vpet_switch_state(STATE_MAIN_MENU);
			break;

		case STATE_MAIN_MENU:
			move_through_menu(8, STATE_DEFAULT, 0);
			if(key_new & VPET_KEY_A) {
				static const enum game_state new_state[] = {STATE_STATUS, STATE_FEED_MENU, STATE_PLAY_MENU, STATE_CLEAN_MENU, STATE_TRAVEL_MENU, STATE_RECORDS_MENU, STATE_OPTIONS_MENU, STATE_PAUSED};
				if(menu_cursor == 3 && (my_pet.pooping_timer || my_pet.poops)) {
					my_pet.poops = 0;
					vpet_set_idle_animation(IDLE_ANIM_WANDER);
					if(my_pet.pooping_timer) {
						add_to_pet_stat(STAT_HAPPY, MAX_STAT / 4);
						vpet_switch_state(STATE_ON_TOILET);
					} else {
						vpet_switch_state(STATE_HAPPY_JUMP);
					}
					my_pet.pooping_timer = 0;
					return;
				}

				vpet_switch_state(new_state[menu_cursor]);
			}
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_PAUSED:
			if( (key_down & (VPET_KEY_A | VPET_KEY_B)) == (VPET_KEY_A | VPET_KEY_B) ) {
				vpet_switch_state(STATE_DEFAULT);
				pet_paused = 0;
			}
			break;

		case STATE_STATUS:
			move_through_menu_generic(&menu_cursor, 5, VPET_KEY_LEFT_PREV, VPET_KEY_RIGHT_NEXT|VPET_KEY_A, 0, STATE_MAIN_MENU, 0);
			if(key_new)
				vpet_refresh_screen();
			break;

		case STATE_FEED_MENU:
			move_through_menu(8, STATE_MAIN_MENU, 1);
			if(key_new & VPET_KEY_A) {

				unsigned int category_id = ((menu_cursor+1) & 7) << 8;
				filtered_menu_option_count = 0;
				for(int i=0; i<MAX_FOOD_INVENTORY_SLOTS; i++) {
					if(!food_inventory[i])
						continue;
					if(category_id && (food_infos[i].flags & FOOD_CATEGORY_MASK) != category_id)
						continue;
					filtered_menu_options[filtered_menu_option_count++] = i;
				}

				// Only go into the submenu if there's anything to select within it!
				if(filtered_menu_option_count) {
					menu_option = menu_cursor;
					vpet_switch_state(STATE_WHICH_FOOD);
				}
			}
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_PLAY_MENU:
			move_through_menu(4, STATE_MAIN_MENU, 2);
			if(key_new & VPET_KEY_A) {
				static const enum game_state new_state[] = {STATE_WHICH_GAME, STATE_WHICH_ITEM, STATE_PETTING, STATE_EXPLORE};
				vpet_switch_state(new_state[menu_cursor]);
			}
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_CLEAN_MENU:
			move_through_menu(2, STATE_MAIN_MENU, 3);
			if(key_new & VPET_KEY_A) {
				static const enum game_state new_state[] = {STATE_BRUSHING, STATE_BATHING};
				vpet_switch_state(new_state[menu_cursor]);
			}
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_TRAVEL_MENU:
			move_through_menu(8, STATE_MAIN_MENU, 4);
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_RECORDS_MENU:
			move_through_menu(4, STATE_MAIN_MENU, 5);
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();
			break;

		case STATE_OPTIONS_MENU:
			move_through_menu(4, STATE_MAIN_MENU, 6);
			if(key_new_or_repeat & VPET_KEY_UP_DOWN_PREV_NEXT)
				vpet_refresh_screen();

			// Temporary: Let you switch pet species here
			if(key_new & VPET_KEY_A) {
				my_pet.profile.species++;
				if(my_pet.profile.species == CHARACTER_COUNT)
					my_pet.profile.species = CHARACTER_MIMI;
				switch(my_pet.profile.species) {
					case CHARACTER_MIMI:    strcpy(my_pet.profile.name, "Mimitchi"); break;
					case CHARACTER_PYONKO:  strcpy(my_pet.profile.name, "Pyonkotchi"); break;
					case CHARACTER_TERRIER: strcpy(my_pet.profile.name, "Terriermon"); break;
					case CHARACTER_LOP:     strcpy(my_pet.profile.name, "Lopmon"); break;
				}
			}
			break;

		case STATE_WHICH_FOOD:
			if(key_new & VPET_KEY_A) {
				pet_food_to_eat = (enum food_id)filtered_menu_options[menu_cursor];
				if((unsigned)(my_pet.stats[STAT_BELLY] + food_infos[pet_food_to_eat].add_belly) >= ((unsigned)MAX_STAT + (unsigned)MAX_STAT/8)) {
					vpet_switch_state(STATE_NO_THANKS);
					break;
				}

				if(food_inventory[pet_food_to_eat] != 255)
					food_inventory[pet_food_to_eat]--;
				add_to_pet_stat(STAT_BELLY, food_infos[pet_food_to_eat].add_belly);
				add_to_pet_stat(STAT_HAPPY, food_infos[pet_food_to_eat].add_happy);
				add_to_pet_stat(STAT_HEAVY, food_infos[pet_food_to_eat].add_heavy);
				int32_t messiness = food_infos[pet_food_to_eat].messiness;
				if(messiness < 0) {
					add_to_pet_stat(STAT_CLEAN, -messiness);
				} else {
					sub_from_pet_stat(STAT_CLEAN, messiness);
				}
				vpet_reroll_stat_drop_rates();
				vpet_switch_state(STATE_EATING);
			}
			move_through_menu_generic(&menu_cursor, filtered_menu_option_count, VPET_KEY_LEFT_PREV, VPET_KEY_RIGHT_NEXT, 1, STATE_FEED_MENU, menu_option);
			if(key_new_or_repeat & (VPET_KEY_LEFT_RIGHT_PREV_NEXT))
				vpet_refresh_screen();
			break;

		case STATE_WHICH_GAME:
			vpet_switch_state(STATE_MINIGAME);
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;

		case STATE_WHICH_ITEM:
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;

		case STATE_BRUSHING:
			#ifdef KEY_LAYOUT_HORIZ_5
			if(key_new & (VPET_KEY_A|VPET_KEY_C|VPET_KEY_PREV|VPET_KEY_NEXT)) {
			#else
			if(key_new & (VPET_KEY_A|VPET_KEY_LEFT|VPET_KEY_DOWN|VPET_KEY_UP|VPET_KEY_RIGHT)) {
			#endif
				if(key_new & (1 << state_variable)) {
					add_to_pet_stat(STAT_HAPPY, MAX_STAT/32);
					add_to_pet_stat(STAT_CLEAN, MAX_STAT/16);
					void vpet_animation_press_brushing();
					vpet_animation_press_brushing();
					#ifdef KEY_LAYOUT_HORIZ_5
					switch(Random(4)) {
						case 0: state_variable = 4; break;
						case 1: state_variable = 6; break;
						case 2: state_variable = 7; break;
						case 3: state_variable = 8; break;
					}
					#else
					state_variable = Random(5);
					#endif
					vpet_draw_pet_animation_and_clear();
				}
			}
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;
		case STATE_PETTING:
			if(key_new & (VPET_KEY_A|VPET_KEY_LEFT|VPET_KEY_DOWN|VPET_KEY_UP|VPET_KEY_RIGHT)) {
				add_to_pet_stat(STAT_HAPPY, MAX_STAT/12);
				void vpet_animation_press_petting();
				vpet_animation_press_petting();
				vpet_draw_pet_animation_and_clear();
			}
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;
		case STATE_EXPLORE:
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;

		// Cancellable animations
		case STATE_BATHING:
		case STATE_EATING:
		case STATE_NO_THANKS:
			if(key_new & VPET_KEY_A)
				vpet_switch_state(STATE_MAIN_MENU);
			if(key_new & VPET_KEY_B)
				vpet_switch_state(STATE_DEFAULT);
			break;

		default:
			break;
	}
}

// Tick continuously
void vpet_tick_60fps() {
	switch(vpet_state) {
		case STATE_MINIGAME:;
			void vpet_tick60fps_minigame();
			vpet_tick60fps_minigame();
			break;
		default:
			break;
	}
}

// Simulates the interrupt system a device would have
void vpet_run() {
	void vpet_tick_second();
	void vpet_tick_animation();

	// Tick from button presses
	if(key_new_or_repeat) {
		vpet_tick_button_press();
	}

	// Tick every frame
	vpet_tick_60fps();

	// Tick very half second or full second
	second_ticks++;
	if(second_ticks == 30 || second_ticks == 60)
		vpet_tick_animation();
	if(second_ticks >= 60) {
		second_ticks = 0;
		if(!pet_paused) {
			vpet_tick_second();
		}
	}
}
