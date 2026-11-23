#include "Other/dPickerMono.h"

#include "Other/dFormatBuffer.h"
#include "Other/dTextureImage.h"
#include "nn/math/math_Vector3.h"

namespace {
// the colors of the picker
const u32 COLOR_NUM = 15;
// the color set of the shades in the color table and the row of the texture
const u32 COLOR_SET = 15;
const u32 TEXTURE_ROW = 15;
} // namespace

// 0x001AF450
u8 PickerMono::Pick()
{
    const s32 index = m_Cursor.x * COLOR_NUM;
    const u8 color = m_Colors[index];
    AttachCursor();
    const f32 width = m_pPicture->m_Size.width;
    nn::math::VEC3 position;
    position.x = width * ((index + 0.5f) * (1.0f / COLOR_NUM)) - width * 0.5f;
    m_pCursor->m_pAll->SetTranslate(position);
    SetCursorColor(color);
    return color;
}

// 0x001AF560
void PickerMono::Setup(const SetupArg& arg)
{
    m_pLayout = arg.pLayout;
    m_Index = arg.index;
    m_pCursor = arg.pCursor;
    g_FormatBuffer.format("%s_%02d", "P_pick", arg.index);
    m_pPicture = m_pLayout->FindPicture(g_FormatBuffer.getCString());
    TextureImage16 image(arg.pTexture);
    for (u32 i = 0; i < COLOR_NUM; i++) {
        m_Colors[i] = SvMyDesign::GetPickerColor(COLOR_SET, i);
        image.SetPixel(i, TEXTURE_ROW, SvMyDesign::GetColorValue(m_Colors[i], true));
        s_ColorEntries[m_Colors[i]].pPicker = this;
        s_ColorEntries[m_Colors[i]].index = i;
    }
    SetTexture(arg.pTexture);
    SetTexCoordRect(0, TEXTURE_ROW, COLOR_NUM - 1, TEXTURE_ROW);
}

// 0x001AF724
void PickerMono::SetCursor(u32 index)
{
    f32 x = index * (1.0f / COLOR_NUM);
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 0.99f) {
        x = 0.99f;
    }
    m_Cursor.x = x;
}
