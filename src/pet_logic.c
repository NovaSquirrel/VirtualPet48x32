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

// Runs every second. Handles the logic that causes events to happen and causes stats to change over time.
void vpet_tick_second() {
	my_pet.seconds++;

	if(my_pet.poops) // If there's poop, cleanliness drops faster
		sub_from_pet_stat(STAT_CLEAN, my_pet.stat_drop_rate[STAT_CLEAN]*my_pet.poops);

	for(int stat=0; stat<STAT_COUNT; stat++) {
		// Decrease each stat according to its rate (which maybe zero)
		unsigned int value = my_pet.stats[stat];
		if(!value)
			continue;
		if(value >= my_pet.stat_drop_rate[stat])
			my_pet.stats[stat] -= my_pet.stat_drop_rate[stat];
		else
			my_pet.stats[stat] = 0;

		// React to a stat being dropped to zero
		if(!my_pet.stats[stat]) {
			switch(stat) {
				case STAT_BELLY:
					break;
				case STAT_HAPPY:
					break;
				case STAT_CLEAN:
					break;

				case STAT_POOP:
					my_pet.stats[STAT_POOP] = MAX_STAT;
					my_pet.pooping_timer = 10;
					vpet_set_idle_animation(IDLE_ANIM_POOP);
					break;
				case STAT_ATTENTION:
					break;
				case STAT_COMMON_EVENT:
					break;
				case STAT_UNCOMMON_EVENT:
					break;
			}
		}
	}
	if(my_pet.pooping_timer && vpet_state == STATE_DEFAULT) {
		my_pet.pooping_timer--;
		if(!my_pet.pooping_timer) {
			my_pet.poops++;
			sub_from_pet_stat(STAT_CLEAN, MAX_STAT/6);
			vpet_set_idle_animation(IDLE_ANIM_WANDER);
		}
	}
	if(vpet_state == STATE_BATHING)
		add_to_pet_stat(STAT_CLEAN, MAX_STAT/24);
	if(vpet_state == STATE_STATUS) // Show the updated stats
		vpet_refresh_screen();
}
