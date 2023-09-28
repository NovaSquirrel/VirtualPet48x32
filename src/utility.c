#include "vpet.h"

#ifdef PLATFORM_PC
void strlcpy(char *Destination, const char *Source, int MaxLength) {
	// MaxLength is directly from sizeof() so it includes the zero
	int SourceLen = strlen(Source);
	if((SourceLen+1) < MaxLength)
		MaxLength = SourceLen + 1;
	memcpy(Destination, Source, MaxLength-1);
	Destination[MaxLength-1] = 0;
}

void SDL_MessageBox(int Type, const char *Title, SDL_Window *Window, const char *fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	char Buffer[512];
	vsprintf(Buffer, fmt, argp);
	SDL_ShowSimpleMessageBox(Type, Title, Buffer, Window);
	va_end(argp);
}

SDL_Surface *SDL_LoadImage(const char *FileName, int Flags) {
	SDL_Surface* loadedSurface = IMG_Load(FileName);
	if(loadedSurface == NULL) {
		SDL_MessageBox(SDL_MESSAGEBOX_ERROR, "Error", window, "Unable to load image %s! SDL Error: %s", FileName, SDL_GetError());
		return NULL;
	}
	if(Flags & 1)
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 0, 255));
	return loadedSurface;
}

SDL_Texture *LoadTexture(const char *FileName, int Flags) {
	SDL_Surface *Surface = SDL_LoadImage(FileName, Flags);
	if(!Surface) return NULL;
	SDL_Texture *Texture = SDL_CreateTextureFromSurface(ScreenRenderer, Surface);
	SDL_FreeSurface(Surface);
	return Texture;
}

// drawing functions
void rectfill(SDL_Renderer *Bmp, int X1, int Y1, int Width, int Height) {
	SDL_Rect Temp = {X1*ScreenZoom, Y1*ScreenZoom, Width*ScreenZoom, Height*ScreenZoom};
	SDL_RenderFillRect(Bmp,	&Temp);
}

void rect(SDL_Renderer *Bmp, int X1, int Y1, int X2, int Y2) {
	SDL_Rect Temp = {X1, Y1, abs(X2-X1)+1, abs(Y2-Y1)+1};
	SDL_RenderDrawRect(Bmp, &Temp);
}

void sblit(SDL_Surface* SrcBmp, SDL_Surface* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height) {
	SDL_Rect Src = {SourceX, SourceY, Width, Height};
	SDL_Rect Dst = {DestX, DestY};
	SDL_BlitSurface(SrcBmp, &Src, DstBmp, &Dst);
}

void blit(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height) {
	SDL_Rect Src = {SourceX*ScreenZoom, SourceY*ScreenZoom, Width*ScreenZoom, Height*ScreenZoom};
	SDL_Rect Dst = {DestX*ScreenZoom, DestY*ScreenZoom, Width*ScreenZoom, Height*ScreenZoom};
	SDL_RenderCopy(DstBmp,	SrcBmp, &Src, &Dst);
}

void blitf(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, SDL_RendererFlip Flip) {
	SDL_Rect Src = {SourceX*ScreenZoom, SourceY*ScreenZoom, Width*ScreenZoom, Height*ScreenZoom};
	SDL_Rect Dst = {DestX*ScreenZoom, DestY*ScreenZoom, Width*ScreenZoom, Height*ScreenZoom};
	SDL_RenderCopyEx(DstBmp,	SrcBmp, &Src, &Dst, 0, NULL, Flip);
}

void blitz(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int SourceX, int SourceY, int DestX, int DestY, int Width, int Height, int Width2, int Height2) {
	SDL_Rect Src = {SourceX, SourceY, Width, Height};
	SDL_Rect Dst = {DestX, DestY, Width2, Height2};
	SDL_RenderCopy(DstBmp,	SrcBmp, &Src, &Dst);
}

void blitfull(SDL_Texture* SrcBmp, SDL_Renderer* DstBmp, int DestX, int DestY) {
	SDL_Rect Dst = {DestX, DestY};
	SDL_RenderCopy(DstBmp,	SrcBmp, NULL, &Dst);
}
#endif
