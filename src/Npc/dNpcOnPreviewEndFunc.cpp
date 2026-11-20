#include "Npc/dNpcOnPreviewEndFunc.h"
#include "Ac/dAcNpc.h"

// 0x002FA634 slot 0x00
void NpcOnPreviewEndFunc::Invoke(AcNpc* npc)
{
    npc->OnPreviewEnd();
}
