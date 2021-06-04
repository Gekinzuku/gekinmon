#ifndef NPC_H
#define NPC_H
#include <vector>
#include <string>
using namespace std;

#include "object.h"

#ifdef WIN32
#define uint unsigned int
#endif

/** These two structs may seem redundant at the moment. 
  * This will allow a bit of customization for the two later. **/
struct OptionStruct {
    string Title;
    string JumpID;
};

struct MessageStruct {
    string ID;
    string Message;
    vector <OptionStruct> Option;
    bool End, Battle;
};

struct DialogueStruct {
    vector <MessageStruct> Message;
};

class NPCClass: public ObjectClass
{
public:
    NPCClass() { Direction = DIRE_UP; Moving = false; VecY = VecX = 75; 
                 AnimFrame = 0; }
    void AddDialogue(DialogueStruct Dialogue_);
    string GetString(unsigned int DNum, unsigned int BNum);
    OptionStruct GetOption(uint DNum, uint BNum, uint Option);

    int GetDialogueNum();
    int IDToBox(uint DNum, string ID);
    bool IsEndMessage(uint DNum, uint BNum);
    int GetDialogueSize(uint DNum) { return Dialogue[DNum].Message.size(); }
    int GetOptionCount(uint DNum, uint BNum);
    void StartBattle(uint DNum, uint BNum);

    void Move(Uint32 DeltaTicks);
    void SetDire(int Dire);
    void SetMoving();
    void UpdateSprite() {
        Sprite[SpriteNum].SetLoc(X + Area.GetX(), Y + Area.GetY()); }
    float GetScreenX() { return X + Area.GetX(); }
    float GetScreenY() { return Y + Area.GetY(); }

    void SetID(const char * IDstr) { ID = IDstr; }
    string GetID() { return ID; }
private:
    string ID;
    vector <DialogueStruct> Dialogue;
};

extern vector<NPCClass> NPC;
int AddNPC(int SpriteNum);
int FindNPC(int X, int Y);
int FindNPC(int X, int Y, int Z);
int FindNPCAbs(int X, int Y, int Z);
void MoveAllNPCs(float X, float Y);
void MoveAllNPCsX(float X);
void MoveAllNPCsY(float Y);
void DeleteNPC(unsigned int NPCNum);
void DeleteAllNPCs();
void SwapPlayerOrder();
#endif
