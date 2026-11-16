#pragma once

#include "decomp.h"
#include "nn/math/math_MTX34.h"
#include "nn/math/math_Vector3.h"

namespace graphic {
// A shadow on the ground under an object: a matrix at a position, turned on the curve of the world, with a size; kept in
// lists of the shadow drawing (the code around 0x006F0028; the class name and all names are ours; only the parts fgobj
// uses). The field objects give it the size and kind from their model table (items: size 8).
struct Shadow
{
    // (inline in the constructors of its owners; the matrix stays uninitialized)
    Shadow() : m_Size(0.0f), m_Unk0x34(0.0f), m_Unk0x38(0.0f), m_Opacity(1.0f), m_Unk0x80(0), m_Unk0x82(0), m_Flags(1),
               m_Kind(27), m_pNext(NULL)
    {
        for (u32 i = 0; i < 16; i++) {
            m_Unk0x3C[i] = 0;
        }
    }

    // the shadow at a position (the curve angle turns it about x; the meaning of the other arguments is unknown)
    void Set(const nn::math::VEC3* position, s16 curveAngle, bool unknown, u8 kind, s16 unknown2, f32 size, f32 size2,
             f32 offset); // 0x006F0028

    nn::math::MTX34 m_Matrix;   // 0x00
    f32 m_Size;                 // 0x30
    f32 m_Unk0x34;              // 0x34
    f32 m_Unk0x38;              // 0x38
    u32 m_Unk0x3C[16];          // 0x3C
    f32 m_Opacity;              // 0x7C 0..1 (the falling and growing objects fade it in)
    s16 m_Unk0x80;              // 0x80
    s16 m_Unk0x82;              // 0x82
    u8 m_Flags;                 // 0x84
    u8 m_Kind;                  // 0x85
    Shadow* m_pNext;            // 0x88 in the lists of the shadow drawing
};
ASSERT_SIZE(Shadow, 0x8C);
} // namespace graphic
