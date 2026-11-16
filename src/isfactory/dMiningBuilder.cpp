#include "isfactory/dMiningBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "Other/dTourData.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"

namespace {
// the five rocks
const u32 ROCK_NUM = 5;

// a random rock
inline SvFgName MakeRock(sead::Random* random)
{
    return SvFgName(static_cast<SvFgName::ID>(SvFgName::NAME_ROCK_A + random->getU32(ROCK_NUM)));
}

// count rocks of the kind at random free units (a first rock of the kind is made and not placed)
inline void PlaceRocks(Field* field, const FieldRect* units, sead::Random* random, u8 kind, u32 count)
{
    {
        SvFgName rock = MakeRock(random);
        rock.Func6BA880(kind);
    }
    for (u32 i = 0; i < count; i++) {
        SvFgName rock = MakeRock(random);
        rock.Func6BA880(kind);
        Func527094(field, units, &rock, random);
    }
}

// the items of the default layout: items that are not ... and not grown trees always, grown trees with a chance of
// 100 % (50 % at level 0)
struct MiningItemsSetter
{
    MiningItemsSetter(Field* field, const FgDataLoader* data, sead::Random* random)
        : m_pField(field), m_pData(data), m_pRandom(random)
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
            if (Func769C18(&item)) {
                continue;
            }
            u32 chance = 100;
            if (item.IsGrownTree() && Func76F7E4(&TourData::s_pInstance->m_Seed) == 0) {
                chance = 50;
            }
            if (m_pRandom->getU32() % 100 < chance) {
                bg->SetFg(item, entry.x, entry.z, 0);
            }
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
    sead::Random* m_pRandom;
};
} // namespace

namespace isfactory {
// 0x00704300 slot 0x00
// 0x007042F0 (deleting dtor)
isfactory::MiningBuilder::~MiningBuilder()
{
    // empty (in the original too)
}

// 0x00703A74 slot 0x18 (name is ours)
void isfactory::MiningBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data = mgr->GetFgData("FgDataTour/Default.bin");
    if (data == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), MiningItemsSetter(m_pField, data, m_pRandom));
}

// 0x00703CD0 slot 0x1C (name is ours)
void isfactory::MiningBuilder::Setup1C()
{
    const FieldRect* units = &m_pField->GetUnitBounds();
    const u32 playerCount = Func6E1510();
    const u8 kindA = Func76FABC(&m_pTourData->m_Seed);
    const u8 kindB = Func76F764(&m_pTourData->m_Seed);
    u32 levelFactor = 0;
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    if (level == 0) {
        levelFactor = 0;
    } else if (level == 1) {
        levelFactor = 1;
    } else if (level == 2) {
        levelFactor = 2;
    }
    PlaceRocks(m_pField, units, m_pRandom, kindA, playerCount * levelFactor);
    PlaceRocks(m_pField, units, m_pRandom, kindB, playerCount * 4);
    // four other kinds at random
    u8 kinds[] = {0, 1, 2, 3, 4, 5, 0xFF, 0xFF};
    u32 num = 6;
    for (u32 i = 0; i < num; i++) {
        if (kinds[i] == kindA) {
            kinds[i] = kinds[--num];
            break;
        }
    }
    for (u32 i = 0; i < num; i++) {
        if (kinds[i] == kindB) {
            kinds[i] = kinds[--num];
            break;
        }
    }
    u8 others[4];
    for (u32 i = 0; i < 4; i++) {
        const u32 index = m_pRandom->getU32(num);
        others[i] = kinds[index];
        kinds[index] = kinds[--num];
    }
    for (u32 i = 0; i < 4; i++) {
        PlaceRocks(m_pField, units, m_pRandom, others[i], playerCount * 5);
    }
    // and plain rocks
    for (u32 i = 0; i < playerCount * 5; i++) {
        const SvFgName rock = MakeRock(m_pRandom);
        Func527094(m_pField, units, &rock, m_pRandom);
    }
}

} // namespace isfactory
