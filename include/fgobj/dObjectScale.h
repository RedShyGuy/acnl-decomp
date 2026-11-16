#pragma once

#include "decomp.h"
#include "bgcheck/dDynamicCylinder.h"
#include "fgobj/dObjectExe.h"

class SvFgName;

namespace fgobj {
// A field object that grows from a tenth to its size in 9 frames; a hole gets a cylinder in the collision world that
// grows with it (the class name is from the RTTI, the rest is ours).
// RTTI N5fgobj11ObjectScaleE @ 0x008D2988
// vtable 0x00908B94 (vptr 0x00908B9C), offset_to_top 0, 10 entries
class ObjectScale : public ::fgobj::ObjectExe
{
public:
    // the object of item at the unit x, z (keepRequest: EXECUTE_FINISHED_KEEP_REQUEST at the end)
    static ObjectScale* Create(const SvFgName& item, s32 x, s32 z, bool keepRequest); // 0x00592EE4

    virtual ~ObjectScale(); // 0x00593138 slot 0x00
    // 0x005930F4 (deleting dtor)
    // the cylinder of a hole leaves the collision world, then ObjectBase::Finalize
    void Finalize() override; // 0x00591594 slot 0x14
    s32 Execute() override; // 0x0059302C slot 0x20

    bgcheck::DynamicCylinder m_Cylinder;    // 0x124 (holes)
    u8 m_Frame;                             // 0x150
    bool m_KeepRequest;                     // 0x151
};
ASSERT_OFFSET(ObjectScale, m_Frame, 0x150);
ASSERT_SIZE(ObjectScale, 0x154);
} // namespace fgobj
