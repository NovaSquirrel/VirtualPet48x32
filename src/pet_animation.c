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

void vpet_draw_pet_animation() {
	// Assume screen was cleared
	vpet_draw_pet(PET_SCREEN_CENTER_X-16/2, 10, 0,  my_pet.profile.species, CF_IDLE);
}

// Runs every half second to update the animations.
void vpet_tick_half_second() {
	switch(vpet_state) {
		case STATE_DEFAULT:
			vpet_clear_screen();
			vpet_draw_pet_animation();
			break;
		default:
			break;
	}
}
