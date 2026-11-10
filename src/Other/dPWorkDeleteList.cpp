#include "Other/dPWorkDeleteList.h"

#include "Other/dPaneVisible.h"
#include "Other/dFormatBuffer.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "nw/lyt/lyt_Pane.h"
#include "script/dScriptExtern.h"

// the map panes P_mp_NN of the acres (5 x 5, the rows from the top)
// 0x00849AA4
const s32 PWorkDeleteList::s_MapPaneIndices[MAP_SIZE][MAP_SIZE] = {
    {20, 21, 22, 23, 24}, {0, 1, 2, 3, 4}, {5, 6, 7, 8, 9}, {10, 11, 12, 13, 14}, {15, 16, 17, 18, 19},
};

// 0x007F4D50
template CatalogBase* CatalogBase::Create<PWorkDeleteList>();

// 0x0029EEFC slot 0x00
// 0x0029EE90 (deleting dtor)
PWorkDeleteList::~PWorkDeleteList()
{
    // (only the members and the base are destroyed)
}

// (inline) the map marks the structure of an item
DECOMP_ALWAYS_INLINE void PWorkDeleteList::MarkStructure(s32 item)
{
    const SvStrc strc = *Func75A28C(&GetGardenPlus()->data.strcData, m_StrcIndices[item]);
    m_Map.Func221C88(strc.xCoord, strc.yCoord, 8, true);
}

// 0x0022344C slot 0x0C (falls into BsMenuMapBase::Func223478)
void PWorkDeleteList::Finalize()
{
    PWorkList::Finalize();
    m_MapLayout.UnbindAllAnimation();
    m_Map.Func223478();
}

// 0x00222E74 slot 0x10 (falls into BsMenuMapBase::Func222E90)
void PWorkDeleteList::Update()
{
    SelectBase::Update();
    m_Map.Func222E90();
}

// 0x00222B20 slot 0x14 (falls into BsMenuMapBase::Func222B3C)
void PWorkDeleteList::Draw()
{
    CatalogBase::Draw();
    m_Map.Func222B3C();
}

// 0x0071CA40 slot 0x2C
u32 PWorkDeleteList::vf_0x2C()
{
    if (GetSelectedId() == -1) {
        return 0;
    }
    SvStrcData* strcData = &GetGardenPlus()->data.strcData;
    return Func5CACA4(Func302720(Func75A28C(strcData, GetSelectedId()))) / 10;
}

// 0x0029EA84 slot 0x34
bool PWorkDeleteList::SetupItems()
{
    if (!m_MapArcReader.ReadArc("Layout/Menu/map_village.arc")) {
        return false;
    }
    switch (m_SetupStep) {
    case 0: {
        // (the original reads the name from a table at 0x00849B3C)
        BsMenuMapBase::Description description = {&m_MapArcReader, &m_MapLayout, "map_00"};
        m_Map.Func222C88(&description, true);
        m_Map.Func2206CC("P_mp_00");
        break;
    }
    case 1:
        if (!m_Map.Func220C7C()) {
            return false;
        }
        break;
    case 2: {
        // the acres of the town on the map
        u8* townMap = &GetGardenPlus()->data.townData.oceanSide;
        for (s32 y = 0; y < MAP_SIZE; y++) {
            for (s32 x = 0; x < MAP_SIZE; x++) {
                const u16 acre = *Func75EBB4(townMap, x + 1, y);
                g_FormatBuffer.format("%s_%02d", "P_mp", s_MapPaneIndices[y][x]);
                m_Map.Func22323C(g_FormatBuffer.getCString(), acre, 0);
            }
        }
        m_Map.Func221250(0, true);
        break;
    }
    case 3: {
        // the projects of the town that can be demolished, sorted by project
        SvStrcData* strcData = &GetGardenPlus()->data.strcData;
        const s32 strcNum = strcData->normalPwPsAmount;
        s32 num = 0;
        s32 found = 0;
        for (s32 i = 0; found < strcNum; i++) {
            SvStrc* strc = Func75A28C(strcData, i);
            if (Func718EC8(strc)) {
                continue;
            }
            if (Func6CC848(strc)) {
                m_StrcIndices[num] = i;
                m_Ids[num] = Func5CA270(Func302720(strc));
                num++;
            }
            found++;
        }
        m_ItemNum = num;
        for (u32 i = 0; i < static_cast<u32>(m_ItemNum - 1); i++) {
            for (u32 j = i + 1; j < static_cast<u32>(m_ItemNum); j++) {
                if (m_Ids[i] > m_Ids[j]) {
                    const u8 id = m_Ids[i];
                    m_Ids[i] = m_Ids[j];
                    m_Ids[j] = id;
                    const s32 index = m_StrcIndices[i];
                    m_StrcIndices[i] = m_StrcIndices[j];
                    m_StrcIndices[j] = index;
                }
            }
            Func56E4B4(&m_Names[i], Func5CAE10(m_Ids[i]));
        }
        Func56E4B4(&m_Names[m_ItemNum - 1], Func5CAE10(m_Ids[m_ItemNum - 1]));
        Func2912B8(1);
        return true;
    }
    default:
        break;
    }
    m_SetupStep++;
    return false;
}

