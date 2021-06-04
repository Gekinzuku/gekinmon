#include "player.h"
#include "npc.h"
#include "area.h"
#include "window.h"
#include "embed.h"

PlayerClass Player; 

void PlayerClass::Move(Uint32 DeltaTicks) 
{
    if (Moving) {
        if (Direction == DIRE_UP) {
            if (Y <= (VIEW_HEIGHT/2)+8 && Area.GetY() < 0) {
                Area.MoveY(VecY * (DeltaTicks/1000.f));
                if ((Area.GetY() - OldY + Y) > 8) {
                    if (AnimFrame == 0) SetFrame(WALK_UP1);
                    else SetFrame(WALK_UP3);
                }
                MoveAllNPCsY(VecY * (DeltaTicks/1000.f));
                if (Area.GetY() >= OldY-Y + 16) {
                    Moving = false;
                    float ChangeY = Area.GetY() - ((OldY-Y)+16);
                    Area.MoveY(-ChangeY);
                    MoveAllNPCsY(-ChangeY);
                    SetFrame(FACE_UP);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                    SwapPlayerOrder();
                }
            }
            else {
                Y -= VecY * (DeltaTicks/1000.f);
                if (-(Area.GetY() + Y - OldY) > 8) {
                    if (AnimFrame == 0) SetFrame(WALK_UP3);
                    else SetFrame(WALK_UP1);
                }
                if (Area.GetY() + Y <= OldY - 16) {
                    Moving = false;
                    Y = OldY-Area.GetY() - 16;
                    SetFrame(FACE_UP);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                    SwapPlayerOrder();
                }
            }
        }
        if (Direction == DIRE_DOWN) {
            if (Y >= (VIEW_HEIGHT/2)+8
                && Area.GetHeight()+Area.GetY() > VIEW_HEIGHT) {
                Area.MoveY(-VecY * (DeltaTicks/1000.f));
                MoveAllNPCsY(-VecY * (DeltaTicks/1000.f));
                if (-(Area.GetY() - OldY + Y) > 8) {
                     if (AnimFrame == 0) SetFrame(WALK_DOWN3);
                     else SetFrame(WALK_DOWN1);
                }
                if (Area.GetY() <= OldY-Y - 16) {
                    Moving = false;
                    float ChangeY = Area.GetY() - ((OldY-Y)-16);
                    Area.MoveY(-ChangeY);
                    MoveAllNPCsY(-ChangeY);
                    SetFrame(FACE_DOWN);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                    SwapPlayerOrder();
                }
            }
            else {
                Y += VecY * (DeltaTicks/1000.f);
                if ((Area.GetY() + Y - OldY) > 8) {
                     if (AnimFrame == 0) SetFrame(WALK_DOWN3);
                     else SetFrame(WALK_DOWN1);
                }
                if (Area.GetY() + Y >= OldY + 16) {
                    Moving = false;
                    Y = OldY-Area.GetY() + 16;
                    SetFrame(FACE_DOWN);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                    SwapPlayerOrder();
                }
            }
        }
        if (Direction == DIRE_LEFT) {
            if (X <= (VIEW_WIDTH/2) && Area.GetX() < 0) {
                Area.MoveX(VecX *(DeltaTicks/1000.f));
                MoveAllNPCsX(VecX *(DeltaTicks/1000.f));
                if ((Area.GetX() + X - OldX) > 8) {
                    if (AnimFrame == 0) SetFrame(WALK_SIDE3);
                    else SetFrame(WALK_SIDE1);
                }
                if (Area.GetX() >= OldX-X + 16) {
                    Moving = false;
                    float ChangeX = Area.GetX() - ((OldX-X)+16);
                    Area.MoveX(-ChangeX);
                    MoveAllNPCsX(-ChangeX);
                    SetFrame(FACE_SIDE);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                }
            }
            else {
                X -= VecX * (DeltaTicks/1000.f);
                if (-(Area.GetX() + X - OldX) > 8) {
                     if (AnimFrame == 0) SetFrame(WALK_SIDE3);
                     else SetFrame(WALK_SIDE1);
                }
                                if (Area.GetX() + X <= OldX - 16) {
                                        Moving = false;
                                        X = OldX-Area.GetX() - 16;
                    SetFrame(FACE_SIDE);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                                }
                        }
        }
        if (Direction == DIRE_RIGHT) {
            if (X >= (VIEW_WIDTH/2)
                && Area.GetWidth()+Area.GetX() > VIEW_WIDTH) {
                Area.MoveX(-VecX *(DeltaTicks/1000.f)); 
                MoveAllNPCsX(-VecX *(DeltaTicks/1000.f));
                if (-(Area.GetX() + X - OldX) > 8) {
                     if (AnimFrame == 0) SetFrame(WALK_SIDE3);
                     else SetFrame(WALK_SIDE1);
                }
                if (Area.GetX() <= OldX-X - 16) {
                    Moving = false;
                    float ChangeX = Area.GetX() - ((OldX-X)-16);
                    Area.MoveX(-ChangeX);
                    MoveAllNPCsX(-ChangeX);
                    SetFrame(FACE_SIDE);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                }
            }
            else {
                X += VecX * (DeltaTicks/1000.f);
                if ((Area.GetX() + X - OldX) > 8) {
                    if (AnimFrame == 0) SetFrame(WALK_SIDE3);
                    else SetFrame(WALK_SIDE1);
                }
                if (Area.GetX() + X >= OldX + 16) {
                    Moving = false;
                    X = OldX-Area.GetX() + 16;
                    SetFrame(FACE_SIDE);
                    if (AnimFrame == 0) AnimFrame = 1;
                    else AnimFrame = 0;
                }
            }
        }
        EmbededLua.Call_PlayerMove();
    }
    Sprite[SpriteNum].SetLoc(X, Y); 
}

void PlayerClass::SetDire(int Dire) 
{
    if (Moving) return; // Don't run this if we're already moving
    Direction = Dire;
    switch (Dire)
    {
    case DIRE_UP:
        SetFlip(false);
        SetFrame(FACE_UP);
        break;
    case DIRE_DOWN:
        SetFlip(false);
        SetFrame(FACE_DOWN);
        break;
    case DIRE_LEFT:
        SetFlip(false);
        SetFrame(FACE_SIDE);
        break;
    case DIRE_RIGHT:
        SetFlip(true);
        SetFrame(FACE_SIDE);
        break;
    }
}
    
void PlayerClass::SetMoving() 
{
    if (Moving) return; // Don't run this if we're already moving

    int OffX = X, OffY = Y;
    if (Direction == DIRE_UP) OffY -= 16;
    else if (Direction == DIRE_DOWN) OffY += 16;
    else if (Direction == DIRE_LEFT) OffX -= 16;
    else if (Direction == DIRE_RIGHT) OffX += 16;

    // NPCs should be solid!
    int NPCNum = FindNPCAbs(OffX - Area.GetX(), OffY - Area.GetY(), ZLevel); 
    if (NPCNum != -1) return;

    int Collision = 
        Area.CheckCollision(-Area.GetX()+OffX, -Area.GetY()+OffY+12, 16, 12);

    if (Collision == 1) return;
    else if (Collision == 2) {
        if (Area.Load("Data/town.xml", "a") == -1 ) cout << "Area fail!\n";
        return;
    }

    Moving = true; 
    OldX = Area.GetX() + int(X); 
    OldY = Area.GetY() + int(Y); 
}

