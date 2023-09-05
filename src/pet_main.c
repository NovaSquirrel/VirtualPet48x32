#include "vpet.h"

void vpet_init() {
	vpet_clear_screen();

	vpet_draw_pet(0, 10, 0,  CHARACTER_MIMI,    CF_HAPPY);
	vpet_draw_pet(16, 10, 0, CHARACTER_PYONKO,  CF_HAPPY);
	vpet_draw_pet(32, 10, 0, CHARACTER_TERRIER, CF_HAPPY);

	vpet_draw_textf(17, 2, "Cute");
}

void vpet_run() {

}
