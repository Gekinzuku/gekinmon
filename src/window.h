#ifndef WINDOW_H
#define WINDOW_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define VIEW_WIDTH	512
#define VIEW_HEIGHT	384	

// Window class
class SDLWindow {
public:
	SDLWindow() { BlitMe = Screen = NULL; }
	bool Init(int Width, int Height, int BPP, string Title);
	int LoadSprite(string FileName); // Simply load an image
    // Load an image with transparency
	int LoadSprite(string FileName, int R, int G, int B); 
	void Render();

	int GetWidth() { return ScreenWidth; }
	int GetHeight() { return ScreenHeight; }
    int GetViewWidth() { return VIEW_WIDTH; }
    int GetViewHeight() { return VIEW_HEIGHT; }
private:
	int ScreenWidth, ScreenHeight;
	SDL_Surface * Screen; // Our actual display
	SDL_Surface * BlitMe; // Blit to this, then stretch to the actual screen.
};

class SDLFonts {
public:
	int Load(const char * FontName, int Size);
	unsigned int Count() { return Font.size(); }
	void Delete(unsigned int Num) 
     { TTF_CloseFont(Font[Num]); Font.erase(Font.begin()+Num); }

	TTF_Font * GetFont(unsigned Num) { return Font[Num]; }

	int CreateText(unsigned int Num, const char * Message,
                   Uint8 R, Uint8 G, Uint8 B);

	int CreateText(unsigned int Num, const char * Message, SDL_Color Color);
private:
	vector <TTF_Font *> Font;
};

extern SDLFonts Font;
extern SDLWindow Window;
#endif

