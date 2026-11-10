#include "Other/dTicketList.h"

#include "Item/item_Api.h"
#include "script/dScriptExtern.h"
#include "stage/stage_Api.h"

namespace {
// the groups of the items (GetKindGroup); the lists hold at most 128 items each
const s32 GROUP_NUM = 3;
const s32 GROUP_MAX = 128;
} // namespace

// 0x007F4C90
template CatalogBase* CatalogBase::Create<TicketList>();

// 0x001B7890 slot 0x00
// 0x001B7834 (deleting dtor)
TicketList::~TicketList()
{
    // (only the members and the base are destroyed)
}

// 0x001B74F8 slot 0x0C
void TicketList::Finalize()
{
    SelectBase::Finalize();
    Func293264();
    if (Func692FD0()->m_pProcess != NULL) {
        Func6925C0(Func692FD0());
    }
}

// 0x0070CE38 slot 0x2C
u32 TicketList::vf_0x2C()
{
    if (m_Selected == -1) {
        return 0;
    }
    return Func5C3750() ? 3 : 5;
}

// 0x001B7524 slot 0x34
bool TicketList::SetupItems()
{
    ScStage* scene = stage::GetScene();
    // wallpapers, carpets and the others, each sorted
    SvFgName groups[GROUP_NUM][GROUP_MAX];
    s32 counts[GROUP_NUM] = {0, 0, 0};
    if (scene != NULL) {
        const void* object = Func75E9AC(scene);
        if (object != NULL) {
            for (s32 i = 0; i < LIST0_NUM; i++) {
                const SvFgName name = Func71FF94(object, i);
                if (!name.IsEmpty()) {
                    const s32 group = GetKindGroup(name);
                    AddName(groups[group], &counts[group], name);
                }
            }
            SvFgName name;
            for (s32 i = 0; i < LIST1_NUM; i++) {
                name = Func720184(object, i);
                if (!name.IsEmpty()) {
                    const s32 group = GetKindGroup(name);
                    AddName(groups[group], &counts[group], name);
                }
            }
            name = Func2FCC0C(object);
            if (!name.IsEmpty()) {
                AddName(groups[0], &counts[0], name);
            }
            name = Func7202FC(object);
            if (!name.IsEmpty()) {
                AddName(groups[1], &counts[1], name);
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
                item::GetName(&m_Names[i], m_Items[i], NULL);
            }
        }
    }
    return true;
}

// 0x00692554 slot 0x60
void TicketList::UpdateWait()
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
