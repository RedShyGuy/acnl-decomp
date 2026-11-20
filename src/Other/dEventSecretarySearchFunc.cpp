#include "Other/dEventSecretarySearchFunc.h"
#include "Ac/dAcNpc.h"
#include "event/event_Api.h"
#include "event/event_Extern.h"
#include "Other/dProcMgr.h"

// 0x0033DB6C slot 0x00
void EventSecretarySearchFunc::Invoke(AcNpc* npc)
{
    if (m_IsFound == true || npc == NULL) {
        return;
    }
    if (npc->m_Index == PROC_AC_NPC_SP_SECRETARY_GP_EVENT) {
        if (event::Func308E74() != event::EVENT_ID_NONE) {
            return;
        }
        if (npc->m_UnknownA8 == 27) {
            return;
        }
        m_IsFound = true;
    } else if (npc->m_Index == PROC_AC_NPC_SP_SECRETARY_EVENT) {
        if (npc->m_UnknownA8 == 27) {
            return;
        }
        m_IsFound = true;
    }
}
