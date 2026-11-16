#include "isfactory/dInsectCatchingReduceTreeBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace {
// the items of the default layout, the grown trees only with a chance of 50 %
struct ReduceTreeItemsSetter
{
    ReduceTreeItemsSetter(Field* field, const FgDataLoader* data, sead::Random* random)
        : m_pField(field), m_pData(data), m_pRandom(random)
    {
    }

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (bg == NULL || !m_pData->IsLoaded()) {
            return;
        }
        const u32 num = m_pData->GetItems(m_Items, sizeof(m_Items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            const SvFgName item(static_cast<SvFgName::ID>(entry.id));
            if (item.IsGrownTree() && m_pRandom->getU32() % 100 >= 50) {
                continue;
            }
            bg->SetFg(item, entry.x, entry.z, 0);
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    sead::Random* m_pRandom;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};
} // namespace

namespace isfactory {
// 0x00707DB8 slot 0x00
// 0x00707DA8 (deleting dtor)
isfactory::InsectCatchingReduceTreeBuilder::~InsectCatchingReduceTreeBuilder()
{
    // empty (in the original too)
}

// 0x00707B8C slot 0x18 (name is ours)
void isfactory::InsectCatchingReduceTreeBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data = mgr->GetFgData("FgDataTour/Default.bin");
    if (data == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), ReduceTreeItemsSetter(m_pField, data, m_pRandom));
}

} // namespace isfactory
