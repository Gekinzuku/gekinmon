#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "game.h"
#include "area.h"
#include "npc.h"
#include "player.h"
#include "quest.h"
#include "embed.h"
#include "window.h"
#include "battle.h"

SDLGame Game;
Uint32 Ticks;
bool StopMoving = false;

void SDLGame::OpenDialogue(int NPCNum_)
{
    NPCNum = NPCNum_;
    // Only set the DNum/BNum if it's a new sequence, 
    // otherwise we'd never see the end!
    if (DNum == -1) {
        Quest.CheckNPC(Area.GetName(), NPC[NPCNum].GetID());
        DNum = NPC[NPCNum].GetDialogueNum();
        BNum = 0;
    }

    /** Let's run any scripts, perhaps so we can actually see what's going on. */
    EmbededLua.Call_NewDialog(NPCNum, DNum, BNum);
    Mode = 'd';
}

void SDLGame::CloseDialogue()
{
    // Check if there is more dialogue in this sequence before quitting
    if (NPC[NPCNum].GetDialogueSize(DNum)-1 > BNum
        && !NPC[NPCNum].IsEndMessage(DNum, BNum)) {
        if (NPC[NPCNum].GetOptionCount(DNum, BNum) > 1) 
            BNum = NPC[NPCNum].IDToBox(DNum, NPC[NPCNum].GetOption(DNum, BNum, OptionID).JumpID);
        else
            ++BNum;
        OpenDialogue(NPCNum);
        return;
    }
    Mode = 'g';
    NPC[NPCNum].StartBattle(DNum, BNum);

    DNum = BNum = NPCNum = -1;
    EmbededLua.Call_EndDialog(NPCNum, DNum);
}

void SDLGame::HandleKeyUp(int Key)
{
    // If a Lua script doesn't want a certain button pressed
    if (!EmbededLua.Call_KeyFunc(Key)) return;

    switch (Key) {
    case SDLK_u: // Move up a Z level
        Player.SetZ(Player.GetZ()+1);
        SwapPlayerOrder();
        Area.SetZLevel(Player.GetSprite(), Player.GetZ());
        cout << "Z Level: " << Player.GetZ() << endl;
        break;
    case SDLK_j: // Move down a Z level
        Player.SetZ(Player.GetZ()-1);
        SwapPlayerOrder();
        Area.SetZLevel(Player.GetSprite(), Player.GetZ());
        cout << "Z Level: " << Player.GetZ() << endl;
        break;
    case SDLK_o:
        NPC[0].SetDire(DIRE_UP);
        NPC[0].SetMoving();
        break;
    case SDLK_k:
        NPC[0].SetDire(DIRE_LEFT);
        NPC[0].SetMoving();
        break;
    case SDLK_l:
        NPC[0].SetDire(DIRE_DOWN);
        NPC[0].SetMoving();
        break;
    case SDLK_SEMICOLON:
        NPC[0].SetDire(DIRE_RIGHT);
        NPC[0].SetMoving();
        break;
    case SDLK_n:
        cout << "NPC: " << NPC[0].GetX() << ", " << NPC[0].GetY() << " :: " << 
            int(NPC[0].GetY()-Area.GetY()) << endl;  
        cout << "Player: " << Player.GetX() << ", " << Player.GetY() << endl;   
        break;
    case SDLK_RETURN:
        if (Area.Load("Data/town.xml", "a") == -1) cout << "Area fail!\n";
        break;
    case SDLK_SPACE:
        StopMoving = !StopMoving;
        break;
    case SDLK_p:
        cout << "Size of sprite: " << Sprite.size() << ", " << 
            SpriteOrder.size() << endl;
        for (unsigned int i = 0; i < SpriteOrder.size(); ++i) 
            cout << SpriteOrder[i] << ", ";
        cout << endl;
        break;
    case SDLK_f:
        int NPCNum;
        if (Mode == 'd') {
            CloseDialogue();
            break;
        }
        switch (Player.GetDire())
        {
        case DIRE_UP:
            NPCNum = FindNPC(Player.GetX(), Player.GetY()-16, Player.GetZ());
            if (NPCNum != -1) {
                NPC[NPCNum].SetFrame(FACE_DOWN);
                NPC[NPCNum].SetFlip(FLIP_LEFT);
                OpenDialogue(NPCNum);
            }
            break;
        case DIRE_DOWN:
            NPCNum = FindNPC(Player.GetX(), Player.GetY()+16, Player.GetZ());
            if (NPCNum != -1) {
                NPC[NPCNum].SetFrame(FACE_UP);
                NPC[NPCNum].SetFlip(FLIP_LEFT);
                OpenDialogue(NPCNum);
            }
            break;
        case DIRE_LEFT:
            NPCNum = FindNPC(Player.GetX()-16, Player.GetY(), Player.GetZ());
            if (NPCNum != -1) {
                NPC[NPCNum].SetFrame(FACE_SIDE);
                NPC[NPCNum].SetFlip(FLIP_RIGHT);
                OpenDialogue(NPCNum);
            }
            break;
        case DIRE_RIGHT:
            NPCNum = FindNPC(Player.GetX()+16, Player.GetY(), Player.GetZ());
            if (NPCNum != -1) {
                NPC[NPCNum].SetFrame(FACE_SIDE);
                NPC[NPCNum].SetFlip(FLIP_LEFT);
                OpenDialogue(NPCNum);
            }
            break;
        }
        break;
    }

}

