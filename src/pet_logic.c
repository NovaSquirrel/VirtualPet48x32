#include "vpet.h"

struct vpet_status my_pet;

void vpet_tick() { // Called every second
	for(int stat=0; stat<STAT_COUNT; stat++) {
		// Decrease each stat according to its rate (which maybe zero)
		int value = my_pet.stats[stat];
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
}
