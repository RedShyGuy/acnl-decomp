#pragma once

#include "decomp.h"

namespace ssys {
namespace co {
// a fade of the screen over a number of frames: covered -> fade in -> clear -> fade out -> covered (function and member
// names are ours)
// RTTI N4ssys2co9FaderBaseE @ 0x008D23F0
// vtable 0x00907154 (vptr 0x0090715C), offset_to_top 0, 8 entries
class FaderBase
{
public:
    enum Status : u8
    {
        STATUS_COVERED = 0,
        STATUS_CLEAR = 1,
        STATUS_FADE_IN = 2,
        STATUS_FADE_OUT = 3,
    };
    // the bits of m_Flags
    enum Flag : u8
    {
        FLAG_FADE_IN_DONE = 1 << 0,
        FLAG_FADE_OUT_DONE = 1 << 1,
    };

    FaderBase(u8 arg, s32 status); // 0x00566FC0
    virtual ~FaderBase(); // 0x0056700C slot 0x00
    // only covered or clear
    virtual void SetStatus(s32 status); // 0x00566FB0 slot 0x08
    virtual u8 GetStatus() const { return m_Status; } // 0x00750B00 slot 0x0C
    // false if it is not covered (clear)
    virtual bool FadeIn(); // 0x00566F4C slot 0x10
    virtual bool FadeOut(); // 0x00566F80 slot 0x14
    // one frame; true in the frame the fade ends
    virtual bool Calc(); // 0x00566EB8 slot 0x18
    virtual void Draw() = 0; // slot 0x1C

    u8 _4;              // 0x4 (set by the constructor, not used here)
    u8 m_Status;        // 0x5 Status
    u8 m_Flags;         // 0x6 Flag
    s32 m_Duration;     // 0x8 frames
    s32 m_FrameCount;   // 0xC
};
ASSERT_SIZE(FaderBase, 0x10);
} // namespace co
} // namespace ssys
