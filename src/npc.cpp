#include <stdlib.h>
#include "npc.h"
#include "player.h"
#include "game.h"

//#define uint unsigned int

vector<NPCClass> NPC;

void NPCClass::AddDialogue(DialogueStruct Dialogue_)
{
    Dialogue.push_back(Dialogue_);
}

string NPCClass::GetString(uint DNum, uint BNum)
{
    if (DNum >= Dialogue.size() || BNum >= Dialogue[DNum].Message.size())
        return "";
    return Dialogue[DNum].Message[BNum].Message;
}

OptionStruct NPCClass::GetOption(uint DNum, uint BNum, uint Option)
{
    OptionStruct blank;
    if (DNum >= Dialogue.size() || BNum >= Dialogue[DNum].Message.size() ||
        Option >= Dialogue[DNum].Message[BNum].Option.size())
        return blank;
    return Dialogue[DNum].Message[BNum].Option[Option];
}

int NPCClass::GetOptionCount(uint DNum, uint BNum)
{
    if (DNum >= Dialogue.size() || BNum >= Dialogue[DNum].Message.size()) {
        return -1;
    }
    return Dialogue[DNum].Message[BNum].Option.size();
}

int NPCClass::GetDialogueNum()
{
    if (Dialogue.size() == 0) return -1;
    return rand() % Dialogue.size();
}

/** Return the string ID to an int ID for boxes in a dialog */
int NPCClass::IDToBox(uint DNum, string ID)
{
    if (DNum >= Dialogue.size()) return -1;
    for (int i = 0; i < Dialogue[DNum].Message.size(); ++i) {
        if (Dialogue[DNum].Message[i].ID == ID) return i;
    }
    return -1;
}

bool NPCClass::IsEndMessage(uint DNum, uint BNum)
{
    if (DNum >= Dialogue.size() || BNum >= Dialogue[DNum].Message.size()) {
        return false;
    }
    return Dialogue[DNum].Message[BNum].End;    
}

void NPCClass::StartBattle(uint DNum, uint BNum)
{
    if (DNum >= Dialogue.size() || BNum >= Dialogue[DNum].Message.size()) {
        return;
    }
    if (Dialogue[DNum].Message[BNum].Battle) {
        Game.SetMode('b'); 
    }
}

void NPCClass::Move(Uint32 DeltaTicks)
{
    if (!Moving) return;
    if (Direction == DIRE_UP) {
        Y -= (VecY * (DeltaTicks/1000.f));
        if (Y <= (OldY - 8)) {
            if (AnimFrame == 0) SetFrame(WALK_UP1);
            else SetFrame(WALK_UP3);
        }
        if (Y <= OldY - 16) {
            Moving = false;
            Y = int(OldY - 16);
            if (int(Y)%16 != 8)
                cout << "FATAL ERROR: Y " << (int(Y)%16) << endl;
            if (int(X)%16 != 0)
                cout << "FATAL ERROR: X " << (int(X)%16) << endl;
            SetFrame(FACE_UP);
            if (AnimFrame == 0) AnimFrame = 1;
            else AnimFrame = 0;
            SwapPlayerOrder();
        }
    }
    else if (Direction == DIRE_DOWN) {
        Y += (VecY * (DeltaTicks/1000.f));
        if (Y >= (OldY + 8)) {
            if (AnimFrame == 0) SetFrame(WALK_DOWN1);
            else SetFrame(WALK_DOWN3);
        }
        if (Y >= OldY + 16) {
            Moving = false;
            Y = int(OldY + 16);
            if (int(Y)%16 != 8)
                cout << "FATAL ERROR: Y " << (int(Y)%16) << endl;
            if (int(X)%16 != 0)
                cout << "FATAL ERROR: X " << (int(X)%16) << endl;
            SetFrame(FACE_DOWN);
            if (AnimFrame == 0) AnimFrame = 1;
            else AnimFrame = 0;
            SwapPlayerOrder();
        }
    }
    else if (Direction == DIRE_LEFT) {
        X -= (VecX * (DeltaTicks/1000.f));
        if (X <= (OldX - 8)) {
            if (AnimFrame == 0) SetFrame(WALK_SIDE1);
            else SetFrame(WALK_SIDE3);
        }
        if (X <= OldX - 16) {
            Moving = false;
            X = int(OldX - 16);
            if (int(Y)%16 != 8)
                cout << "FATAL ERROR: Y " << (int(Y)%16) << endl;
            if (int(X)%16 != 0)
                cout << "FATAL ERROR: X " << (int(X)%16) << endl;
            SetFrame(FACE_SIDE);
            if (AnimFrame == 0) AnimFrame = 1;
            else AnimFrame = 0;
        }
    }
    else if (Direction == DIRE_RIGHT) {
        X += (VecX * (DeltaTicks/1000.f));
        if (X >= (OldX + 8)) {
            if (AnimFrame == 0) SetFrame(WALK_SIDE1);
            else SetFrame(WALK_SIDE3);
        }
        if (X >= OldX + 16) {
            Moving = false;
            X = int(OldX + 16);
            if (int(Y)%16 != 8)
                cout << "FATAL ERROR: Y " << (int(Y)%16) << endl;
            if (int(X)%16 != 0)
                cout << "FATAL ERROR: X " << (int(X)%16) << endl;
            SetFrame(FACE_SIDE);
            if (AnimFrame == 0) AnimFrame = 1;
            else AnimFrame = 0;
        }
    }
    UpdateSprite();
}

