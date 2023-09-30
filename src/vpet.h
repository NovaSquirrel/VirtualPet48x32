#ifndef VPET_HEADER
#define VPET_HEADER

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#ifdef PLATFORM_PC
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>

// ------------------------------------------------------------

// Pet configuration
#ifndef PET_SCREEN_W
#define PET_SCREEN_W 48
#endif
#ifndef PET_SCREEN_H
#define PET_SCREEN_H 32
#endif

#define PET_SCREEN_CENTER_X (PET_SCREEN_W/2)
#define PET_SCREEN_CENTER_Y (PET_SCREEN_H/2)

#ifdef PLATFORM_PICO
	#define PET_KEY_NAME_A "E"
	#define PET_KEY_NAME_B "D"
	#define PET_KEY_NAME_C "C"
	#define KEY_LAYOUT_HORIZ_5
	#ifdef SAVE_BATTERY
		#define PET_ANIMATION_PLUS
	#else
		#define PET_ANIMATION_FULL
	#endif
#elif defined(PLATFORM_WATCHY)
	#define PET_KEY_NAME_A "A"
	#define PET_KEY_NAME_B "B"
	#define PET_KEY_NAME_C "C"
	#define KEY_LAYOUT_QUAD
	#define PET_ANIMATION_EPAPER
#else
	#define PET_KEY_NAME_A "A"
	#define PET_KEY_NAME_B "B"
	#define PET_KEY_NAME_C "C"
	#define KEY_LAYOUT_DPAD_AB
	#define PET_ANIMATION_FULL
#endif

// ------------------------------------------------------------

#define MAX_FOOD_INVENTORY_SLOTS FOOD_COUNT

// ------------------------------------------------------------