void SDLGame::HandleKeyDown(int Key)
{
    switch (Key) {
/*  case SDLK_UP: Player.AddVecY(-100); break;
    case SDLK_DOWN: Player.AddVecY(100); break;
    case SDLK_LEFT: Player.AddVecX(-100); break;
    case SDLK_RIGHT: Player.AddVecX(100); break;*/
    }
}

void SDLGame::Execute()
{
    if (Mode == 'g') {
        Uint8 *Keys = SDL_GetKeyState(NULL);
        if (Keys[SDLK_UP] || Keys[SDLK_w]) {
            Player.SetDire(DIRE_UP);
            Player.SetMoving();
        }
        else if (Keys[SDLK_DOWN] || Keys[SDLK_s]) {
            Player.SetDire(DIRE_DOWN);
            Player.SetMoving();
        }
        else if (Keys[SDLK_LEFT] || Keys[SDLK_a]) {
            Player.SetDire(DIRE_LEFT);
            Player.SetMoving();
        }
        else if (Keys[SDLK_RIGHT] || Keys[SDLK_d]) {
            Player.SetDire(DIRE_RIGHT);
            Player.SetMoving();
        }
        Player.Move(SDL_GetTicks() - Ticks);

        // Actually move the NPCs
        for (unsigned int i = 0; i < NPC.size(); ++i) {
//          if (SDL_GetTicks() % 1024 < rand() % 256) {
            if (!StopMoving) {
        //      NPC[i].SetDire(rand() % (DIRE_RIGHT+1));
        //      NPC[i].SetMoving();
            }
            NPC[i].Move(SDL_GetTicks() - Ticks);
        }
        Area.Update();
    }
    else if (Mode == 'b') // Battle mode
    {
        static int SpriteNum = -1, HP_Old = -1; 
        int HP = Battle.GetAllyHP(0);
        if (HP_Old != HP) {
            if (SpriteNum != -1) DeleteSprite(SpriteNum);
            char Buffer[256];
            if (HP > 0) sprintf(Buffer, "HP: %d", HP);
            else sprintf(Buffer, "Dead");
            SpriteNum = Font.CreateText(0, Buffer, 255, 255, 255);
            SetSpriteOrder(SpriteNum, 0);
            Sprite[SpriteNum].SetLoc(0,0); 
            cout << "HP updated: " << (HP_Old - HP) << endl;
            HP_Old = HP;
        }
    }
    else
    {
        // The game has been borked
        //printf("Panic! Mode set to \"%c\"\n", Mode);
    }
}

void SDLGame::OpenMenu()
{
/*  if (Mode == 'g') {
        Mode = 'm';
        Font.CreateText(0,"I was like yo dawg, and you blew me.", 255, 0, 255);
        Sprite[Sprite.size()-1].SetLoc(128, 128);
    }
    else {
        Mode = 'g';
        Sprite[Sprite.size()-1].CleanUp();
        Sprite.erase(Sprite.begin()+Sprite.size()-1);
    }*/
}

/** Sets the option for dialog */
void SDLGame::SetOptionID(int OptionID_) {
    OptionID = OptionID_;
}
