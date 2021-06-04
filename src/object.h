#ifndef OBJECT_H
#define OBJECT_H
#include <vector>
using namespace std;

#include "sprite.h"
#include "area.h"
enum { DIRE_UP, DIRE_DOWN, DIRE_LEFT, DIRE_RIGHT };
enum { WALK_DOWN1, FACE_DOWN, WALK_DOWN3, WALK_UP1, FACE_UP, WALK_UP3, 
       WALK_SIDE1, FACE_SIDE, WALK_SIDE3 };
#define FLIP_LEFT   false
#define FLIP_RIGHT  true


class ObjectClass {
public:
    ObjectClass() { SpriteNum = -1; Flipped = false; ZLevel = 0; }
    void SetSprite(int Number) { SpriteNum = Number; }
    int GetSprite() { return SpriteNum; }
    void SetLoc(float X_, float Y_) { X = X_; Y = Y_; }
    void SetVelocity(int X_, int Y_) { VecX = X_; VecY = Y_; }
    void SetVecX(int X_) { VecX = X_; }
    void SetVecY(int Y_) { VecY = Y_; }
    void AddVecX(int X_) { VecX += X_; }
    void AddVecY(int Y_) { VecY += Y_; }
    int GetVecX() { return VecX; }
    int GetVecY() { return VecY; }
    float GetX() { return X; }
    float GetY() { return Y; }
    int GetZ() { return ZLevel; }
    void SetZ(int Z) { ZLevel = Z; }
    void AddX(float X_) { X += X_; }
    void AddY(float Y_) { Y += Y_; }
    void AddOldX(float X_) { OldX += X_; }
    void AddOldY(float Y_) { OldY += Y_; }
    float GetOldX() { return OldX; }
    float GetOldY() { return OldY; }
    void SplitFrames(int Width, int Height);
    void SetFrame(int FrameNum);
    void SetFlip(bool SetFlip);
    void Flip();
    bool IsFlipped() { return Flipped; }
    bool IsMoving() { return Moving; }
    int GetDire() { return Direction; }
protected:
    float VecX, VecY;
    float X, Y;
    float OldX, OldY, OldAreaX, OldAreaY;
    int Direction;
    int ZLevel;
    int SpriteNum;
    vector <SDL_Rect> Frame;
    bool Flipped, Moving;
    int AnimFrame;
};

#endif