// 0x0029E7D0 slot 0x3C
void PWorkDeleteList::OnScroll()
{
    SelectBase::OnScroll();
    if (GetSelectedId() != -1) {
        MarkStructure(m_Selected);
    }
}

// 0x0029E754 slot 0x40
void PWorkDeleteList::OnSelect(s32 selected, s32 prev)
{
    SelectBase::OnSelect(selected, prev);
    MarkStructure(m_Selected);
}

// 0x0029E918 slot 0x4C
void PWorkDeleteList::OnEnterIn()
{
    SetFlagVisible(m_Map.m_pPane, true);
    m_Map.Func223580(0.0f, false);
    m_Map.m_pLayout->Animate();
    m_Map.Func220410();
}

// 0x0029E864 slot 0x50
void PWorkDeleteList::UpdateIn()
{
    if (m_InAnim.IsDone() && m_Map.m_InAnim.IsDone()) {
        m_Layout.UnbindAnimation(&m_InAnim);
        m_Map.Func2203C0(false);
        m_IsAnimating = false;
        m_IsScrollBarStopped = false;
        m_Map.m_Unknown0xB3A6 = true;
        Change(&CatalogBase::State_Wait);
        return;
    }
    m_InAnim.IncreaseFrame();
    m_Map.m_InAnim.IncreaseFrame();
    m_Map.m_pLayout->Animate();
    m_Map.Func220410();
    m_Layout.Animate();
}

// 0x0029EA28 slot 0x54
void PWorkDeleteList::OnEnterOut()
{
    Func2274A4(g_pUnknown9516B0);
    m_Map.Func223580(0.0f, true);
    m_Map.m_pLayout->Animate();
    m_Map.Func220410();
    m_Map.m_Unknown0xB3A6 = false;
}

// 0x0029E96C slot 0x58
void PWorkDeleteList::UpdateOut()
{
    if (m_OutAnim.IsDone() && m_Map.m_OutAnim.IsDone()) {
        m_Layout.UnbindAnimation(&m_OutAnim);
        m_Map.Func2203C0(true);
        SetFlagVisible(m_Map.m_pPane, false);
        m_IsAnimating = false;
        Change(&CatalogBase::State_None);
        return;
    }
    m_OutAnim.IncreaseFrame();
    m_Map.m_OutAnim.IncreaseFrame();
    m_Map.m_pLayout->Animate();
    m_Map.Func220410();
    m_Layout.Animate();
}

// 0x0029EDFC slot 0x60
void PWorkDeleteList::UpdateWait()
{
    const s32 prev = m_Selected;
    SelectBase::UpdateWait();
    if (m_Selected != prev) {
        MarkStructure(m_Selected);
    }
}
