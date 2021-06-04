#ifndef AREA_H
#define AREA_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class AreaClass {
public:
    AreaClass() { Tiles = NULL; Background = -1; }
    int Load(const char * FileName, const char * Entrance);
    int CheckCollision(int Ox, int Oy, int Ow, int Oh);
    void Update();
    void MoveX(float X) { ScrollX += X; }
    void MoveY(float Y) { ScrollY += Y; }
    float GetX() { return ScrollX; }
    float GetY() { return ScrollY; }
    int GetWidth() { return Width; }
    int GetHeight() { return Height; }
    void SetX(float X) { ScrollX = X; }
    void SetY(float Y) { ScrollY = Y; }
    void SetZLevel(int SpriteNum, int Level);
    string GetName() { return AreaName; }
private:
    string AreaName;
    int * Tiles;
    int Background;
    vector <int> Foreground;
    int Width, Height;
    float ScrollX, ScrollY;
};

extern AreaClass Area;
#endif
