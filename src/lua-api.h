#ifndef LUA_API_H
#define LUA_API_H

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static int gekinmon_CreateSprite(lua_State * L);
static int gekinmon_CreateText(lua_State * L);
static int gekinmon_DeleteSprite(lua_State * L);
static int gekinmon_FlipSprite(lua_State * L);
static int gekinmon_SetSpriteX(lua_State * L);
static int gekinmon_SetSpriteY(lua_State * L);
static int gekinmon_SetSpriteXY(lua_State * L);
static int gekinmon_DisplaySprite(lua_State *L);
static int gekinmon_GetSpriteWidth(lua_State * L);
static int gekinmon_GetSpriteHeight(lua_State * L);

static int gekinmon_GetPlayerX(lua_State * L);
static int gekinmon_GetPlayerY(lua_State * L);
static int gekinmon_GetWindowWidth(lua_State * L);
static int gekinmon_GetWindowHeight(lua_State * L);
static int gekinmon_GetNPCCount(lua_State * L);
static int gekinmon_Log(lua_State * L);
static int gekinmon_Register(lua_State * L);
static int gekinmon_RegisterEvent(lua_State * L);

static int gekinmon_Error(lua_State * L);

static int gekinmon_GetDialog(lua_State * L);
static int gekinmon_GetDialogOption(lua_State * L);
static int gekinmon_GetDialogOptionCount(lua_State * L);
static int gekinmon_SetDialogOptionID(lua_State * L);

static int gekinmon_GetGameMode(lua_State * L);

static int gekinmon_LoadScript(lua_State * L);
static int gekinmon_UnloadScript(lua_State * L);

static int gekinmon_GetAllyHP(lua_State * L);
static int gekinmon_GetEnemyHP(lua_State * L);
static int gekinmon_GetAllyCount(lua_State * L);
static int gekinmon_GetEnemyCount(lua_State * L);
static int gekinmon_Attack(lua_State * L);

/** Stores the "Gekinmon" library for the Lua scripts. */
extern const struct luaL_reg gekinmon_lua_api [];
#endif
