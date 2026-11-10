#include "Other/dOrderList.h"

#include "Item/item_Api.h"
#include "script/dScriptExtern.h"

// 0x0098C080
Unknown98C080 g_Unknown98C080;

namespace {
// the groups of the items (GetKindGroup)
const s32 GROUP_NUM = 3;
} // namespace

// 0x007F4E74
template CatalogBase* CatalogBase::Create<OrderList>();

// 0x006EF658 slot 0x00
// 0x006EF5FC (deleting dtor)
OrderList::~OrderList()
{
    // (only the members and the base are destroyed)
}

// 0x006EF3E4 slot 0x0C
void OrderList::Finalize()
{
    SelectBase::Finalize();
    Func293264();
    if (Func692FD0()->m_pProcess != NULL) {
        Func6925C0(Func692FD0());
    }
}

// 0x00770C20 slot 0x28
bool OrderList::vf_0x28()
{
    if (m_Selected == -1) {
        return true;
    }
    if (Func769264(&m_Items[m_Selected]) == 178) {
        return true;
    }
    return Func76D334(&m_Items[m_Selected]);
}

// 0x00770C8C slot 0x2C
u32 OrderList::vf_0x2C()
{
    if (m_Selected == -1) {
        return 0;
    }
    return static_cast<u32>(Func3055C8(&m_Items[m_Selected]) * 1.2f);
}

// 0x006EF410 slot 0x34
bool OrderList::SetupItems()
{
    const u32 num = g_Unknown98C080.num;
    void* myDesign = Func6FB8CC();
    // wallpapers, carpets and the others, each sorted
    SvFgName groups[GROUP_NUM][ITEM_MAX];
    s32 counts[GROUP_NUM] = {0, 0, 0};
    for (u32 i = 0; i < num; i++) {
        const s32 group = GetKindGroup(g_Unknown98C080.items[i]);
        AddName(groups[group], &counts[group], g_Unknown98C080.items[i]);
    }
    m_ItemNum = 0;
    for (s32 i = 0; i < GROUP_NUM; i++) {
        SortNames(groups[i], counts[i]);
        for (s32 j = 0; j < counts[i]; j++) {
            m_Items[m_ItemNum] = groups[i][j];
            m_ItemNum++;
        }
    }
    for (u32 i = 0; i < static_cast<u32>(m_ItemNum); i++) {
        item::GetName(&m_Names[i], m_Items[i], myDesign);
    }
    return true;
}

// 0x006EF590 slot 0x60
void OrderList::UpdateWait()
{
    const s32 prev = m_Selected;
    SelectBase::UpdateWait();
    if (m_Selected != -1) {
        const SvFgName name(m_Items[m_Selected]);
        Func692330(Func692FD0(), name);
    } else if (prev != -1) {
        Func6925C0(Func692FD0());
    }
}
