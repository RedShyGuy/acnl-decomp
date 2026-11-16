#include "field/dField.h"

#include "Sv/dSvApi.h"

// 0x0057C680 (name is ours)
const FieldRect& BgInfo::GetLocalUnitRect()
{
    // 0x0098B124 (guard 0x0094F4D0)
    static FieldRect s_LocalUnitRect(Vector2i::Zero(), Vector2i(UNIT_BASE_NUM - 1, UNIT_BASE_NUM - 1));
    return s_LocalUnitRect;
}

// 0x0057C708 (name is ours)
void BgInfo::SetBlocks(SvFgBlock* layer0, SvFgBlock* layer1)
{
    m_pBlocks[0] = layer0;
    m_pBlocks[1] = layer1;
}

// 0x0057C714 (name is ours)
bool BgInfo::IsInside(s32 unitX, s32 unitZ)
{
    Vector2i pos(unitX, unitZ);
    return GetLocalUnitRect().Contains(pos);
}

// 0x0057C7C8 (name is ours)
bool BgInfo::SetFg(const SvFgName& fg, s32 unitX, s32 unitZ, u8 layer)
{
    if (IsSaveIoOnWholeFile() || IsSaveIoOnMainData() || IsSaveIoOnNpcData()) {
        return false;
    }
    for (u8 i = 0; i < 4; i++) {
        if (IsSaveIoOnPlayer(i)) {
            return false;
        }
    }
    SvFgName* item = GetFg(unitX, unitZ, layer);
    if (item == NULL) {
        return false;
    }
    *item = fg;
    return true;
}

// 0x0057C860 (name is ours)
bool BgInfo::Initialize(s32, s32)
{
    m_pBlocks[0] = NULL;
    m_pBlocks[1] = NULL;
    return true;
}

// 0x0057C874 (name is ours)
void BgInfo::SetName(SvBgName name)
{
    m_Name = name;
}

// 0x0057C87C (name is ours)
void BgInfo::Finalize()
{
    m_Name = BG_NAME_NONE;
    m_pBlocks[0] = NULL;
    m_pBlocks[1] = NULL;
}

// 0x0075383C (name is ours)
SvFgName* BgInfo::GetFg(s32 unitX, s32 unitZ, u8 layer)
{
    if (layer < MAX_LAYER_NUM) {
        Vector2i pos(unitX, unitZ);
        if (GetLocalUnitRect().Contains(pos) && m_pBlocks[layer] != NULL) {
            return m_pBlocks[layer]->GetFg(unitX, unitZ);
        }
    }
    return NULL;
}
