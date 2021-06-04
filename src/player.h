#ifndef PLAYER_H
#define PLAYER_H

#include "object.h"


class PlayerClass: public ObjectClass {
public:
    PlayerClass() { Direction = DIRE_UP; Moving = false; VecY = VecX = 100; 
                    AnimFrame = 0; }

    void Move(Uint32 DeltaTicks);

    void SetDire(int Dire);
    int GetDire() { return Direction; }
    void SetMoving();

private:
};


extern PlayerClass Player;
#endif
