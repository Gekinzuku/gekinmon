#ifndef EMBED_H
#define EMBED_H

#include <vector>

using namespace std;

enum {LOG_ERROR, LOG_WARNING, LOG_MESSAGE };

#define BASE_PATH "Data/scripts/"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

struct Gekinmon_Plugin {
    string Name, Author, Version, License, Description;
    lua_State * Interpreter;
    // Events
    string Event_KeyPress, Event_PlayerMove, Event_NewDialog, Event_EndDialog;
};

class Embed {
public:
    Embed();
    int LoadFromXML(const char * XMLFileName, const char * BasePath);
    bool Load(const char * FileName);
    void Unload(unsigned int PluginNum);
    void Log(short Type, string Message);

    int GetPluginFromInter(lua_State * L);
    bool Call_KeyFunc(int Key);
    void Call_PlayerMove();
    void Call_NewDialog(int NPCNum, int DNum, int BNum);
    void Call_EndDialog(int NPCNum, int DNum);
private:
    bool SafeCall(lua_State * L);
    bool SafeCall(lua_State * L, int nargs, int nresults, int errfunc);
};

extern Embed EmbededLua;
extern vector <Gekinmon_Plugin> Plugin;


#endif
