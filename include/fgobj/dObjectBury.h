#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// An item that is buried: it shrinks into the ground (the class name is from the RTTI, the rest is ours; the Proc
// builds it, see 0x0059ADC0).
// RTTI N5fgobj10ObjectBuryE @ 0x008D291C
// vtable 0x00908A64 (vptr 0x00908A6C), offset_to_top 0, 10 entries
class ObjectBury : public ::fgobj::ObjectExe
{
public:
    // m_Animation
    enum Animation : u8
    {
        ANIMATION_SHRINK = 0,       // shrinks in 9 frames
        ANIMATION_PRESS = 1,        // pressed to 0.8 and held
        ANIMATION_PRESS_DEEP = 2,   // pressed to 0.65, held and shrinks to nothing
    };
    // m_FinishMode: what happens to the place request of the unit at the end
    enum FinishMode : u8
    {
        FINISH_MODE_END = 0,            // EXECUTE_FINISHED
        FINISH_MODE_KEEP = 1,           // EXECUTE_FINISHED_KEEP_REQUEST
        FINISH_MODE_TAKE_UP = 2,        // PlaceBuryResult, then EXECUTE_FINISHED_KEEP_REQUEST
    };

    virtual ~ObjectBury(); // 0x00591DEC slot 0x00
    // 0x00591DBC (deleting dtor)
    s32 Execute() override; // 0x00591BCC slot 0x20

    u8 m_Frame;         // 0x124
    u8 m_FinishMode;    // 0x125 FinishMode
    u8 m_Animation;     // 0x126 Animation
};
ASSERT_SIZE(ObjectBury, 0x128);
} // namespace fgobj
