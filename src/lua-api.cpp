#include <iostream>
#include <sstream>
using namespace std;

#include "lua-api.h"
#include "embed.h"

#include "player.h"
#include "npc.h"
#include "sprite.h"
#include "window.h"
#include "game.h"
#include "battle.h"

/** The Gekinmon API */

/** Creates a sprite based on an image */
static int gekinmon_CreateSprite(lua_State * L)
{
    int argc = lua_gettop(L);
    // Only load the sprite, don't place it. No transparency
    if (argc == 1) {
        if (lua_isstring(L, 1)) {
            int SpriteNum = Window.LoadSprite(lua_tostring(L, 1));
            SetSpriteOrder(SpriteNum, 0);
            Sprite[SpriteNum].SetDisplay(false);
            lua_pushnumber(L, SpriteNum);
            return 1;
        }
    }
    // Only load the sprite, don't place it, Transparency
    else if (argc == 4) {
        if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) 
        && lua_isnumber(L, 4)) {
            int SpriteNum = Window.LoadSprite(lua_tostring(L, 1), 
                            lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4));
            SetSpriteOrder(SpriteNum, 0);
            Sprite[SpriteNum].SetDisplay(false);
            lua_pushnumber(L, SpriteNum);
            return 1;
        }
    }
    // No transparency
    else if (argc == 3) {
        if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
            int SpriteNum = Window.LoadSprite(lua_tostring(L, 1));
            SetSpriteOrder(SpriteNum, 0);
            Sprite[SpriteNum].SetLoc(lua_tonumber(L, 2), lua_tonumber(L, 3));
            lua_pushnumber(L, SpriteNum);
            return 1;
        }
    }
    // Transparency
    else if (argc == 6) {
        if (lua_isstring(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)
         && lua_isnumber(L, 4) && lua_isnumber(L, 5) && lua_isnumber(L, 6)) {
            int SpriteNum = Window.LoadSprite(lua_tostring(L, 1), 
                            lua_tonumber(L, 4), lua_tonumber(L, 5), lua_tonumber(L, 6));
            SetSpriteOrder(SpriteNum, 0);
            Sprite[SpriteNum].SetLoc(lua_tonumber(L, 2), lua_tonumber(L, 3));
            lua_pushnumber(L, SpriteNum);
            return 1;
        }
    }
    
    lua_pushnumber(L, -1);
    return 1;
}

/** Creates a sprite with some text on it and puts it onto the screen. */
static int gekinmon_CreateText(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 7) {
        lua_pushnumber(L, -1);
        return 1;
    }

    if (lua_isnumber(L, 1) && lua_isstring(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)
       && lua_isnumber(L, 5) && lua_isnumber(L, 6) && lua_isnumber(L, 7)) 
    {
        int SpriteNum = Font.CreateText(lua_tonumber(L, 1), lua_tostring(L, 2),
                        lua_tonumber(L, 5), lua_tonumber(L, 6), lua_tonumber(L, 7));
        SetSpriteOrder(SpriteNum, 0); 
        Sprite[SpriteNum].SetLoc(lua_tonumber(L, 3), lua_tonumber(L, 4));
        lua_pushnumber(L, SpriteNum);
        return 1;
    }
    lua_pushnumber(L, -1);
    return 1;
}

/** Deletes a sprite */
static int gekinmon_DeleteSprite(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;

    if (lua_isnumber(L, 1)) {
        DeleteSprite(lua_tonumber(L, 1));
    } 

    return 0;
}

static int gekinmon_FlipSprite(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;

    if (lua_isnumber(L, 1)) {
        int SpriteNum = lua_tonumber(L, 1);
        if (Sprite[SpriteNum].GetBPP() == 32)
            Sprite[SpriteNum].FlipSurface();
        else
            EmbededLua.Log(LOG_WARNING, "Can only flip 32bit images.");
    }
    return 0;
}

static int gekinmon_SetSpriteX(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 2) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        Sprite[lua_tonumber(L, 1)].SetX(lua_tonumber(L, 2));
    }
    return 0;
}

static int gekinmon_SetSpriteY(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 2) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2)) {
        Sprite[lua_tonumber(L, 1)].SetY(lua_tonumber(L, 2));
    }
    return 0;
}

static int gekinmon_SetSpriteXY(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 3) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
        Sprite[lua_tonumber(L, 1)].SetLoc(lua_tonumber(L, 2), lua_tonumber(L, 3));
    }
    return 0;
}

static int gekinmon_DisplaySprite(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 2) return 0;
    if (lua_isnumber(L, 1) && lua_isboolean(L, 2)) {
        Sprite[lua_tonumber(L, 1)].SetDisplay(lua_toboolean(L, 2));
    }
    return 0;
}

static int gekinmon_GetSpriteWidth(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;
    if (lua_isnumber(L, 1)) {
        lua_pushnumber(L, Sprite[lua_tonumber(L, 1)].GetWidth());
        return 1;
    }
    return 0;
}

