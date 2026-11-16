#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"

namespace fgobj {
// A field object that stays for a number of frames, optionally with a shadow that fades in (the class name is from the
// RTTI, the rest is ours; the Proc builds it after a cut tree, see 0x0059B8D8).
// RTTI N5fgobj11ObjectTimerE @ 0x008D29B8
// vtable 0x00908C54 (vptr 0x00908C5C), offset_to_top 0, 10 entries
class ObjectTimer : public ::fgobj::ObjectExe
{
public:
    virtual ~ObjectTimer(); // 0x00594D5C slot 0x00
    // 0x00594D2C (deleting dtor)
    bool vf_0x10() override; // 0x00594C4C slot 0x10
    s32 Execute() override; // 0x00594C54 slot 0x20

    u8 m_Timer;         // 0x124 the frames that are left
    u8 m_Frame;         // 0x125
    bool m_HasShadow;   // 0x126 the shadow fades in (frames count twice)
};
ASSERT_SIZE(ObjectTimer, 0x128);
} // namespace fgobj
