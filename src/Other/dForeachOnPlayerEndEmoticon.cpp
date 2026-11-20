#include "Other/dForeachOnPlayerEndEmoticon.h"
#include "Ac/dAcNpc.h"

// 0x00342B50 slot 0x00
void ForeachOnPlayerEndEmoticon::Invoke(AcNpc* npc)
{
    npc->OnPlayerEndEmoticon(m_Emoticon);
}
