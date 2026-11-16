#include "isfactory/dBalloonBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dJmpBlock.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace {
// the items of the default layout, each one with a chance: grown trees with the tree chance, the items of Func769C18
// with the other chance, the rest always
struct BalloonItemsSetter
{
    BalloonItemsSetter(Field* field, const FgDataLoader* data, u32 treeChance, u32 otherChance, sead::Random* random)
        : m_pField(field), m_pData(data), m_TreeChance(treeChance), m_OtherChance(otherChance), m_pRandom(random)
    {
    }

    void operator()(const Vector2i& block)
    {
        if (m_pField == NULL) {
            return;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (!m_pData->IsLoaded()) {
            return;
        }
        const u32 num = m_pData->GetItems(m_Items, sizeof(m_Items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            const FgDataItem& entry = m_Items[i];
            const SvFgName item(static_cast<SvFgName::ID>(entry.id));
            u32 chance = 100;
            if (item.IsGrownTree()) {
                chance = m_TreeChance;
            }
            if (Func769C18(&item)) {
                chance = m_OtherChance;
            }
            if (m_pRandom->getU32() % 100 < chance) {
                bg->SetFg(item, entry.x, entry.z, 0);
            }
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
    u32 m_TreeChance;
    u32 m_OtherChance;
    sead::Random* m_pRandom;
};
} // namespace

namespace isfactory {
// 0x00704848 slot 0x00
// 0x00704838 (deleting dtor)
isfactory::BalloonBuilder::~BalloonBuilder()
{
    // empty (in the original too)
}

// 0x00704304 slot 0x18 (name is ours)
void isfactory::BalloonBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data = mgr->GetFgData("FgDataTour/Default.bin");
    if (data == NULL) {
        return;
    }
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    u32 treeChance = 0;
    if (level == 0) {
        treeChance = 0;
    } else if (level == 1) {
        treeChance = 50;
    } else if (level == 2) {
        treeChance = 100;
    }
    u32 otherChance = 0;
    if (level == 0 || level == 1) {
        otherChance = 50;
    } else if (level == 2) {
        otherChance = 100;
    }
    ForEachBlock(m_pField->GetBlockBounds(), BalloonItemsSetter(m_pField, data, treeChance, otherChance, m_pRandom));
}

// 0x007045A8 slot 0x1C (name is ours)
void isfactory::BalloonBuilder::Setup1C()
{
    if (m_pField == NULL) {
        return;
    }
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    u32 chance = 1;
    if (level == 0) {
        chance = 0;
    } else if (level == 1) {
        chance = 2;
    } else if (level == 2) {
        chance = 5;
    }
    PlacePitfalls(m_pField, m_pRandom, chance);
}

} // namespace isfactory
