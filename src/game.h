
class SDLGame {
public:
    SDLGame() { Mode = 'g'; DNum = BNum = NPCNum = -1; }
    void Execute();
    void OpenMenu();

    void HandleKeyUp(int Key);
    void HandleKeyDown(int Key);

    void OpenDialogue(int NPCNum);
    void CloseDialogue();

    void StartBattle();

    void SetOptionID(int OptionID_);

    char GetMode() { return Mode; };
    void SetMode(char Mode_) { Mode = Mode_; } 
private:
    char Mode;
    int DNum, BNum, NPCNum;
    int OptionID;
};


extern SDLGame Game;
extern Uint32 Ticks;
