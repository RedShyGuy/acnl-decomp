#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dArcResourceAccessorVRAM.h"
#include "ssys/ma/lyt/dResAccInterface.h"

namespace ssys {
namespace ma {
namespace lyt {
// the resources of an archive in memory (names after libgarden)
// RTTI N4ssys2ma3lyt9ArcResAccE @ 0x008D24B4
// vtable 0x00907348 (vptr 0x00907350), offset_to_top 0, 2 entries
class ArcResAcc : public ::ssys::ma::lyt::ResAccInterface
{
public:
    ArcResAcc(); // 0x0056A5A0
    virtual ~ArcResAcc(); // 0x0056A610 slot 0x00

    // false if an archive is set already or it is not valid
    DECOMP_EMIT_COPY bool SetData(const void* arcData, const char* rootName) // 0x0056A580 | libgarden [tier A]
    {
        if (m_pData != NULL) {
            return false;
        }
        m_pData = arcData;
        return m_ArcResourceAccessor.Attach(const_cast<void*>(arcData), rootName);
    }
    ArcResourceAccessorVRAM* GetResourceAccessorVRAM() { return &m_ArcResourceAccessor; }
    // loads all textures (the .bclim files of the timg directory); false if there is no such directory (name is ours)
    bool LoadTextures(); // 0x0056A3C4

    ArcResourceAccessorVRAM m_ArcResourceAccessor; // 0x0C
};
ASSERT_SIZE(ArcResAcc, 0x158);
} // namespace lyt
} // namespace ma
} // namespace ssys
