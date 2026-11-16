#pragma once

#include "decomp.h"

namespace fgobj {
// One bit per unit of the largest field (112 x 96 units); the Proc marks the units whose items changed (class name
// from the RTTI, the other names are ours).
// RTTI N5fgobj8FieldBitE @ 0x008D2A3C
// vtable 0x00908E3C (vptr 0x00908E44), offset_to_top 0, 2 entries
class FieldBit
{
public:
    static const s32 WIDTH = 112;
    static const s32 HEIGHT = 96;

    FieldBit()
    {
        Clear();
    }
    virtual ~FieldBit(); // 0x005A3234 slot 0x00
    // 0x005A3230 slot 0x04 (deleting dtor)

    void Set(s32 x, s32 z);   // 0x005A310C
    void Reset(s32 x, s32 z); // 0x005A31F0
    void Clear();             // 0x005A314C
    // (inline; a copy for the game) whether the bit of the unit x, z is set (no range check)
    DECOMP_EMIT_COPY bool IsSet(s32 x, s32 z) const // 0x00754CB0
    {
        s32 index = x + z * WIDTH;
        return (m_Bits[index >> 5] & (1 << (index & 31))) != 0;
    }

private:
    /* 0x4 */ u32 m_Bits[WIDTH * HEIGHT / 32];
};
ASSERT_SIZE(FieldBit, 0x544);
} // namespace fgobj
