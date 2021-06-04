#include "battle.h"

BattleClass Battle;

BattleClass::BattleClass()
{
    // Temporary hardcoded battle
    // Enemy and Ally have the same stats. :P
    Character Tmp;
    vector <Character> AllyTmp, EnemyTmp;
    Tmp.HP = 20;
    Tmp.Level = 5;
    Tmp.Strength = 10;
    Tmp.Attack = 13;
    Tmp.Front = true;
    Tmp.Defend = false;
    Tmp.Defense = 10;
    Tmp.MagicEvade = 1;
    Tmp.Evade = 10;
    Tmp.Accuracy = 30;

    AllyTmp.push_back(Tmp);
    EnemyTmp.push_back(Tmp);
    InitBattle(AllyTmp, EnemyTmp);
}

/** Loads in the Allies and Enemies for a battle */
bool BattleClass::InitBattle(vector <Character> AllyTmp, vector <Character> EnemyTmp)
{
    Ally = AllyTmp;
    Enemy = EnemyTmp;
}

/** Returns the HP of a specific ally, assuming it exits */
int BattleClass::GetAllyHP(unsigned short AllyID)
{
    if (AllyID < Ally.size())
        return Ally[AllyID].HP;
    return -1;
}

/** Returns the HP of a specific enemy, assuming it exists. */
int BattleClass::GetEnemyHP(unsigned short EnemyID)
{
    if (EnemyID < Enemy.size())
        return Enemy[EnemyID].HP;
    return -1;
}

/** Returns the number of allies in this battle */
unsigned int BattleClass::GetAllyCount()
{
    return Ally.size();
}

/** Returns the number of enemies in this battle */
unsigned int BattleClass::GetEnemyCount()
{
    return Enemy.size();
}

void BattleClass::Attack(bool AttackAlly, unsigned int AttackerID, bool TargetAlly, unsigned int TargetID)
{
    bool Magic = true; // No magic attacks yet
    Character Attacker, Target;

    // Puts the stats of the proper attacker/target into a temp character
    if (AttackAlly) Attacker = Ally[AttackerID];
    else Attacker = Enemy[AttackerID];
    
    if (TargetAlly) Target = Ally[TargetID];
    else Target = Ally[TargetID];

    // Calculate if the attack should hit
    int Block = (Magic) ? Target.MagicEvade : Target.Evade;

    int BlockValue = (255 - Block * 2) + 1; 
    if (BlockValue > 255) BlockValue = 255;
    else if (BlockValue < 1) BlockValue = 1;

    if ((Attacker.Accuracy * BlockValue)/256 > rand()%99) {
        printf("Attack missed!\n");
        return;
    }

    // Perform the actual battle calculations with our temp character
    int Attack = Attacker.Attack * 2 + Attacker.Strength;
    int Damage = Attacker.Strength + (((Attacker.Level * Attacker.Level * Attack)/256) * 3)/2;
    if (!Attacker.Front) Damage /= 2;

    // Special multipliers
    int DamageMul = 0;
    if (rand()%32 == 0) { printf("Critical hit\n"); DamageMul += 2; }
    Damage = Damage + ((Damage/2)*DamageMul);
    
    // Apply a little randomness to the damage
    Damage = (Damage * (rand()%31 + 224)/256) + 1;

    // Take target's stats into consideration
    Damage = (Damage * (255 - Target.Defense)/256) + 1;
    if (!Target.Front) Damage /= 2;
    if (!Target.Defend) Damage /= 2;

    Target.HP -= Damage;

    // Push the updated stats back to the actual Attacker/Target
    if (TargetAlly) Ally[TargetID] = Target;
    else Enemy[TargetID] = Target;
}

