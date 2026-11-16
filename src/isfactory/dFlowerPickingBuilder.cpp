#include "isfactory/dFlowerPickingBuilder.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dFieldFactory.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "isfactory/isfactory_Internal.h"
#include "Item/item_Api.h"
#include "Other/dTourData.h"
#include "script/dScriptExtern.h"
#include "Sv/dSvBgName.h"

namespace {
// whether the ground of a block has the acre kind (FindBlock)
struct BlockKindTest
{
    BlockKindTest(Field* field, u8 kind) : m_pField(field), m_Kind(kind) {}

    bool operator()(const Vector2i& block) const
    {
        if (m_pField == NULL) {
            return false;
        }
        BgInfo* bg = m_pField->GetBg(block.x, block.y);
        if (bg == NULL || bg->GetName() == BG_NAME_NONE) {
            return false;
        }
        return Func6060E8(bg->GetName()) == m_Kind;
    }

    Field* m_pField;
    u8 m_Kind;
};

// the first block that passes the test (end if none does; like std::find_if)
template <typename T>
inline FieldRect::iterator FindBlock(FieldRect::iterator first, FieldRect::iterator last, T test)
{
    for (; first != last; ++first) {
        if (test(*first)) {
            return first;
        }
    }
    return last;
}

// the number of the flowers for the number of the players
inline u32 GetFlowerCount()
{
    switch (isfactory::Func6E1510()) {
    case 1:
        return 25;
    case 2:
        return 49;
    case 3:
        return 64;
    case 4:
        return 81;
    default:
        return 25;
    }
}

// count flowers of the set at random free units
inline void PlaceFlowers(Field* field, const FieldRect* units, SvFgNameSet* flowers, sead::Random* random, u32 count)
{
    for (u32 i = 0; i < count; i++) {
        const SvFgName flower = Func1AD2E0(flowers, random->getU32());
        SvFgName plant;
        Func768A60(&flower, &plant);
        Func527734(field, units, &plant, random);
    }
}
} // namespace

namespace isfactory {
// 0x00706EB4 slot 0x00
// 0x00706EA4 (deleting dtor)
isfactory::FlowerPickingBuilder::~FlowerPickingBuilder()
{
    // empty (in the original too)
}

// 0x007065EC slot 0x08 (name is ours)
void isfactory::FlowerPickingBuilder::SetupBlocks()
{
    const u8 state = m_pTourData->m_State;
    const u8 oceanSide = m_pTourData->m_OceanSide;
    u16 kinds[ACRE_KIND_NUM];
    GetAcreKinds(kinds, state, oceanSide, m_pRandom);
    u8 map[FieldFactory::ACRE_MAP_WIDTH * FieldFactory::ACRE_MAP_HEIGHT];
    FieldFactory::ReplacePlaceholders(map, kinds, s_PeninsulaMap);
    FieldFactory::ReplaceRandomCell8To19(map, m_pRandom);
    FieldFactory::ReplaceRandomCell34Or35Or39(map, m_pRandom);
    FieldFactory::SetNeighbors(map);
    ReplaceRandomInnerCell(map, 0, 5, m_pRandom);
    ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, map, m_pRandom));
    if (m_pField == NULL) {
        return;
    }
    // the block of the kind 5 gets the flower bed for the number of the players
    const FieldRect& blocks = m_pField->GetBlockBounds();
    const FieldRect::iterator found = FindBlock(blocks.begin(), blocks.end(), BlockKindTest(m_pField, 5));
    if (!(found != blocks.end())) {
        return;
    }
    SvBgName name = BG_NAME_GRD_F_F_0;
    switch (Func6E1510()) {
    case 1:
        break;
    case 2:
        name = BG_NAME_GRD_F_F_1;
        break;
    case 3:
        name = BG_NAME_GRD_F_F_2;
        break;
    case 4:
        name = BG_NAME_GRD_F_F_3;
        break;
    default:
        break;
    }
    BgInfo* bg = m_pField->GetBg((*found).x, (*found).y);
    if (bg != NULL) {
        bg->SetName(name);
    }
}

// 0x00706B00 slot 0x1C (name is ours)
void isfactory::FlowerPickingBuilder::Setup1C()
{
    {
        // the flowers of the tour item (all flowers, changed by Func302A90, if there is none)
        const FieldRect* units = &m_pField->GetUnitBounds();
        SvFgNameSet flowers;
        const u32 count = GetFlowerCount();
        const SvFgName target(m_pTourData->m_TourItem);
        if (target.IsEmpty()) {
            flowers.Clear();
            Func302978(&flowers, item::KIND_FLOWER);
            Func302A90(&flowers, 74);
        } else if (target.GetKind() == item::KIND_FLOWER) {
            flowers.Clear();
            Func302F80(&flowers, &target);
        } else {
            flowers.Clear();
            Func302978(&flowers, item::KIND_FLOWER);
            Func302A90(&flowers, 74);
            const u8 value = Func769264(&target);
            if (value == 111) {
                Func6F3ECC(&flowers, Func769828(&target));
            } else if (value == 112) {
                Func6F1524(&flowers, Func76BF1C(&target));
            }
        }
        PlaceFlowers(m_pField, units, &flowers, m_pRandom, count);
    }
    {
        // twice as many other flowers
        const FieldRect* units = &m_pField->GetUnitBounds();
        const u32 count = GetFlowerCount() * 2;
        SvFgNameSet flowers;
        flowers.Clear();
        Func302978(&flowers, item::KIND_FLOWER);
        Func302A90(&flowers, 74);
        PlaceFlowers(m_pField, units, &flowers, m_pRandom, count);
    }
}

// 0x00706DC8 slot 0x20 (name is ours)
void isfactory::FlowerPickingBuilder::Setup20()
{
    SetupTourObjects(m_pTourData, m_pRandom, 7, 1);
}

} // namespace isfactory
