#include <vector>
using namespace std;

struct Character {
    int HP;
    int Attack, Defense, Strength, Level;
    bool Front, Defend;

    int Spell, Magic, MDefense;
    bool MultiTarget;

    int Evade, MagicEvade, Accuracy;
};

class BattleClass {
public:
    BattleClass();

    bool InitBattle(vector <Character> AllyTmp, vector <Character> EnemyTmp);

    int GetAllyHP(unsigned short AllyID);
    int GetEnemyHP(unsigned short EnemyID);
    unsigned int GetAllyCount();
    unsigned int GetEnemyCount();

    void Attack(bool AttackAlly, unsigned int AttackerID, bool TargetAlly, unsigned int TargetID);
private:
    vector <Character> Ally, Enemy;
};

extern BattleClass Battle;

