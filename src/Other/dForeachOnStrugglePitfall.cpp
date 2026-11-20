#include "Other/dForeachOnStrugglePitfall.h"
#include "Ac/dAcNpc.h"

// 0x0033DDCC slot 0x00
void ForeachOnStrugglePitfall::Invoke(AcNpc* npc)
{
    npc->OnStrugglePitfall();
}
