#include "Npc/dNpcOnPreviewFunc.h"
#include "Ac/dAcNpc.h"

// 0x002BB598 slot 0x00
void NpcOnPreviewFunc::Invoke(AcNpc* npc)
{
    npc->OnPreview(m_X0, m_Z0, m_X1, m_Z1, m_Flag);
}
