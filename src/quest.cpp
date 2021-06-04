#include <iostream>
#include "quest.h"
    
QuestClass Quest;

bool QuestClass::Load(const char * FileName)
{
    // First, initlize the XML stuff
    TiXmlDocument Doc(FileName);
    if (!Doc.LoadFile()) return false;
    TiXmlHandle hDoc(&Doc);
    TiXmlElement * Root = hDoc.FirstChildElement().Element();
    if (!Root) return false;
    TiXmlHandle hRoot(0);
    hRoot = TiXmlHandle(Root);

    if (hRoot.FirstChild("Title").Element())
        Title = hRoot.FirstChild("Title").Element()->GetText();
    if (hRoot.FirstChild("Description").Element())
        Description = hRoot.FirstChild("Description").Element()->GetText();
    // Load the objectives
    for (TiXmlElement * Obj = hRoot.FirstChild("Objective").Element(); Obj;
            Obj = Obj->NextSiblingElement("Objective")) {
        Objective TmpObj;
        TmpObj.Description = Obj->FirstChildElement("Description")->GetText();

        const char * Visible = Obj->FirstChildElement("Visible")->GetText(); 
        if (strcmp(Visible, "Yes") == 0) TmpObj.Visible = true;
        else if (strcmp(Visible, "No") == 0) TmpObj.Visible = false;
        else return false;

        TmpObj.ID = Obj->Attribute("id");

        /** Load in the action needed to complete this Objective **/
        if (!Obj->FirstChildElement("Action")) return false;
        const char * Action = Obj->FirstChildElement("Action")->GetText();
        if (strcmp(Action, "Talk") == 0) TmpObj.ActionType = ACTION_TALK;
        else return false;

        if (Obj->FirstChildElement("Action")->Attribute("area") == NULL ||
                Obj->FirstChildElement("Action")->Attribute("id") == NULL) 
            return false;
        TmpObj.ActionArea =Obj->FirstChildElement("Action")->Attribute("area"); 
        TmpObj.ActionID   =Obj->FirstChildElement("Action")->Attribute("id");

        /** Add the rewards for completing an objective **/
        if (!Obj->FirstChild("Completion")) return false;
        for (TiXmlElement * Rwd = 
                Obj->FirstChild("Completion")->FirstChildElement(); Rwd; 
                Rwd = Rwd->NextSiblingElement())
        {
            Prize TmpRwd;
            const char * Value = Rwd->Value();
            cout << Value << " " << Action << endl;
            if (!Value) return false;
            if (strcmp(Value, "Objective") == 0) {
                const char * Action = Rwd->Attribute("action");
                if (!Action) return false;
                if (strcmp(Action, "start") == 0) {
                    TmpRwd.Type = REWARD_OBJ_START;
                    TmpRwd.ID = Rwd->GetText();
                }
            }
            else if (strcmp(Value, "Quest") == 0) {
                const char * Type = Rwd->GetText(); 
                if (!Type) return false; 
                if (strcmp(Type, "Complete") == 0) {
                    TmpRwd.Type = REWARD_QUEST_COMPLETE;
                }
            }
            else { 
                TmpRwd.Type = 255;
            }
            TmpObj.Reward.push_back(TmpRwd);
        }

        const char * ID = Obj->Attribute("id");
        Objectives[ID] = TmpObj;
    }
    ActiveObj.push_back("001");     
    return true;
}

void QuestClass::CheckNPC(string AreaName, string NPCName)
{
    for (unsigned int i = 0; i < ActiveObj.size(); ++i) {
        cout << "A: " << i << endl;
        const Objective Obj = Objectives[ActiveObj[i]];
        if (Obj.ActionType == ACTION_TALK && Obj.ActionArea == AreaName 
            && Obj.ActionID == NPCName) {
//          cout << "Success! Moving on to: " << Obj.Reward[0].ID << endl;
            cout << "Yay!" << endl;
            RunRewards(ActiveObj[i]);
            ActiveObj.erase(ActiveObj.begin()+i);
            break;
        }
    }
}

void QuestClass::RunRewards(string ObjID)
{
    for (unsigned int i = 0; i < Objectives[ObjID].Reward.size(); ++i) {
        switch (Objectives[ObjID].Reward[i].Type)
        { 
        case REWARD_OBJ_START:
            ActiveObj.push_back(Objectives[ObjID].Reward[i].ID);
            break;
        case REWARD_MOD_ON:
            break;
        case REWARD_QUEST_COMPLETE:
            cout << "You beat this quest, now fuck off.\n";
            break;
        }
    }
}

