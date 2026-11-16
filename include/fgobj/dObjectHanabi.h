#pragma once

#include "decomp.h"
#include "Other/dSeadParticleHandle.h"
#include "fgobj/dObjectThrow.h"

namespace fgobj {
// A fountain firework (SvFgName::NAME_FOUNTAIN_FIREWORK) that is put down: it is thrown onto its unit, waits, then burns
// with the effect 530 for 300 frames (the class name is from the RTTI, the rest is ours; the Proc builds it, see
// 0x0059BB68).
// RTTI N5fgobj12ObjectHanabiE @ 0x008D29D0
// vtable 0x00908C94 (vptr 0x00908C9C), offset_to_top 0, 10 entries
class ObjectHanabi : public ::fgobj::ObjectThrow
{
public:
    // m_State
    enum State : u8
    {
        STATE_THROW = 0,
        STATE_WAIT = 1,
        STATE_BURN = 2,
    };

    virtual ~ObjectHanabi(); // 0x00595284 slot 0x00
    // 0x00595240 (deleting dtor)
    // kills the effect, ends the place request of the unit while it holds it, then ObjectBase::Finalize
    void Finalize() override; // 0x00595010 slot 0x14
    s32 Execute() override; // 0x00595068 slot 0x20

    SeadParticleHandle m_Effect;    // 0x148
    u8 m_State;                     // 0x160 State
    bool m_HoldsRequest;            // 0x161 the place request of the unit ends with the object (Finalize)
};
ASSERT_SIZE(ObjectHanabi, 0x164);
} // namespace fgobj
