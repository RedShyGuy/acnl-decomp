#include "Npc/dNpcOnBuildFunc.h"
#include "Ac/dAcNpc.h"

// 0x00270AC4 slot 0x00
void NpcOnBuildFunc::Invoke(AcNpc* npc)
{
    npc->OnBuild(m_X0, m_Z0, m_X1, m_Z1, m_Flag);
}
