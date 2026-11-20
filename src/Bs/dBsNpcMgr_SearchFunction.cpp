#include "Bs/dBsNpcMgr_SearchFunction.h"
#include "Ac/dAcNpc.h"
#include "Bs/dBsNpcMgrExtern.h"
#include "stage/stage_Api.h"

// 0x0069670C slot 0x00
void BsNpcMgr::SearchFunction::Invoke(AcNpc* npc)
{
    if (m_pResult != NULL) {
        return;
    }
    const stage::NpcInfo& own = npc->m_Info;
    if (own.kind != m_pInfo->kind) {
        return;
    }
    switch (m_pInfo->kind) {
    case stage::NpcInfo::KIND_ID:
        if (static_cast<u16>(own.value) != static_cast<u16>(m_pInfo->value)) {
            return;
        }
        break;
    case stage::NpcInfo::KIND_MODEL: {
        u32 a = own.value;
        u32 b = m_pInfo->value;
        if (!IsEqualString(reinterpret_cast<const char*>(&a), reinterpret_cast<const char*>(&b))) {
            return;
        }
        break;
    }
    case stage::NpcInfo::KIND_VALUE:
        if (own.value != m_pInfo->value) {
            return;
        }
        break;
    default:
        return;
    }
    m_pResult = npc;
}
