#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "sprite.h"
#include "window.h"
#include "player.h"
#include "npc.h"
#include "area.h"
#include "quest.h"
#include "embed.h"
#include "game.h"
using namespace std;

#define FPS_LIMIT   60

Uint32 Timer = 0;

int main( int argc, char* args[] )
{
    Quest.Load("Data/quest01.xml");


    if (!Window.Init(1024,768,32,"Gekinmon")) {
        cout << "Could not init SDL.\n";
        return 1;
    }
    bool quit = false;
    SDL_Event event;

    srand(time(NULL));


    Timer = SDL_GetTicks();
    // Battle test
    /*SetSpriteOrder(Window.LoadSprite("Data/battle.png"), -1);
    SetSpriteOrder(Window.LoadSprite("Data/dialoguebg.png", 255, 0, 255), 0);
    Sprite[Sprite.size()-1].SetLoc(44, 90);
    SetSpriteOrder(Font.CreateText(0,"You'll never get my pills! Prepare to die!",
                    255,255,255),0);
    Sprite[Sprite.size()-1].SetLoc(48, 96);
    NPCClass MyNPC; 
    MyNPC.SetSprite(Window.LoadSprite("Data/oldman.png", 255, 0, 255));
    SetSpriteOrder(MyNPC.GetSprite(), 0);
    MyNPC.SplitFrames(16,24);
    MyNPC.SetLoc(16*5, VIEW_HEIGHT - 24 * 5);
    MyNPC.UpdateSprite();
    MyNPC.SetFrame(FACE_SIDE);
    MyNPC.Flip();*/

    Player.SetSprite(Window.LoadSprite("Data/locke.png", 255, 0, 255));
    SetSpriteOrder(Player.GetSprite(), 0);
    Player.SplitFrames(16, 24);
    Player.SetLoc(VIEW_WIDTH- 16*5, VIEW_HEIGHT - 24*5);
    Sprite[Player.GetSprite()].SetLoc(Player.GetX(), Player.GetY()); 
    Player.SetFrame(0);
    if (Area.Load("Data/level.xml", "a") == -1) return 1;

//  SwapSpriteInFrontOf(Player.GetSprite(), NPC[0].GetSprite());
/*  char Buffer[512];
    sprintf(Buffer, "XML Loading: %d miliseconds.", (SDL_GetTicks()-Timer));
    SetSpriteOrder(Font.CreateText(0,Buffer,0,0,0),1);
    Sprite[Sprite.size()-1].SetLoc(48, 96);*/
    
    EmbededLua.LoadFromXML("Data/scripts/scripts.xml", "Data/scripts/");

    while(!quit) {
        // Keyboard input
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                Game.HandleKeyDown(event.key.keysym.sym);
            }
            else if (event.type == SDL_KEYUP) {
                Game.HandleKeyUp(event.key.keysym.sym);
            }
            else if (event.type == SDL_QUIT) quit = true;

        }
        Game.Execute();
        static int Frames = 0, LastFrame = Ticks;
        ++Frames;
        if (SDL_GetTicks() - LastFrame > 1000) {
            char Buffer[512];
            sprintf(Buffer, "Gekinmon. FPS: %d     ", Frames);
            SDL_WM_SetCaption(Buffer, NULL);
            Frames = 0;
            LastFrame = Ticks;
        }

        Ticks = SDL_GetTicks();
        Window.Render();
        if ((SDL_GetTicks() - Ticks) < 1000 / FPS_LIMIT ) {
            int Delay = (1000/FPS_LIMIT) - (SDL_GetTicks() - Ticks);
            SDL_Delay(Delay);
        }
    }

    //Quit SDL
    TTF_Quit();
    SDL_Quit();

    return 0;
}
