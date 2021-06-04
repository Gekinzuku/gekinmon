#ifndef QUEST_H
#define QUEST_H
#include <map>
#include <vector>
#include <string>
#include "xml/tinystr.h"
#include "xml/tinyxml.h"

using namespace std;

enum { ACTION_TALK };
enum { REWARD_OBJ_START, REWARD_OBJ_FAIL, REWARD_MOD_ON, REWARD_MOD_OFF, 
       REWARD_QUEST_COMPLETE };

struct Prize {
    unsigned short Type;
    short Amount;
    string ID;
};

struct Objective {
    bool Visible, Complete;
    string ID;
    string Description;
    /** Related to the completion of the quest **/
    unsigned short ActionType;
    string ActionArea;
    string ActionID;
    /** Reward items **/
    vector <Prize> Reward;
};

class QuestClass {
public:
    bool Load(const char * FileName);
    void CheckNPC(string AreaName, string NPCName);
    void RunRewards(string ObjID);
private:
    string Title, Description;
    map <string, Objective> Objectives;
    vector <string> ActiveObj;
};

extern QuestClass Quest;
#endif
