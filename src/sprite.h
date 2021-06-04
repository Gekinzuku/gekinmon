#ifndef SPRITE_H
#define SPRITE_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
class SDLSprite
{
public:
    SDLSprite() { Surface = NULL; Display = false; X = Y = 0; }
    SDLSprite(SDL_Surface * Src) 
        { Surface = Src; Display = true; X = Y = 0; Clip = Surface->clip_rect; }

    SDL_Surface * GetSurface() { return Surface; }
    void CleanUp() { SDL_FreeSurface(Surface); }
    void Render(SDL_Surface * Screen);
    void SetX(float X_) { X = int(X_+0.5); }
    void SetY(float Y_) { Y = int(Y_+0.5); }
    void SetLoc(float X_, float Y_) { X = int(X_+0.5); Y = int(Y_+0.5); }
    void SetClip(int X_, int Y_, int W_, int H_) 
        { Clip.x = X_; Clip.y = Y_; Clip.w = W_; Clip.h = H_; }

    void SetClip(SDL_Rect Frame) { Clip = Frame; }
    int GetWidth() { return Surface->w; }
    int GetHeight() { return Surface->h; }
    void SetDisplay(bool Display_) { Display = Display_; }
    bool GetDisplay() { return Display; }
    void FlipSurface();
    unsigned short GetBPP() { return Surface->format->BitsPerPixel; }
private:
    SDL_Surface * Surface;
    bool Display;
    int X, Y;
    SDL_Rect Clip;
};

void DeleteSprite(int SpriteNum);

extern map <int,SDLSprite> Sprite;

// We use this to determine the order of rendering sprites
extern vector <int> SpriteOrder; 
void SetSpriteOrder(int SpriteNumber, int Order);
void RemoveSpriteFromOrder(int SpriteNumber);
void SwapSpriteOrder(int SpriteNum1, int SpriteNum2);
void SwapSpriteInFrontOf(int SpriteNum1, int SpriteNum2);
void SwapSpriteBehind(int SpriteNum1, int SpriteNum2);
void ForceSwapSpriteInFrontOf(int SpriteNum1, int SpriteNum2);
void ForceSwapSpriteBehind(int SpriteNum1, int SpriteNum2);
int GetSpriteOrder(int SpriteNumber);
#endif
