#ifndef PLATFORMER_HEADER
#define PLATFORMER_HEADER
#define NO_STDIO_REDIRECT
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dirent.h>
#include <stdarg.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

// ------------------------------------------------------------

// Pet configuration
#define PET_SCREEN_W 48
#define PET_SCREEN_H 32
#define PET_SCREEN_CENTER_X 24
#define PET_SCREEN_CENTER_Y 16

// Full animation
#define PET_ANIMATION_FULL
// Animation for LCD devices
#define PET_ANIMATION_PLUS
// Minimal animation
#define PET_ANIMATION_EPAPER

// ------------------------------------------------------------

void vpet_clear_screen();
void vpet_set_pixel(int x, int y, int value);
int  vpet_get_pixel(int x, int y);
void vpet_render_screen();
void vpet_or_8_pixels(int x, int y, uint8_t value);
void vpet_xor_8_pixels(int x, int y, uint8_t value);
void vpet_and_8_pixels(int x, int y, uint8_t value);
void vpet_andnot_8_pixels(int x, int y, uint8_t value);
void vpet_sprite_8(int x, int y, int hflip, int h, const uint8_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_16(int x, int y, int hflip, int h, const uint16_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_24(int x, int y, int hflip, int h, const uint32_t rows[], void (*operation)(int, int, uint8_t));
void vpet_sprite_overwrite_8(int x, int y, int hflip, int h, const uint8_t rows[]);
void vpet_sprite_overwrite_16(int x, int y, int hflip, int h, const uint16_t rows[]);
void vpet_sprite_overwrite_24(int x, int y, int hflip, int h, const uint32_t rows[]);
void vpet_sprite_mask_8(int x, int y, int hflip, int h, const uint8_t rows[], const uint8_t mask_rows[]);
void vpet_sprite_mask_16(int x, int y, int hflip, int h, const uint16_t rows[], const uint16_t mask_rows[]);
void vpet_sprite_mask_24(int x, int y, int hflip, int h, const uint32_t rows[], const uint32_t mask_rows[]);
#define vpet_sprite_xor_8(x, y, hflip, h, rows) vpet_sprite_8(x, y, hflip, h, rows, vpet_xor_8_pixels)
#define vpet_sprite_xor_16(x, y, hflip, h, rows) vpet_sprite_16(x, y, hflip, h, rows, vpet_xor_8_pixels)
#define vpet_sprite_xor_24(x, y, hflip, h, rows) vpet_sprite_24(x, y, hflip, h, rows, vpet_xor_8_pixels)
extern const uint8_t font_4x6[];
void vpet_set_font(const uint8_t font[], int width, int height);
void vpet_draw_textf(int x, int y, const char *fmt, ...);
void vpet_draw_text(int x, int y, const char *buffer);
void vpet_hline(int x, int y, int width);
void vpet_vline(int x, int y, int height);
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
	CF_EATING1,
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
};

extern struct vpet_status my_pet;

void vpet_tick();

// ------------------------------------------------------------

enum game_state {
	STATE_DEFAULT,
	STATE_MAIN_MENU,
	STATE_STATUS,
	STATE_FEED_MENU,
	STATE_PLAY_MENU,
	STATE_CLEAN_MENU,
	STATE_TRAVEL_MENU,
	STATE_RECORDS_MENU,
	STATE_OPTIONS_MENU,
	STATE_PAUSED,
};
extern enum game_state vpet_state;
void vpet_refresh_screen();
void vpet_switch_state(enum game_state new_state);
void vpet_draw_pet_animation();

// ------------------------------------------------------------

enum KeyCode {
  KEY_LEFT  = 0x0001,
  KEY_RIGHT = 0x0002,
  KEY_UP    = 0x0004,
  KEY_DOWN  = 0x0008,
  KEY_A     = 0x0010,
  KEY_B     = 0x0020,
  KEY_RESET = 0x0040,
};
extern uint16_t key_down, key_new, key_last, key_new_or_repeat;

// ------------------------------------------------------------

enum entity_state {
	STATE_NORMAL,
	STATE_PAUSE,
	STATE_STUNNED,
	STATE_ACTIVE,
	STATE_INIT,
};

struct entity {
	int type;
	enum entity_state state;

	int xpos;
	int ypos;

	int xspeed;
	int yspeed;

	int var[4]; // generic variables
	int timer;
};
#define ENTITY_LEN 64
extern struct entity entities[ENTITY_LEN];

// ------------------------------------------------------------

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

void RandomSeed();
uint32_t RandomRaw();
uint32_t Random(uint32_t Bound);
uint32_t RandomMinMax(uint32_t Min, uint32_t Max);