static int gekinmon_GetSpriteHeight(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;
    if (lua_isnumber(L, 1)) {
        lua_pushnumber(L, Sprite[lua_tonumber(L, 1)].GetHeight());
        return 1;
    }
    return 0;
}

/** Returns the player's current X coordinate to the Lua script */
static int gekinmon_GetPlayerX(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return 0;

    lua_pushnumber(L, int(Player.GetX()));
    return 1;
}

/** Returns the player's current Y coordinate to the Lua script */
static int gekinmon_GetPlayerY(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return 0;

    lua_pushnumber(L, int(Player.GetY()));
    return 1;
}

/** Returns the width of the window. */
static int gekinmon_GetWindowWidth(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return 0;

    lua_pushnumber(L, Window.GetViewWidth());
    return 1;
}

/** Returns the height of the window. */
static int gekinmon_GetWindowHeight(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return 0;

    lua_pushnumber(L, Window.GetViewHeight());
    return 1;
}

static int gekinmon_GetNPCCount(lua_State * L)
{
    lua_newtable(L);
    for (unsigned int i = 0; i < NPC.size(); ++i)
    {
        lua_pushnumber(L, i+1);
        lua_pushnumber(L, i+1);
        lua_rawset(L, -3);
    }
    return 1;

}

/** Lets the plugin log messages. */
static int gekinmon_Log(lua_State * L)
{
    if (lua_gettop(L) != 1) return 0;

    if (lua_isstring(L, 1))
    {
        EmbededLua.Log(LOG_MESSAGE, lua_tostring(L, 1));
    }
    return 0;
}

/** Registers a plugin */
static int gekinmon_Register(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 5) return 0;

    if (lua_isstring(L, 1) && lua_isstring(L, 2) && lua_isstring(L, 3) && lua_isstring(L, 4)
       && lua_isstring(L, 5))
    {
        Gekinmon_Plugin PluginTmp;
        PluginTmp.Name        = lua_tostring(L, 1);
        PluginTmp.Author      = lua_tostring(L, 2);
        PluginTmp.Version     = lua_tostring(L, 3);
        PluginTmp.License     = lua_tostring(L, 4);
        PluginTmp.Description = lua_tostring(L, 5);
        PluginTmp.Interpreter = L;

        Plugin.push_back(PluginTmp); 

        stringstream Output;
        Output << "Registered plugin \"" << PluginTmp.Name << "\", " << PluginTmp.Version
               << " (" << PluginTmp.Description << ")";
        EmbededLua.Log(LOG_MESSAGE, Output.str());
    } 
    return 0;
}

/** Registers an event for the plugin. */
static int gekinmon_RegisterEvent(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 2) return 0;
    if (lua_isstring(L, 1) && lua_isstring(L, 2)) {
        int PluginNum = EmbededLua.GetPluginFromInter(L);
        if (strcmp("KeyPress", lua_tostring(L, 1)) == 0) {
            Plugin[PluginNum].Event_KeyPress = lua_tostring(L, 2);
        } else if (strcmp("PlayerMove", lua_tostring(L, 1)) == 0) {
            Plugin[PluginNum].Event_PlayerMove = lua_tostring(L, 2);
        } else if (strcmp("NewDialog", lua_tostring(L, 1)) == 0) {
            Plugin[PluginNum].Event_NewDialog = lua_tostring(L, 2);
        } else if (strcmp("EndDialog", lua_tostring(L, 1)) == 0) {
            Plugin[PluginNum].Event_EndDialog = lua_tostring(L, 2);
        }
    }
    return 0;
}

static int gekinmon_Error(lua_State * L)
{
    cout << "Error func: " << lua_tostring(L, 1) << endl;
}

/** Returns the text for a NPC */
static int gekinmon_GetDialog(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 3) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
        int NPCNum = lua_tonumber(L, 1);
        int DNum = lua_tonumber(L, 2);
        int BNum = lua_tonumber(L, 3);
        lua_pushstring(L, NPC[NPCNum].GetString(DNum, BNum).c_str());
        return 1;
    }
    return 0;
}

static int gekinmon_GetDialogOption(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 4) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3) && lua_isnumber(L, 4)) {
        int NPCNum = lua_tonumber(L, 1);
        int DNum = lua_tonumber(L, 2);
        int BNum = lua_tonumber(L, 3);
        int OptionID = lua_tonumber(L, 4);
        lua_pushstring(L, NPC[NPCNum].GetOption(DNum, BNum, OptionID).Title.c_str());
        return 1;
    }
    return 0;
}

/** Returns the number of options for the dialog */
static int gekinmon_GetDialogOptionCount(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 3) return 0;
    if (lua_isnumber(L, 1) && lua_isnumber(L, 2) && lua_isnumber(L, 3)) {
        int NPCNum = lua_tonumber(L, 1);
        int DNum = lua_tonumber(L, 2);
        int BNum = lua_tonumber(L, 3);
        lua_pushnumber(L, NPC[NPCNum].GetOptionCount(DNum, BNum));
        return 1;
    }
    return 0;
}

