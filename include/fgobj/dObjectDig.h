#pragma once

#include "decomp.h"
#include "bgcheck/dDynamicCylinder.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// A hole that is dug: its cylinder in the collision world grows in 4 frames (the class name is from the RTTI, the rest is
// ours; the Proc builds it, see 0x0059A9D0).
// RTTI N5fgobj9ObjectDigE @ 0x008D2A44
// vtable 0x00908E4C (vptr 0x00908E54), offset_to_top 0, 10 entries
class ObjectDig : public ::fgobj::ObjectExe
{
public:
    virtual ~ObjectDig(); // 0x005A35F8 slot 0x00
    // 0x005A35B4 (deleting dtor)
    // the cylinder leaves the collision world, then ObjectBase::Finalize
    void Finalize() override; // 0x005A3544 slot 0x14
    s32 Execute() override; // 0x005A3560 slot 0x20

    bgcheck::DynamicCylinder m_Cylinder;    // 0x124
    u8 m_Frame;                             // 0x150
};
ASSERT_OFFSET(ObjectDig, m_Frame, 0x150);
ASSERT_SIZE(ObjectDig, 0x154);
} // namespace fgobj
