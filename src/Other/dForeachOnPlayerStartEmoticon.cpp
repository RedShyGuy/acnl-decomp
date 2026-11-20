#include "Other/dForeachOnPlayerStartEmoticon.h"
#include "Ac/dAcNpc.h"

// 0x00343974 slot 0x00
void ForeachOnPlayerStartEmoticon::Invoke(AcNpc* npc)
{
    npc->OnPlayerStartEmoticon(m_Emoticon);
}
