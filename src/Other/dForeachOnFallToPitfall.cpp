#include "Other/dForeachOnFallToPitfall.h"
#include "Ac/dAcNpc.h"

// 0x003300A8 slot 0x00
void ForeachOnFallToPitfall::Invoke(AcNpc* npc)
{
    npc->OnFallToPitfall();
}
