#include "isfactory/dTreasureHuntBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "Item/item_Api.h"
#include "netgame/netgame_Api.h"
#include "Other/dTourData.h"
#include "script/dScriptExtern.h"
#include "sead/seadObjArray.h"
#include "sead/seadRandom.h"
#include "Sv/dSvBgName.h"
#include "Sv/dSvFgName.h"

namespace {
// the places of the treasures
typedef sead::FixedObjArray<Vector2i, 100> PlaceArray;

// a ground of the treasure hunt at a block
struct TreasureGround
{
    s32 x;
    s32 z;
    u16 name;
};

// the items of the layout but the places of the treasures (NAME_BROKEN_HOLE)
struct TreasureItemsSetter
{
    TreasureItemsSetter(Field* field, const FgDataLoader* data, sead::Random* random)
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
            if (entry.id != SvFgName::NAME_BROKEN_HOLE) {
                bg->SetFg(SvFgName(static_cast<SvFgName::ID>(entry.id)), entry.x, entry.z, 0);
            }
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    sead::Random* m_pRandom;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};

// count treasures of the set, each one buried at the next two places
inline void BuryTreasures(Field* field, PlaceArray* places, SvFgNameSet* set, sead::Random* random, u32 count)
{
    for (u32 i = 0; i < count; i++) {
        SvFgName treasure = Func1AD2E0(set, random->getU32());
        treasure.SetBuried();
        for (u32 j = 0; j < 2; j++) {
            Vector2i* place = places->front();
            if (place != NULL) {
                field->PlaceItem(treasure, place->x, place->y, 0);
            }
            places->erase(0);
        }
        RemoveFgName(set, &treasure);
    }
}
} // namespace

namespace isfactory {
// 0x0070656C slot 0x00
// 0x0070655C (deleting dtor)
isfactory::TreasureHuntBuilder::~TreasureHuntBuilder()
{
    // empty (in the original too)
}

// 0x007059EC slot 0x08 (name is ours)
void isfactory::TreasureHuntBuilder::SetupBlocks()
{
    const u8 map[] = {
        84, 85, 85, 85, 85, 85, 86,
        83, 95, 88, 88, 88, 89, 82,
        83, 94, 96, 97, 96, 90, 82,
        83, 94, 96, 96, 96, 90, 82,
        83, 93, 92, 92, 92, 91, 82,
        81, 79, 79, 79, 79, 79, 80,
    };
    ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, map, m_pRandom));
    const TreasureGround grounds[] = {
        {2, 2, BG_NAME_GRD_IF_3}, {3, 2, BG_NAME_GRD_IF_H_0}, {4, 2, BG_NAME_GRD_IF_4},
        {2, 3, BG_NAME_GRD_IF_0}, {3, 3, BG_NAME_GRD_IF_1},   {4, 3, BG_NAME_GRD_IF_2},
    };
    for (u32 i = 0; i < sizeof(grounds) / sizeof(grounds[0]); i++) {
        BgInfo* bg = m_pField->GetBg(grounds[i].x, grounds[i].z);
        if (bg != NULL) {
            bg->SetName(static_cast<SvBgName>(grounds[i].name));
        }
    }
}

// 0x00705AB0 slot 0x18 (name is ours)
void isfactory::TreasureHuntBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    // the layout of the level and the number of the players
    switch (Func76F7E4(&m_pTourData->m_Seed)) {
    case 0:
        m_pData = mgr->GetFgData(m_PlayerCount < 3 ? "FgDataTour/F0.bin" : "FgDataTour/F1.bin");
        break;
    case 1:
        m_pData = mgr->GetFgData(m_PlayerCount < 3 ? "FgDataTour/F2.bin" : "FgDataTour/F3.bin");
        break;
    case 2:
        m_pData = mgr->GetFgData(m_PlayerCount < 3 ? "FgDataTour/F4.bin" : "FgDataTour/F5.bin");
        break;
    default:
        break;
    }
    if (m_pData == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), TreasureItemsSetter(m_pField, m_pData, m_pRandom));
}

// 0x00705DCC slot 0x1C (name is ours)
void isfactory::TreasureHuntBuilder::Setup1C()
{
    if (m_pData == NULL) {
        return;
    }
    // the places of the treasures (NAME_BROKEN_HOLE in the layout) in a random order
    PlaceArray places;
    if (m_pField == NULL) {
        return;
    }
    const FieldRect& blocks = m_pField->GetBlockBounds();
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        BgInfo* bg = m_pField->GetBg((*it).x, (*it).y);
        if (bg == NULL) {
            continue;
        }
        FgDataItem items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
        const u32 num = m_pData->GetItems(items, sizeof(items), bg->GetName());
        for (u32 i = 0; i < num; i++) {
            if (items[i].id == SvFgName::NAME_BROKEN_HOLE) {
                const Vector2i unit(items[i].x, items[i].z);
                places.pushBack(FdInfo::ToUnit(*it, unit));
            }
        }
    }
    TourData::s_pInstance->m_TreasurePairNum = places.size() / 2;
    places.shuffle(m_pRandom);
    // the counts of the three kinds of treasures for the level and the number of the players
    SvFgNameSet treasures;
    treasures.Clear();
    const bool isRandomMatch = netgame::GetNetgameType() == netgame::TYPE_RANDOM_MATCH;
    Func2FECF8(&treasures, 5, 20, isRandomMatch);
    Func2FECF8(&treasures, 5, 21, isRandomMatch);
    Func2FECF8(&treasures, 5, 22, isRandomMatch);
    Func2FECF8(&treasures, 5, 23, isRandomMatch);
    u32 count0 = 4;
    switch (Func76F7E4(&m_pTourData->m_Seed)) {
    case 0:
        if (m_PlayerCount >= 3) {
            count0 = 7;
        }
        break;
    case 1:
        count0 = m_PlayerCount < 3 ? 5 : 8;
        break;
    case 2:
        count0 = m_PlayerCount < 3 ? 6 : 9;
        break;
    default:
        break;
    }
    u32 count1 = 1;
    switch (Func76F7E4(&m_pTourData->m_Seed)) {
    case 1:
        if (m_PlayerCount >= 3) {
            count1 = 2;
        }
        break;
    case 2:
        count1 = m_PlayerCount >= 3 ? 3 : 2;
        break;
    default:
        break;
    }
    u32 count2 = 1;
    switch (Func76F7E4(&m_pTourData->m_Seed)) {
    case 1:
        count2 = 2;
        break;
    case 2:
        count2 = m_PlayerCount >= 3 ? 3 : 2;
        break;
    default:
        break;
    }
    BuryTreasures(m_pField, &places, &treasures, m_pRandom, count0);
    // gyroids without the Brewstoids
    treasures.Clear();
    Func302978(&treasures, item::KIND_HANIWA);
    const u16 brewstoids[] = {
        SvFgName::NAME_MEGA_BREWSTOID, SvFgName::NAME_MINI_BREWSTOID, SvFgName::NAME_TALL_BREWSTOID,
        SvFgName::NAME_BREWSTOID,
    };
    for (u32 i = 0; i < sizeof(brewstoids) / sizeof(brewstoids[0]); i++) {
        const SvFgName gyroid(static_cast<SvFgName::ID>(brewstoids[i]));
        RemoveFgName(&treasures, &gyroid);
    }
    BuryTreasures(m_pField, &places, &treasures, m_pRandom, count1);
    // fossils
    treasures.Clear();
    Func302978(&treasures, item::KIND_FOSSIL);
    BuryTreasures(m_pField, &places, &treasures, m_pRandom, count2);
}

} // namespace isfactory
