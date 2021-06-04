#include "sprite.h"

map <int,SDLSprite> Sprite;
vector <int> SpriteOrder;

void SDLSprite::Render(SDL_Surface * Screen)
{
    if (!Display) return;

    SDL_Rect Offset;
    Offset.x = X;
    Offset.y = Y;

    SDL_BlitSurface(Surface, &Clip, Screen, &Offset);
}

// Flips the entire surface
// Possible todo: Make a function to only flip a single row?
void SDLSprite::FlipSurface()
{
    Uint32 * Pixels = (Uint32 *) Surface->pixels;
    Uint32 * Backup = new Uint32[Surface->w*Surface->h];
    memcpy(Backup, Pixels, sizeof(Uint32[Surface->w*Surface->h]));

    for (int y = 0; y < Surface->h; ++y) {
        for (int x = 0; x < Surface->w; ++x) {
            Pixels[y*Surface->w+x] = Backup[(y*Surface->w)+(Surface->w-x-1)];
        }
    }
    delete[] Backup;
}

void DeleteSprite(int SpriteNum)
{
    if (Sprite.count(SpriteNum) == 0) return;
    Sprite[SpriteNum].CleanUp();
    Sprite.erase(Sprite.find(SpriteNum));
    RemoveSpriteFromOrder(SpriteNum);
}


/** The lower the number the sooner it'll get rendered. 
  * 0 = first, SpriteOrder.size()-1 = last.
  * -1 is the end of the list to render.
  * Pushes back any sprites that may be after the number set for this sprite.
  * If the given Order is past the end of the vector, it'll go to the end. */
void SetSpriteOrder(int SpriteNumber, int Order)
{
    if (SpriteOrder.size() == 0 || Order < 0 || 
        Order > int(SpriteOrder.size()-1)) 
        SpriteOrder.push_back(SpriteNumber);
    else {
        SpriteOrder.push_back(SpriteOrder[SpriteOrder.size()-1]);
        for (int i = SpriteOrder.size()-2; i > Order; --i) {
            SpriteOrder[i] = SpriteOrder[i-1];
        }
        SpriteOrder[Order] = SpriteNumber;
    }
}

void RemoveSpriteFromOrder(int SpriteNumber)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNumber)
            SpriteOrder.erase(SpriteOrder.begin()+i);
    }
}

void SwapSpriteOrder(int SpriteNum1, int SpriteNum2)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNum1)
            SpriteOrder[i] = SpriteNum2;
        else if (SpriteOrder[i] == SpriteNum2)
            SpriteOrder[i] = SpriteNum1;
    }
}

/** Put SpriteNum1 in front of SpriteNum2,
  * Note: Will not swap if the current spot SpriteNum1 is in is "higher" 
  * than the spot it would get swapped to */
void SwapSpriteInFrontOf(int SpriteNum1, int SpriteNum2)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNum2) {
            if (i >= GetSpriteOrder(SpriteNum1)) return;
            // Push all the other sprites back one and drop this in its place
            RemoveSpriteFromOrder(SpriteNum1);
            SpriteOrder.push_back(SpriteOrder[SpriteOrder.size()-1]);
            for (int j = SpriteOrder.size()-2; j > i; --j) {
                SpriteOrder[j] = SpriteOrder[j-1];
            }
            SpriteOrder[i] = SpriteNum1;
            return;
        }
    }
}

/** Same as the above, only it drops SpriteNum1 in front of SpriteNum2 
  * regardless of its current position */
void ForceSwapSpriteInFrontOf(int SpriteNum1, int SpriteNum2)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNum2) {
            // Push all the other sprites back one 
            // and drop this in its place
            if (i >= GetSpriteOrder(SpriteNum1)) --i;
            RemoveSpriteFromOrder(SpriteNum1);
            SpriteOrder.push_back(SpriteOrder[SpriteOrder.size()-1]);
            for (int j = SpriteOrder.size()-2; j > i; --j) {
                SpriteOrder[j] = SpriteOrder[j-1];
            }
            SpriteOrder[i] = SpriteNum1;
            return;
        }
    }
}

/** Drops SpriteNum1 behind SpriteNum2 only if it isn't already further back */
void SwapSpriteBehind(int SpriteNum1, int SpriteNum2)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNum2) {
            if (i <= GetSpriteOrder(SpriteNum1)) return;
            // Push all the other sprites back one and drop this in its place
            RemoveSpriteFromOrder(SpriteNum1);
            SpriteOrder.push_back(SpriteOrder[SpriteOrder.size()-1]);
            for (int j = SpriteOrder.size()-2; j > i; --j) {
                SpriteOrder[j] = SpriteOrder[j-1];
            }
            SpriteOrder[i] = SpriteNum1;
            return;
        }
    }
}

/** Drops SpriteNum1 behind SpriteNum2 regardless of its current position */
void ForceSwapSpriteBehind(int SpriteNum1, int SpriteNum2)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNum2) {
            // Push all the other sprites back one and drop this in its place
            RemoveSpriteFromOrder(SpriteNum1);
            SpriteOrder.push_back(SpriteOrder[SpriteOrder.size()-1]);
            for (int j = SpriteOrder.size()-2; j > i; --j) {
                SpriteOrder[j] = SpriteOrder[j-1];
            }
            SpriteOrder[i] = SpriteNum1;
            return;
        }
    }
}


int GetSpriteOrder(int SpriteNumber)
{
    for (unsigned int i = 0; i < SpriteOrder.size(); ++i) {
        if (SpriteOrder[i] == SpriteNumber) return i;
    }
}

