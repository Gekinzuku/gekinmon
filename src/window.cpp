#include "window.h"
#include "sprite.h"
#include "player.h"

SDLWindow Window;
SDLFonts Font;

bool SDLWindow::Init(int Width, int Height, int BPP, string Title)
{
	ScreenWidth = Width;
	ScreenHeight = Height;
	// Let's start up SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)  return false;
    // Set the Video mode
	Screen = SDL_SetVideoMode(Width, Height, BPP, SDL_HWSURFACE | SDL_DOUBLEBUF);
	BlitMe = SDL_DisplayFormat(Screen);
	if (Screen == NULL) return false; // Return if the display isn't set up
	if (TTF_Init() == -1) return false; // Init the font system
	SDL_WM_SetCaption(Title.c_str(), NULL); // We need a window title too
	
	if (Font.Load("Data/fonts/pf_tempesta_seven_compressed.ttf", 17) == -1)
        return false;

	return true;
}

int SDLWindow::LoadSprite(string FileName)
{
	SDL_Surface * tmpImage = NULL;
	SDL_Surface * OptImage = NULL;
	tmpImage = IMG_Load(FileName.c_str());

	if (tmpImage != NULL) {
		OptImage = SDL_DisplayFormat(tmpImage);
		SDL_FreeSurface(tmpImage);

		int SpriteNum = 0;
		for (;;++SpriteNum) {
			if (Sprite.count(SpriteNum) == 0) break;	
		}
		Sprite[SpriteNum] = SDLSprite(OptImage);
		return SpriteNum;
	}

	return -1;
}

int SDLWindow::LoadSprite(string FileName, int R, int G, int B)
{
	SDL_Surface * tmpImage = NULL;
	SDL_Surface * OptImage = NULL;
	tmpImage = IMG_Load(FileName.c_str());

	if (tmpImage != NULL) {
		OptImage = SDL_DisplayFormat(tmpImage);
		SDL_FreeSurface(tmpImage);

		if (OptImage != NULL) {
			Uint32 Key = SDL_MapRGB(OptImage->format, R, G, B);
			SDL_SetColorKey(OptImage, SDL_SRCCOLORKEY, Key);
			int SpriteNum = 0;
			for (;;++SpriteNum) {
				if (Sprite.count(SpriteNum) == 0) break;	
			}
			Sprite[SpriteNum] = SDLSprite(OptImage);
			return SpriteNum;
		}
	}

	return -1;
}

void SDLWindow::Render()
{
	SDL_FillRect(BlitMe, &BlitMe->clip_rect, 
                 SDL_MapRGB(Screen->format, 0x00, 0x00, 0x00));
	for (int i = SpriteOrder.size()-1; i >= 0; --i) {
		Sprite[SpriteOrder[i]].Render(BlitMe);
	}
//	SDL_Rect play = {Player.GetX(),Player.GetY(),32,64};	
//	SDL_FillRect(BlitMe, &play, SDL_MapRGB(Screen->format, 0xFF, 0xFF, 0x00));
	SDL_Rect dr = {0,0, ScreenWidth,ScreenHeight};	
	SDL_Rect sr = {0,0, VIEW_WIDTH, VIEW_HEIGHT};
	// Display the screen, First we stretch it though
	SDL_SoftStretch(BlitMe, &sr, Screen, &dr); 
	if (SDL_Flip(Screen) == -1) return;
}


int SDLFonts::Load(const char * FontName, int Size)
{
	Font.push_back(TTF_OpenFont(FontName, Size));
	if (Font[Font.size()-1] == NULL) { cout << "Failed font.\n";
		TTF_CloseFont(Font[Font.size()-1]);
		Font.pop_back();
		return -1; 
	}
	return (Font.size()-1);
}

int SDLFonts::CreateText(unsigned int Num, const char * Message, 
                         Uint8 R, Uint8 G, Uint8 B)
{
	SDL_Color TextColor = { R, G, B };
	SDL_Surface * message = TTF_RenderText_Solid(Font[Num], Message, TextColor);
	int SpriteNum = 0;
	for (;;++SpriteNum) {
		if (Sprite.count(SpriteNum) == 0) break;	
	}
	Sprite[SpriteNum] = SDLSprite(message);
	return SpriteNum;
}

int SDLFonts::CreateText(unsigned int Num, const char * Message, 
                         SDL_Color Color)
{
	SDL_Surface * message = TTF_RenderText_Solid(Font[Num], Message, Color);
	int SpriteNum = 0;
	for (;;++SpriteNum) {
		if (Sprite.count(SpriteNum) == 0) break;	
	}
	Sprite[SpriteNum] = SDLSprite(message);
	return SpriteNum;
}

