#include "Other/dPickerParts.h"

#include "Other/dFormatBuffer.h"
#include "Other/dTextureImage.h"
#include "nn/math/math_Vector3.h"

namespace {
// the colors per row and column
const u32 CELL_SIZE = 3;
} // namespace

// 0x001CE56C
u8 PickerParts::Pick()
{
    const s32 column = m_Cursor.x * CELL_SIZE;
    const s32 row = m_Cursor.y * CELL_SIZE;
    const u8 color = m_Colors[row * CELL_SIZE + column];
    AttachCursor();
    const nw::lyt::Size& size = m_pPicture->m_Size;
    nn::math::VEC3 position;
    position.x = size.width * ((column + 0.5f) * (1.0f / CELL_SIZE)) - size.width * 0.5f;
    position.y = size.height * 0.5f - size.height * ((row + 0.5f) * (1.0f / CELL_SIZE));
    m_pCursor->m_pAll->SetTranslate(position);
    SetCursorColor(color);
    return color;
}

// 0x001CE6BC
void PickerParts::Setup(const SetupArg& arg)
{
    m_pLayout = arg.pLayout;
    m_Index = arg.index;
    m_pCursor = arg.pCursor;
    g_FormatBuffer.format("%s_%02d", "P_pick", arg.index);
    m_pPicture = m_pLayout->FindPicture(g_FormatBuffer.getCString());
    m_CellX = (m_Index & 3) * CELL_SIZE;
    m_CellY = (m_Index >> 2) * CELL_SIZE;
    TextureImage16 image(arg.pTexture);
    for (s32 i = 0; i < 9; i++) {
        const s32 x = m_CellX + i % CELL_SIZE;
        const s32 y = m_CellY + i / CELL_SIZE;
        m_Colors[i] = SvMyDesign::GetPickerColor(i, m_Index);
        image.SetPixel(x, y, SvMyDesign::GetColorValue(m_Colors[i], true));
        s_ColorEntries[m_Colors[i]].pPicker = this;
        s_ColorEntries[m_Colors[i]].index = i;
    }
    SetTexture(arg.pTexture);
    SetTexCoordRect(m_CellX, m_CellY, m_CellX + 2, m_CellY + 2);
}

// 0x001CE8BC
void PickerParts::SetCursor(u32 index)
{
    const s32 column = index % CELL_SIZE;
    const s32 row = index / CELL_SIZE;
    f32 x = column * (1.0f / CELL_SIZE);
    if (x < 0.0f) {
        x = 0.0f;
    } else if (x > 0.9f) {
        x = 0.9f;
    }
    m_Cursor.x = x;
    f32 y = row * (1.0f / CELL_SIZE);
    if (y < 0.0f) {
        y = 0.0f;
    } else if (y > 0.9f) {
        y = 0.9f;
    }
    m_Cursor.y = y;
}

// 0x001CE950
PickerParts::PickerParts() : m_CellX(0), m_CellY(0)
{
}
