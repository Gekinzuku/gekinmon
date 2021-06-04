#include <iostream>
#include <sstream>
#include "xml/tinystr.h"
#include "xml/tinyxml.h"

#include "embed.h"
#include "lua-api.h"

Embed EmbededLua;

vector <Gekinmon_Plugin> Plugin;

Embed::Embed()
{
}

int Embed::LoadFromXML(const char * XMLFileName, const char * BasePath)
{
    TiXmlDocument Doc(XMLFileName);
    if(!Doc.LoadFile()) return -1;
    TiXmlHandle hDoc(&Doc);

    TiXmlElement * Root = hDoc.FirstChildElement().Element();
    if (!Root) return -1;
    TiXmlHandle hRoot(0);
    hRoot = TiXmlHandle(Root);

    for (TiXmlElement * hElement = hRoot.FirstChild("Script").Element(); hElement;
        hElement = hElement->NextSiblingElement()) {
        string FileName = BasePath;
        FileName += hElement->GetText();
        Load(FileName.c_str());
    }
}

bool Embed::Load(const char * FileName)
{
    stringstream Output;
    Output << "Loading plugin \"" << FileName << "\".";
    Log(LOG_MESSAGE, Output.str());

    int PluginCount = Plugin.size();
    lua_State * Inter;
    Inter = lua_open();

    luaL_openlibs(Inter);
    luaL_openlib(Inter, "gekinmon", gekinmon_lua_api, 0);

    luaL_loadfile(Inter, FileName);
    if(!SafeCall(Inter)) {
        if (PluginCount == Plugin.size()) {
            Log(LOG_ERROR, "Plugin encountered an error before registering, unloading...\n");
            lua_close(Inter);
        }
        else Unload(Plugin.size()-1);
        return false;
    }

    /** If the plugin is already loaded we should probably kill it! */
    for (int i = 0; i < Plugin.size()-1; ++i) {
        if (Plugin[i].Name == Plugin[Plugin.size()-1].Name)
        {
            Log(LOG_ERROR, "Plugin is already loaded, unloading...\n");
            Plugin.pop_back();
            lua_close(Inter);
            return false;
        }
    }

    /** If the plugin list didn't grow then we didn't register a new one. */
    if (PluginCount == Plugin.size()) {
        Log(LOG_ERROR, "Plugin did not register, unloading...\n");
        lua_close(Inter);
        return false;
    }

    return true;
}

void Embed::Unload(unsigned int PluginNum)
{
    stringstream Output;
    Output << "Unloading plugin \"" << Plugin[PluginNum].Name << "\".";
    Log(LOG_MESSAGE, Output.str());
    lua_close(Plugin[PluginNum].Interpreter);
    Plugin.erase(Plugin.begin()+PluginNum);
}

/** Logs messages, this will probably be improved later. */
void Embed::Log(short Type, string Message)
{
    cout << "Script: ";
    switch (Type)
    {
    case LOG_ERROR:
        cout << "[ERROR] " << Message << endl;
        break;
    case LOG_WARNING:
        cout << "[WARNING] " << Message << endl;
        break;
    case LOG_MESSAGE:
        cout << Message << endl;
        break;
    }
}

int Embed::GetPluginFromInter(lua_State * L)
{
    for (unsigned int i = 0; i < Plugin.size(); ++i) {
        if (Plugin[i].Interpreter == L) return i;
    }
    return -1;
}

/** Runs all plugins that requested to be informed when a key is pressed. */
bool Embed::Call_KeyFunc(int Key)
{
    bool Allow = true;
    for (int i = 0; i < Plugin.size(); ++i) {
        if (Plugin[i].Event_KeyPress == "") continue;
        lua_getglobal(Plugin[i].Interpreter, Plugin[i].Event_KeyPress.c_str());
        lua_pushnumber(Plugin[i].Interpreter, Key);
        if (!SafeCall(Plugin[i].Interpreter, 1, 1, 0))
            Unload(i);
        // If one of the scripts returns false then we need to store that value
        if (lua_isboolean(Plugin[i].Interpreter, -1)) {
            bool Val = lua_toboolean(Plugin[i].Interpreter, -1);
            lua_pop(Plugin[i].Interpreter, 1);
            if (Allow == true) Allow = Val;
        }
    }
    return Allow;
}

/** Runs all plugins that requested to be informed when the player moves */
void Embed::Call_PlayerMove()
{
    for (int i = 0; i < Plugin.size(); ++i) {
        if (Plugin[i].Event_PlayerMove == "") continue;
        lua_getglobal(Plugin[i].Interpreter, Plugin[i].Event_PlayerMove.c_str());
        if (!SafeCall(Plugin[i].Interpreter))
            Unload(i);
    }
}

/** Runs all plugins that requested to be informed on new dialog events */
void Embed::Call_NewDialog(int NPCNum, int DNum, int BNum)
{
    for (int i = 0; i < Plugin.size(); ++i) {
        if (Plugin[i].Event_NewDialog == "") continue;
        lua_getglobal(Plugin[i].Interpreter, Plugin[i].Event_NewDialog.c_str());
        lua_pushnumber(Plugin[i].Interpreter, NPCNum);
        lua_pushnumber(Plugin[i].Interpreter, DNum);
        lua_pushnumber(Plugin[i].Interpreter, BNum);
        if (!SafeCall(Plugin[i].Interpreter, 3, 0, 0))
            Unload(i);
    }
}

/** Runs all plugins that requested to be informed when dialog events end */
void Embed::Call_EndDialog(int NPCNum, int DNum)
{
    for (int i = 0; i < Plugin.size(); ++i) {
        if (Plugin[i].Event_NewDialog == "") continue;
        lua_getglobal(Plugin[i].Interpreter, Plugin[i].Event_EndDialog.c_str());
        lua_pushnumber(Plugin[i].Interpreter, NPCNum);
        lua_pushnumber(Plugin[i].Interpreter, DNum);
        if (!SafeCall(Plugin[i].Interpreter, 2, 0, 0))
            Unload(i);
    }
}

/** A convience function for safely calling functions and log errors from C++ */
bool Embed::SafeCall(lua_State * L)
{
    if (lua_pcall(L, 0, 0, 0) != 0) {
        EmbededLua.Log(LOG_ERROR, lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

/** A convience function for safely calling functions and log errors from C++ 
    Allows for setting number of args and results. */
bool Embed::SafeCall(lua_State * L, int nargs, int nresults, int errfunc)
{
    if (lua_pcall(L, nargs, nresults, errfunc) != 0) {
        EmbededLua.Log(LOG_ERROR, lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

