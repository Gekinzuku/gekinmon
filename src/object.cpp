#include "object.h"

void ObjectClass::SplitFrames(int Width, int Height)
{
    int W = Sprite[SpriteNum].GetWidth(), H = Sprite[SpriteNum].GetHeight();
    W = W - (W%Width);
    H = H - (H%Height);
    Frame.clear();
    SDL_Rect Tmp;
    for (int OffY = 0; OffY < H; OffY += Height) {
        for (int OffX = 0; OffX < W; OffX += Width) { 
//          cout << "Frame: " << OffX << ", " << OffY << " - " 
//               << (OffX+Width) << ", " << (OffY+Height) << endl;
            Tmp.x = OffX;
            Tmp.y = OffY;
            Tmp.w = Width;
            Tmp.h = Height;
            Frame.push_back(Tmp);
        }
    }
}

void ObjectClass::SetFrame(int FrameNum)
{
    Sprite[SpriteNum].SetClip(Frame[FrameNum]);
}

void ObjectClass::SetFlip(bool SetFlip)
{
    if (SetFlip != Flipped) {
        Sprite[SpriteNum].FlipSurface();
        Flipped = SetFlip;
    }
}

void ObjectClass::Flip()
{
    Sprite[SpriteNum].FlipSurface();
    Flipped = !Flipped;
}

