#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

class SvFgName;

namespace fgobj {
// A field object that is planted: it springs up in 15 frames while its shadow appears (the class name is from the RTTI,
// the rest is ours).
// RTTI N5fgobj11ObjectPlantE @ 0x008D297C
// vtable 0x00908B64 (vptr 0x00908B6C), offset_to_top 0, 10 entries
class ObjectPlant : public ::fgobj::ObjectExe
{
public:
    // the object of item at the unit x, z (playSound: SE_OBJ_PLANT_CMN_GROW)
    static ObjectPlant* Create(const SvFgName& item, s32 x, s32 z, bool playSound); // 0x00592CC8

    virtual ~ObjectPlant(); // 0x00592EB8 slot 0x00
    // 0x00592E88 (deleting dtor)
    s32 Execute() override; // 0x00592DB8 slot 0x20

    u8 m_Frame; // 0x124
};
ASSERT_SIZE(ObjectPlant, 0x128);
} // namespace fgobj
