#include "Bs/dBsNpcMgr_NpcPosGetCB.h"
#include "Ac/dAcNpc.h"

// 0x006960A8
BsNpcMgr::NpcPosGetCB::NpcPosGetCB(AcNpc* exclude) : m_pExclude(exclude), m_Num(0)
{
}

// 0x006960E4 slot 0x00
void BsNpcMgr::NpcPosGetCB::Invoke(AcNpc* npc)
{
    if (npc != m_pExclude) {
        m_Positions[m_Num] = npc->m_Position;
        m_Num++;
    }
}
