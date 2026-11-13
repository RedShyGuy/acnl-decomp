#include "svfscnve/dUomasaForeachFunction.h"

#include "Sv/dSvInsectConventionExtern.h"

namespace {
// (the fields of an NPC actor that the Fishing Tourney reads; AcNpc has no layout yet)
DECOMP_ALWAYS_INLINE u8 GetActorByte9A8(const AcNpc* npc)
{
    return reinterpret_cast<const u8*>(npc)[0x9A8];
}

DECOMP_ALWAYS_INLINE const void* GetActorObject868(const AcNpc* npc)
{
    return reinterpret_cast<const u8*>(npc) + 0x868;
}
} // namespace

namespace svfscnve {
// 0x006CD70C slot 0x00 (name is ours)
void svfscnve::UomasaForeachFunction::Invoke(AcNpc* npc)
{
    if (npc != NULL && GetActorByte9A8(npc) == 0 && Func753588(GetActorObject868(npc))) {
        m_IsMarked[m_Num] = true;
    } else {
        m_IsMarked[m_Num] = false;
    }
    m_Num++;
}
} // namespace svfscnve
