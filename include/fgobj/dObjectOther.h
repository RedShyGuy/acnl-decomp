#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"
#include "nn/math/math_Vector3.h"

class SvFgName;

namespace fgobj {
// An item shown at a position without a unit while someone keeps it there: it ends when it was not refreshed in the
// last frame (the class name is from the RTTI, the rest is ours; built by 0x0059B598).
// RTTI N5fgobj11ObjectOtherE @ 0x008D2970
// vtable 0x00908B34 (vptr 0x00908B3C), offset_to_top 0, 10 entries
class ObjectOther : public ::fgobj::ObjectExe
{
public:
    // the item at position with scale (faceCameraIndoors: turned to the camera indoors, else rotateY; hasShadow: with a
    // shadow of shadowSize; withFlag40000: FLAG_UNK_40000)
    static ObjectOther* Create(const SvFgName& item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id, f32 shadowSize,
                               bool faceCameraIndoors, bool hasShadow, s16 rotateY, bool withFlag40000); // 0x00592AD8

    virtual ~ObjectOther(); // 0x00592C9C slot 0x00
    // 0x00592C6C (deleting dtor)
    s32 Execute() override; // 0x00592C38 slot 0x20
    bool Refresh(const SvFgName* item, const nn::math::VEC3* position, const nn::math::VEC3* scale, s32 id) override; // 0x00592A70 slot 0x24

    s32 m_Id;       // 0x124 (of the one who shows it)
    s32 m_Refresh;  // 0x128 1 after Refresh, cleared by Execute
};
ASSERT_SIZE(ObjectOther, 0x12C);
} // namespace fgobj