void vpet_clear_screen();
void vpet_set_pixel(int x, int y, int value);
int  vpet_get_pixel(int x, int y);
void vpet_render_screen();
void vpet_or_8_pixels(int x, int y, uint8_t value);
void vpet_xor_8_pixels(int x, int y, uint8_t value);
void vpet_and_8_pixels(int x, int y, uint8_t value);
void vpet_andnot_8_pixels(int x, int y, uint8_t value);
void vpet_sprite_8(int x, int y, int hflip, unsigned int h, const uint8_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_16(int x, int y, int hflip, unsigned int h, const uint16_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_24(int x, int y, int hflip, unsigned int h, const uint32_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_overwrite_8(int x, int y, int hflip, unsigned int h, const uint8_t rows[]);
void vpet_sprite_overwrite_16(int x, int y, int hflip, unsigned int h, const uint16_t rows[]);
void vpet_sprite_overwrite_24(int x, int y, int hflip, unsigned int h, const uint32_t rows[]);
void vpet_sprite_mask_8(int x, int y, int hflip, unsigned int h, const uint8_t rows[], const uint8_t mask_rows[]);
void vpet_sprite_mask_16(int x, int y, int hflip, unsigned int h, const uint16_t rows[], const uint16_t mask_rows[]);
void vpet_sprite_mask_24(int x, int y, int hflip, unsigned int h, const uint32_t rows[], const uint32_t mask_rows[]);
#define vpet_sprite_xor_8(x, y, hflip, h, rows) vpet_sprite_8(x, y, hflip, h, rows, vpet_xor_8_pixels)
#define vpet_sprite_xor_16(x, y, hflip, h, rows) vpet_sprite_16(x, y, hflip, h, rows, vpet_xor_8_pixels)
#define vpet_sprite_xor_24(x, y, hflip, h, rows) vpet_sprite_24(x, y, hflip, h, rows, vpet_xor_8_pixels)
extern const uint8_t font_4x6[];
void vpet_set_font(const uint8_t font[], int width, int height);
void vpet_draw_textf(int x, int y, const char *fmt, ...);
void vpet_draw_text(int x, int y, const char *buffer);
void vpet_hline(int x, int y, int width);
void vpet_hline_or(int x, int y, int width);
void vpet_vline(int x, int y, int height);
void vpet_vline_or(int x, int y, int height);
void vpet_rect(int x, int y, int width, int height);
void vpet_rectfill(int x, int y, int width, int height);
void vpet_rectfill_operation(int x, int y, int width, int height, void (*operation)(int, int, uint8_t));

enum character_frame {
	CF_IDLE,
	CF_IDLE2,
	CF_HAPPY,
	CF_JUMP,
	CF_BACK_TURN,
	CF_SAD,
	CF_SAD2,
	CF_ANGRY,
	CF_ANGRY2,
	CF_EYES_CLOSED,
	CF_SLEEP,
	CF_SLEEP2,
	CF_EATING,
	CF_EATING2,
	CF_NO_THANKS,
	CF_SITTING,
	CF_SITTING2,
	CF_RUNNING,
	CF_RUNNING2,
	CF_FACEPLANT,
	CF_SURPRISED,
	CF_SURPRISED_GOOD,
	CF_SURPRISED_BAD,
	CF_DIZZY,
	CF_NAUGHTY,
	CF_BEGGING,
	CF_KISS,
	CF_FISHING,
	CF_BACKPACK_SIDE,
	CF_BACKPACK_BACK,
	CF_ATTACK,
};

enum character_id {
	CHARACTER_MIMI,
	CHARACTER_PYONKO,
	CHARACTER_TERRIER,
	CHARACTER_LOP,
};

void vpet_draw_pet(int x, int y, int hflip, enum character_id character, enum character_frame frame);
void vpet_draw_pet_crop(int x, int y, int hflip, enum character_id character, enum character_frame, int rows);

// ------------------------------------------------------------

#define MAX_NAME_SIZE 12

enum gender_id {
	GENDER_NONE,
	GENDER_OTHER,
	GENDER_GIRL,
	GENDER_BOY,
	GENDER_NONBINARY,
};

enum personality_id {
	PERSONALITY_DEFAULT,
	PERSONALITY_KIND,
	PERSONALITY_ENERGETIC,
	PERSONALITY_LAID_BACK,
	PERSONALITY_COOL,
	PERSONALITY_SILLY,
	PERSONALITY_STUBBORN,
	PERSONALITY_CAUTIOUS,

	PERSONALITY_COUNT
};

#define MAX_STAT 0x7fffffff
// ^ Normal maximum for each stat, but can go above as a bonus

enum pet_stat_id {
	// Care
	STAT_BELLY,
	STAT_HAPPY,
	STAT_CLEAN,
	STAT_HEAVY,

	// Training
	STAT_COOL,
	STAT_BEAUTIFUL,
	STAT_CUTE,
	STAT_CLEVER,
	STAT_TOUGH,

	// Timers until an action
	STAT_POOP,
	STAT_ATTENTION,
	STAT_COMMON_EVENT,
	STAT_UNCOMMON_EVENT,

	STAT_COUNT
};

struct vpet_profile {
	enum character_id   species;
	enum gender_id      gender;
	enum personality_id personality;
	uint32_t            random[4];
	time_t              created_at;
	char                name[MAX_NAME_SIZE+1]; // null terminated
};

struct vpet_status {
	struct vpet_profile profile;
	uint32_t stats[STAT_COUNT];
	uint32_t stat_drop_rate[STAT_COUNT];

    // Number of seconds the vpet has been active
	unsigned int seconds;

	// Like gratitude points in DQB2 but you don't spend them?
	unsigned int gratitude;

	// Status information
	unsigned int pooping_timer;
	unsigned int poops;
};

extern struct vpet_status my_pet;

void add_to_pet_stat(int which_stat, unsigned int value);
void sub_from_pet_stat(int which_stat, unsigned int value);

// ------------------------------------------------------------

enum idle_animation {
	IDLE_ANIM_WANDER, // For STATE_DEFAULT
	IDLE_ANIM_POOP,
};

extern enum idle_animation current_idle_animation;
void vpet_set_idle_animation(enum idle_animation new_idle_animation);

// ------------------------------------------------------------

enum game_state {
	STATE_DEFAULT,

	// Main menu
	STATE_MAIN_MENU,
	STATE_FEED_MENU,
	STATE_PLAY_MENU,
	STATE_CLEAN_MENU,
	STATE_TRAVEL_MENU,
	STATE_RECORDS_MENU,
	STATE_OPTIONS_MENU,

	// Submenu
	STATE_WHICH_FOOD,
	STATE_WHICH_GAME,
	STATE_WHICH_ITEM,

	// Doing stuff
	STATE_STATUS,
	STATE_PAUSED,
	STATE_BRUSHING,
	STATE_BATHING,
	STATE_PETTING,
	STATE_EXPLORE,
	STATE_MINIGAME,

	// Animation states
	STATE_EATING,
	STATE_HAPPY_JUMP,
	STATE_NO_THANKS,
	STATE_ON_TOILET,
};
extern enum game_state vpet_state;
extern int state_variable;

void vpet_refresh_screen();
void vpet_switch_state(enum game_state new_state);
void vpet_draw_pet_animation();
void vpet_draw_pet_animation_and_clear();

// ------------------------------------------------------------

enum KeyCode {
  VPET_KEY_LEFT  = 0x0001,
  VPET_KEY_RIGHT = 0x0002,
  VPET_KEY_UP    = 0x0004,
  VPET_KEY_DOWN  = 0x0008,
  VPET_KEY_A     = 0x0010, // Confirm
  VPET_KEY_B     = 0x0020, // Cancel
  VPET_KEY_C     = 0x0040,

  VPET_KEY_PREV  = 0x0080,
  VPET_KEY_NEXT  = 0x0100,

  VPET_KEY_RESET = 0x8000,
};
#define VPET_KEY_LEFT_PREV    (VPET_KEY_LEFT|VPET_KEY_PREV)
#define VPET_KEY_RIGHT_NEXT   (VPET_KEY_RIGHT|VPET_KEY_NEXT)
#define VPET_KEY_UP_PREV      (VPET_KEY_UP|VPET_KEY_PREV)
#define VPET_KEY_DOWN_NEXT    (VPET_KEY_DOWN|VPET_KEY_NEXT)
#define VPET_KEY_LEFT_RIGHT_PREV_NEXT (VPET_KEY_LEFT|VPET_KEY_RIGHT|VPET_KEY_PREV|VPET_KEY_NEXT)
#define VPET_KEY_UP_DOWN_PREV_NEXT (VPET_KEY_UP|VPET_KEY_DOWN|VPET_KEY_PREV|VPET_KEY_NEXT)
#define VPET_KEY_ANY_DIRECTION (VPET_KEY_LEFT|VPET_KEY_RIGHT|VPET_KEY_UP|VPET_KEY_DOWN|VPET_KEY_PREV|VPET_KEY_NEXT)
extern uint16_t key_down, key_new, key_last, key_new_or_repeat;

// ------------------------------------------------------------

enum entity_state {
	E_STATE_NORMAL,
	E_STATE_PAUSE,
	E_STATE_STUNNED,
	E_STATE_ACTIVE,
	E_STATE_INIT,
};

enum entity_draw_flags {
	ENTITY_DRAW_SIZE     = 0x03, // ......ss
	  ENTITY_DRAW_8      = 0x00,
	  ENTITY_DRAW_16     = 0x01,
	  ENTITY_DRAW_24     = 0x02,
	  ENTITY_DRAW_CUSTOM = 0x03,

	ENTITY_DRAW_COLOR    = 0x0c, // ....cc..
	  ENTITY_DRAW_XOR    = 0x00,
	  ENTITY_DRAW_BLACK  = 0x04, // OR
	  ENTITY_DRAW_WHITE  = 0x08, // AND NOT
	  ENTITY_DRAW_BOTH   = 0x0c,

	ENTITY_DRAW_ABOVE    = 0x10, // ...a.... - Above the pet
	ENTITY_NO_CENTERING  = 0x20, // ..n.....
};

struct entity {
	int type;
	enum entity_state state;

	int x;
	int y;
	int hspeed;
	int vspeed;

	int var[4]; // generic variables
	int timer;

	// ----------
	unsigned char collide_width;
	unsigned char collide_height;
	unsigned char hflip;
	unsigned char art_height; // Number of rows to show
	unsigned char draw_flags;  // Bits from entity_draw_flags
	union {
		const uint8_t *art_8;
		const uint16_t *art_16;
		const uint32_t *art_24;
		void (*custom)(int);
	} frame;
};
#define ENTITY_LEN 32
#define COORD_SCALE 256
extern struct entity entities[ENTITY_LEN];

// ------------------------------------------------------------

#ifdef PLATFORM_PC
extern int ScreenWidth, ScreenHeight, ScreenZoom;
extern SDL_Window *window;
extern SDL_Renderer *ScreenRenderer;
extern int framecounter;

void SDL_MessageBox(int Type, const char *Title, SDL_Window *Window, const char *fmt, ...);
void strlcpy(char *Destination, const char *Source, int MaxLength);
SDL_Surface *SDL_LoadImage(const char *FileName, int Flags);
SDL_Texture *LoadTexture(const char *FileName, int Flags);
void rectfill(SDL_Renderer *Bmp, int X1, int Y1, int Width, int Height);
void rect(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2);
void sblit(SDL_Surface* SrcBmp, SDL_Surface* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blit(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height);
void blitf(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, SDL_RendererFlip Flip);
void blitz(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Width2, int Height2);
void blitfull(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int DestX, int DestY);
#endif

void RandomSeed();
uint32_t RandomRaw();
uint32_t Random(uint32_t Bound);
uint32_t RandomMinMax(uint32_t Min, uint32_t Max);

// ------------------------------------------------------------

enum food_id {
	FOOD_BREAD,
	FOOD_CEREAL,
	FOOD_SCONE,
	FOOD_SUSHI,
	FOOD_APPLE,
	FOOD_CONE,
	FOOD_PUDDING,
	FOOD_TART,
	FOOD_APPLE_PIE,
	FOOD_BANANAS,
	FOOD_BEEF_BOWL,
	FOOD_CAKE,
	FOOD_CHEESE,
	FOOD_CHEESECAKE,
	FOOD_CHERRIES,
	FOOD_CHICKEN,
	FOOD_CHOCOLATE,
	FOOD_COOKIE,
	FOOD_CORN,
	FOOD_CORN_DOG,
	FOOD_CREAM_CAKE,
	FOOD_CUPCAKE,
	FOOD_DONUT,
	FOOD_FISH,
	FOOD_FRENCH_FRIES,
	FOOD_GRAPES,
	FOOD_HAMBURGER,
	FOOD_HOT_DOG,
	FOOD_ICE_CREAM,
	FOOD_KEBAB,
	FOOD_MELON,
	FOOD_NOODLES,
	FOOD_OMELETTE,
	FOOD_PARFAIT,
	FOOD_PASTA,
	FOOD_PEAR,
	FOOD_PINEAPPLE,
	FOOD_PIZZA,
	FOOD_POPCORN,
	FOOD_SANDWICH,
	FOOD_SAUSAGE,
	FOOD_STEAK,
	FOOD_SUNDAE,
	FOOD_TACO,
	FOOD_TURKEY,
	FOOD_ENERGY_DRINK,
	FOOD_FRUIT_JUICE,
	FOOD_MILK,
	FOOD_SODA,
	FOOD_ICE_CREAM_2,
	FOOD_BREAD_SLICE,
	FOOD_CRAB,
	FOOD_CRACKER,
	FOOD_EGGS,
	FOOD_FISH_2,
	FOOD_PIZZA_2,
	FOOD_SHRIMP,
	FOOD_FRIED_CHICKEN,
	FOOD_MEAT,
	FOOD_PEANUT,
	FOOD_POTATO,
	FOOD_FLOWER_DRINK,
	FOOD_COFFEE,
	FOOD_LOLLIPOP,
	FOOD_WATERMELON,
	FOOD_FISH_BURGER,
	FOOD_TEA,
	FOOD_PRETZEL,
	FOOD_ICE_POP,
	FOOD_SONIC_POP,
	FOOD_TIDE_POD,
	FOOD_MC_FRIES,
	FOOD_WORLD_FRIES,
	FOOD_MUNF_MUNF,
	FOOD_DB_BURGER,

	FOOD_COUNT
};
void vpet_draw_food(int x, int y, enum food_id food, int frame);

struct food_info {
	uint32_t add_belly;
	uint32_t add_happy;
	uint32_t add_heavy;
	int32_t messiness;
	uint32_t flags;
	char name[12+1];
};

enum food_flags {
	FOOD_IS_MEAL  = 0x0001,
	FOOD_IS_SNACK = 0x0002,
	FOOD_DONT_EAT_PLATE = 0x0004,

	FOOD_CATEGORY_MASK    = 0x0f00,
	FOOD_CATEGORY_GRAINS  = 0x0100,
	FOOD_CATEGORY_PROTEIN = 0x0200,
	FOOD_CATEGORY_PRODUCE = 0x0300,
	FOOD_CATEGORY_COOKED  = 0x0400,
	FOOD_CATEGORY_DESSERT = 0x0500,
	FOOD_CATEGORY_FROZEN  = 0x0600,
	FOOD_CATEGORY_DRINK   = 0x0700,
};
extern const struct food_info food_infos[];
#endif
