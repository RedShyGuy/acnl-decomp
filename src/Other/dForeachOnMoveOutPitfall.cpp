#include "Other/dForeachOnMoveOutPitfall.h"
#include "Ac/dAcNpc.h"

// 0x003353EC slot 0x00
void ForeachOnMoveOutPitfall::Invoke(AcNpc* npc)
{
    npc->OnMoveOutPitfall();
}
