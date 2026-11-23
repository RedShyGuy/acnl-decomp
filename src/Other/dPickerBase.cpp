#include "Other/dPickerBase.h"

#include "nn/math/math_Vector2.h"

// 0x0098CB28
PickerBase::ColorEntry PickerBase::s_ColorEntries[256];

// 0x001AF39C
void PickerBase::SetTexCoordRect(s32 x0, s32 y0, s32 x1, s32 y1)
{
    const f32 scale = 1.0f / 16.0f;
    nn::math::VEC2 coords[4];
    coords[0] = nn::math::VEC2(x0 * scale, y0 * scale);
    coords[1] = nn::math::VEC2((x1 + 1) * scale, y0 * scale);
    coords[2] = nn::math::VEC2(x0 * scale, (y1 + 1) * scale);
    coords[3] = nn::math::VEC2((x1 + 1) * scale, (y1 + 1) * scale);
    m_pPicture->SetTexCoord(0, coords);
}