/** Lets a script pick which option the dialog should use */
static int gekinmon_SetDialogOptionID(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;
    if (lua_isnumber(L, 1)) {
        Game.SetOptionID(lua_tonumber(L, 1));
    }
    return 0;
}

/** Returns the current game mode */
static int gekinmon_GetGameMode(lua_State * L)
{
    char Mode = Game.GetMode();
    if (Mode == 'g') lua_pushstring(L, "Game");
    else if (Mode == 'd') lua_pushstring(L, "Dialog");
    else if (Mode == 'b') lua_pushstring(L, "Battle");
    else lua_pushstring(L, "Unknown");
    return 1;
}

/** Loads a script */
static int gekinmon_LoadScript(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;
    if (lua_isstring(L, 1)) {
        string FileName = BASE_PATH;
        FileName += lua_tostring(L, 1);
        EmbededLua.Load(FileName.c_str());        
    }
    return 0;
}

/** Unloads a script by name */
static int gekinmon_UnloadScript(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;

    if (lua_isstring(L, 1)) {
        for (int i = 0; i < Plugin.size(); ++i)
        {
            if (Plugin[i].Name == lua_tostring(L, 1)) {
 
                if (Plugin[i].Interpreter == L) {
                    EmbededLua.Log(LOG_ERROR, "A plugin cannot unload itself with this function.");
                    return 0;
                }
                EmbededLua.Unload(i);
                return 0;
            }
        }
    }
    return 0;
}

static int gekinmon_GetAllyHP(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;

    if (lua_isnumber(L, 1)) {
        lua_pushnumber(L, Battle.GetAllyHP(lua_tonumber(L, 1))); 
        return 1;
    }
    return 0;
}

static int gekinmon_GetEnemyHP(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 1) return 0;

    if (lua_isnumber(L, 1)) {
        lua_pushnumber(L, Battle.GetEnemyHP(lua_tonumber(L, 1))); 
        return 1;
    }
    return 0;
}

static int gekinmon_GetAllyCount(lua_State * L)
{
    lua_pushnumber(L, Battle.GetAllyCount());
    return 1;
}

static int gekinmon_GetEnemyCount(lua_State *L)
{
    lua_pushnumber(L, Battle.GetEnemyCount());
    return 1;
}

static int gekinmon_Attack(lua_State * L)
{
    int argc = lua_gettop(L);
    if (argc != 4) return false;
    if (lua_isboolean(L, 1) && lua_isnumber(L, 2) && lua_isboolean(L, 3) && lua_isnumber(L, 4))
    {
        Battle.Attack(lua_toboolean(L, 1), lua_tonumber(L, 2), lua_toboolean(L, 3), lua_tonumber(L, 4));

    }

    return 0;
}

const struct luaL_reg gekinmon_lua_api [] = {
    {"CreateSprite",        &gekinmon_CreateSprite },
    {"CreateText",          &gekinmon_CreateText },
    {"DeleteSprite",        &gekinmon_DeleteSprite },
    {"FlipSprite",          &gekinmon_FlipSprite },
    {"GetSpriteWidth",      &gekinmon_GetSpriteWidth },
    {"GetSpriteHeight",     &gekinmon_GetSpriteHeight },
    {"SetSpriteX",          &gekinmon_SetSpriteX },
    {"SetSpriteY",          &gekinmon_SetSpriteY },
    {"SetSpriteXY",         &gekinmon_SetSpriteXY },
    {"DisplaySprite",       &gekinmon_DisplaySprite },
    {"GetPlayerX",          &gekinmon_GetPlayerX },
    {"GetPlayerY",          &gekinmon_GetPlayerY },
    {"GetWindowWidth",      &gekinmon_GetWindowWidth },
    {"GetWindowHeight",     &gekinmon_GetWindowHeight },
    {"GetNPCList",          &gekinmon_GetNPCCount },
    {"Log",                 &gekinmon_Log },
    {"Register",            &gekinmon_Register },
    {"RegisterEvent",       &gekinmon_RegisterEvent },
    {"GetDialog",           &gekinmon_GetDialog },
    {"GetDialogOption",     &gekinmon_GetDialogOption },
    {"GetDialogOptionCount",&gekinmon_GetDialogOptionCount },
    {"SetDialogOptionID",   &gekinmon_SetDialogOptionID },
    {"GetGameMode",         &gekinmon_GetGameMode },
    {"LoadScript",          &gekinmon_LoadScript },
    {"UnloadScript",        &gekinmon_UnloadScript },
    {"GetAllyHP",           &gekinmon_GetAllyHP },
    {"GetEnemyHP",          &gekinmon_GetEnemyHP },
    {"GetAllyCount",        &gekinmon_GetAllyCount },
    {"GetEnemyCount",       &gekinmon_GetEnemyCount },
    {"Attack",              &gekinmon_Attack },
    {NULL, NULL}
};

