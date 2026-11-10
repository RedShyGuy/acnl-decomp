#include "Other/dPWorkList.h"

#include "script/dScriptExtern.h"

// 0x009516B0
Unknown9516B0* g_pUnknown9516B0 = NULL;

// 0x007F4EE8
template CatalogBase* CatalogBase::Create<PWorkList>();

// 0x006EF868 slot 0x00
// 0x006EF828 (deleting dtor)
PWorkList::~PWorkList()
{
    // (only the members and the base are destroyed)
}

// 0x006EF6F4 slot 0x0C
void PWorkList::Finalize()
{
    SelectBase::Finalize();
    Func293264();
}

// 0x00770D0C slot 0x2C
u32 PWorkList::vf_0x2C()
{
    if (GetSelectedId() == -1) {
        return 0;
    }
    return Func5CACA4(Func5CAE10(GetSelectedId()));
}

// 0x006EF714 slot 0x34
bool PWorkList::SetupItems()
{
    s32 num = 0;
    for (s32 i = 0; i < PROJECT_NUM; i++) {
        const u8 project = i;
        if (Func5CACCC(project) && Func6CBAC8(project)) {
            Func56E4B4(&m_Names[num], Func5CAE10(project));
            m_Ids[num] = project;
            num++;
        }
    }
    m_ItemNum = num;
    Func2912B8(1);
    return true;
}

// 0x006EF6B0 slot 0x40
void PWorkList::OnSelect(s32 selected, s32 prev)
{
    SelectBase::OnSelect(selected, prev);
    if (m_Selected != -1) {
        Func229054(g_pUnknown9516B0, m_Ids[m_Selected]);
    } else {
        Func2274A4(g_pUnknown9516B0);
    }
}

// 0x006EF704 slot 0x54
void PWorkList::OnEnterOut()
{
    Func2274A4(g_pUnknown9516B0);
}

// 0x006EF824 slot 0x5C
void PWorkList::OnEnterWait()
{
    // empty (in the original too)
}

// 0x006EF7AC slot 0x60
void PWorkList::UpdateWait()
{
    const s32 prev = m_Selected;
    SelectBase::UpdateWait();
    if (m_Selected != prev && m_Selected != -1) {
        if (g_pUnknown9516B0 != NULL) {
            Func229054(g_pUnknown9516B0, m_Ids[m_Selected]);
        }
    } else if (m_Selected == -1 && prev != -1) {
        Func2274A4(g_pUnknown9516B0);
    }
}