void NPCClass::SetDire(int Dire) 
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

void NPCClass::SetMoving()
{
    if (Moving) return;
    Y = int(Y+0.5f);
    X = int(X+0.5f);
    int OffX = X, OffY = Y;
    if (Direction == DIRE_UP) OffY -= 16;
    else if (Direction == DIRE_DOWN) OffY += 16;
    else if (Direction == DIRE_LEFT) OffX -= 16;
    else if (Direction == DIRE_RIGHT) OffX += 16;
    
    // NPCs should be solid!
    int NPCNum = FindNPCAbs(OffX, OffY, ZLevel);    
    if (NPCNum != -1) return;

    // Running through the player would be silly
    if (Player.GetX() == OffX && Player.GetY() == OffY 
        && Player.GetZ() == ZLevel) 
        return;

    int Collision = Area.CheckCollision(OffX, OffY+12, 16, 12);
    if (Collision == 1) return;

    Moving = true;
    OldX = X;
    OldY = Y;
} 

void MoveAllNPCs(float X, float Y)
{
/*  for (unsigned int i = 0; i < NPC.size(); ++i) {
        NPC[i].AddX(X);
        if (NPC[i].IsMoving()) NPC[i].AddOldX(X);
        NPC[i].AddY(Y);
        if (NPC[i].IsMoving()) NPC[i].AddOldY(Y);
        NPC[i].UpdateSprite();
    }*/
}

void MoveAllNPCsX(float X)
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
//      NPC[i].AddX(X);
//      if (NPC[i].IsMoving()) NPC[i].AddOldX(X);
//      NPC[i].UpdateSprite();
        NPC[i].UpdateSprite();
    }
}

void MoveAllNPCsY(float Y)
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
    //  NPC[i].AddY(Y);
//      if (NPC[i].IsMoving()) NPC[i].AddOldY(Y);
        NPC[i].UpdateSprite();
    }
}

int AddNPC(int SpriteNum)
{
    NPC.push_back(NPCClass());
    NPC[NPC.size()-1].SetSprite(SpriteNum);
    return NPC.size()-1;
}

int FindNPC(int X, int Y)
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
        if (X == int(NPC[i].GetScreenX()+0.5f)
            && Y == int(NPC[i].GetScreenY()+0.5f))
            return i;
    }
    return -1;
}

int FindNPC(int X, int Y, int Z)
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
        if (X == int(NPC[i].GetScreenX()+0.5f)
            && Y == int(NPC[i].GetScreenY()+0.5f) && NPC[i].GetZ() == Z)
            return i;
    }
    return -1;
}

int FindNPCAbs(int X, int Y, int Z)
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
        if (NPC[i].IsMoving()) {
            switch (NPC[i].GetDire())
            {
            case DIRE_UP:
                if (X == int(NPC[i].GetX()+0.5f) && Y < NPC[i].GetOldY() + 19
                    && Y > NPC[i].GetOldY() - 17 && NPC[i].GetZ() == Z)
                    return i;
                break;
            case DIRE_DOWN:
                if (X == int(NPC[i].GetX()+0.5f) && Y > NPC[i].GetOldY() - 17 
                    && Y < NPC[i].GetOldY() + 17 && NPC[i].GetZ() == Z)
                    return i;
                break;
            case DIRE_RIGHT:
                if (Y == int(NPC[i].GetY()+0.5f) && X > NPC[i].GetOldX() - 17 
                    && X < NPC[i].GetOldX() + 17 && NPC[i].GetZ() == Z)
                    return i;
                break;
            case DIRE_LEFT:
                if (Y == int(NPC[i].GetY()+0.5f) && X < NPC[i].GetOldX() + 17
                    && X > NPC[i].GetOldX() - 17 && NPC[i].GetZ() == Z)
                    return i;
                break;
            }
        }
        else {
            if (X == int(NPC[i].GetX()+0.5f) && Y == int(NPC[i].GetY()+0.5f)
                && NPC[i].GetZ() == Z)
                return i;
        }
    }
    return -1;
}

void DeleteNPC(unsigned int NPCNum)
{
    if (NPCNum < NPC.size()) {
        DeleteSprite(NPC[NPCNum].GetSprite());
        NPC.erase(NPC.begin()+NPCNum);
    }
}

void DeleteAllNPCs()
{
    for (int i = NPC.size()-1; i >= 0; --i) DeleteSprite(NPC[i].GetSprite());
    NPC.clear();
}

void SwapPlayerOrder()
{
    for (unsigned int i = 0; i < NPC.size(); ++i) {
        if (Player.GetY() < NPC[i].GetY() && Player.GetZ() <= NPC[i].GetZ()) {
            SwapSpriteInFrontOf(NPC[i].GetSprite(), Player.GetSprite());
        }
        else if (Player.GetY() > NPC[i].GetY() 
                && Player.GetZ() >= NPC[i].GetZ()) {
            SwapSpriteBehind(NPC[i].GetSprite(), Player.GetSprite()); 
        }
        else if (Player.GetZ() > NPC[i].GetZ()) {
            SwapSpriteBehind(NPC[i].GetSprite(), Player.GetSprite()); 
        }
        else if (Player.GetZ() < NPC[i].GetZ()) {
            SwapSpriteInFrontOf(NPC[i].GetSprite(), Player.GetSprite()); 
        }
    } 
}

