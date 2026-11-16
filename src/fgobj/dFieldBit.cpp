#include "fgobj/dFieldBit.h"

#include <cstring>

namespace fgobj {
// 0x005A3234
// 0x005A3230 (deleting dtor)
fgobj::FieldBit::~FieldBit()
{
    // empty (in the original too)
}

// 0x005A310C (name is ours)
void fgobj::FieldBit::Set(s32 x, s32 z)
{
    if (x >= 0 && z >= 0 && x < WIDTH && z < HEIGHT) {
        s32 index = x + z * WIDTH;
        m_Bits[index >> 5] |= 1 << (index & 31);
    }
}

// 0x005A314C (name is ours)
void fgobj::FieldBit::Clear()
{
    std::memset(m_Bits, 0, sizeof(m_Bits));
}

// 0x005A31F0 (name is ours)
void fgobj::FieldBit::Reset(s32 x, s32 z)
{
    if (x >= 0 && z >= 0 && x < WIDTH && z < HEIGHT) {
        s32 index = x + z * WIDTH;
        m_Bits[index >> 5] &= ~(1 << (index & 31));
    }
}
} // namespace fgobj
