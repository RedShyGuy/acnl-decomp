#include "isfactory/dMazeBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "Other/dTourData.h"
#include "script/dScriptExtern.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadObjArray.h"
#include "sead/seadRandom.h"
#include "sead/seadStringFormatter.h"
#include "Sv/dSvBgName.h"
#include "Sv/dSvFgName.h"

namespace {
// the acre map of the maze (7 x 6 kinds; one with fewer than three players)
// 0x0090C920
const u8 s_MazeMap[42] = {
    83, 75, 73, 73, 73, 74, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 69, 6, 7, 6, 59, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 72, 40, 52, 36, 62, 82,
    81, 79, 79, 79, 79, 79, 80,
};
// 0x0090C94A
const u8 s_MazeMapSmall[42] = {
    83, 75, 73, 73, 73, 74, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 69, 6, 7, 6, 59, 82,
    83, 69, 0, 0, 0, 59, 82,
    83, 72, 40, 52, 36, 62, 82,
    81, 79, 79, 79, 79, 79, 80,
};
// the bushes for the placeholder NAME_UNUSED_BUSH of the item layout
// 0x0090C974
const u16 s_MazeBushes[] = {
    SvFgName::NAME_HOLLY_BUSH,          SvFgName::NAME_PINK_AZALEA_BUSH,   SvFgName::NAME_WHITE_AZALEA_BUSH,
    SvFgName::NAME_BLUE_HYDRANGEA_BUSH, SvFgName::NAME_PINK_HYDRANGEA_BUSH, SvFgName::NAME_RED_HIBISCUS_BUSH,
    SvFgName::NAME_YELLOW_HIBISCUS_BUSH, SvFgName::NAME_SWEET_OLIVE_BUSH,
};
// the fruits that the maze hides besides the items of the tour
// 0x0090C984
const u16 s_MazeFruits[] = {
    SvFgName::NAME_APPLE,  SvFgName::NAME_ORANGE,  SvFgName::NAME_PEAR,  SvFgName::NAME_PEACH,
    SvFgName::NAME_CHERRY, SvFgName::NAME_COCONUT, SvFgName::NAME_DURIAN, SvFgName::NAME_LEMON,
    SvFgName::NAME_LYCHEE, SvFgName::NAME_MANGO,   SvFgName::NAME_PERSIMMON, SvFgName::NAME_BANANA,
};

// a ground of the maze at a block
struct MazeGround
{
    s32 x;
    s32 z;
    u16 name;
};

// the items of the maze layout: NAME_UNUSED_BUSH becomes a random bush, NAME_BROKEN_JACOBS_LADDER marks a place for a
// hidden item (Setup1C) and is not placed
struct MazeItemsSetter
{
    MazeItemsSetter(Field* field, const FgDataLoader* data, sead::Random* random)
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
            u16 id = entry.id;
            if (id == SvFgName::NAME_UNUSED_BUSH) {
                id = s_MazeBushes[m_pRandom->getU32(8)];
            }
            if (id != SvFgName::NAME_BROKEN_JACOBS_LADDER) {
                bg->SetFg(SvFgName(static_cast<SvFgName::ID>(id)), entry.x, entry.z, 0);
            }
        }
    }

    Field* m_pField;
    const FgDataLoader* m_pData;
    sead::Random* m_pRandom;
    FgDataItem m_Items[FgDataLoader::ITEM_BUFFER_SIZE / sizeof(FgDataItem)];
};

// the ground of a block
inline void SetGround(Field* field, s32 x, s32 z, SvBgName name)
{
    BgInfo* bg = field->GetBg(x, z);
    if (bg != NULL) {
        bg->SetName(name);
    }
}

// the next free place takes an item
inline void PlaceAtNext(Field* field, sead::FixedObjArray<Vector2i, 100>* places, const SvFgName& item)
{
    Vector2i* place = places->front();
    if (place != NULL) {
        field->PlaceItem(item, place->x, place->y, 0);
    }
    places->erase(0);
}
} // namespace

namespace isfactory {
// 0x0070312C slot 0x00
// 0x0070311C (deleting dtor)
isfactory::MazeBuilder::~MazeBuilder()
{
    // empty (in the original too)
}

// 0x007027D0 slot 0x08 (name is ours)
void isfactory::MazeBuilder::SetupBlocks()
{
    const u32 playerCount = Func6E1510();
    ForEachBlock(m_pField->GetBlockBounds(),
                 RandomAcreSetter(m_pField, playerCount < 3 ? s_MazeMapSmall : s_MazeMap, m_pRandom));
    // the grounds of the blocks in the middle
    const MazeGround grounds[] = {
        {2, 1, BG_NAME_GRD_F_M_0}, {3, 1, BG_NAME_GRD_F_M_1}, {4, 1, BG_NAME_GRD_F_M_2},
        {2, 2, BG_NAME_GRD_F_M_3}, {3, 2, BG_NAME_GRD_F_T_0}, {4, 2, BG_NAME_GRD_F_M_4},
    };
    if (m_pField == NULL) {
        return;
    }
    for (u32 i = 0; i < sizeof(grounds) / sizeof(grounds[0]); i++) {
        BgInfo* bg = m_pField->GetBg(grounds[i].x, grounds[i].z);
        if (bg != NULL) {
            bg->SetName(static_cast<SvBgName>(grounds[i].name));
        }
    }
    // the southern row of the middle (smaller with fewer than three players)
    if (playerCount < 3) {
        SetGround(m_pField, 2, 3, BG_NAME_GRD_F_0);
        SetGround(m_pField, 3, 3, BG_NAME_GRD_F_1);
        SetGround(m_pField, 4, 3, BG_NAME_GRD_F_2);
    } else {
        SetGround(m_pField, 2, 3, BG_NAME_GRD_F_M_5);
        SetGround(m_pField, 3, 3, BG_NAME_GRD_F_M_6);
        SetGround(m_pField, 4, 3, BG_NAME_GRD_F_M_7);
    }
}

// 0x00527994 slot 0x10 (name is ours)
void isfactory::MazeBuilder::Setup10()
{
    // (the call became a fall-through into SetEdgeAttributes)
    SetEdgeAttributes(m_pField, 2, 4);
}

// 0x0070297C slot 0x18 (name is ours)
void isfactory::MazeBuilder::SetupItems()
{
    // one of the four maze layouts
    sead::FixedSafeString<64> path;
    {
        sead::StringFormatter formatter(&path);
        formatter.append("FgDataTour/H%d.bin").appendValue(m_pRandom->getU32(4)).flush();
    }
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    m_pData = mgr->GetFgData(path.getCString());
    if (m_pData == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), MazeItemsSetter(m_pField, m_pData, m_pRandom));
}

// 0x00702C80 slot 0x1C (name is ours)
void isfactory::MazeBuilder::Setup1C()
{
    if (m_pData == NULL) {
        return;
    }
    // the places for hidden items (NAME_BROKEN_JACOBS_LADDER in the layout) in a random order
    sead::FixedObjArray<Vector2i, 100> places;
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
            if (items[i].id == SvFgName::NAME_BROKEN_JACOBS_LADDER) {
                const Vector2i unit(items[i].x, items[i].z);
                places.pushBack(FdInfo::ToUnit(*it, unit));
            }
        }
    }
    places.shuffle(m_pRandom);
    // the fruits for the remaining places
    SvFgNameSet fruits;
    fruits.Clear();
    for (u32 i = 0; i < sizeof(s_MazeFruits) / sizeof(s_MazeFruits[0]); i++) {
        const SvFgName fruit(static_cast<SvFgName::ID>(s_MazeFruits[i]));
        Func302F80(&fruits, &fruit);
    }
    u32 rest = Func6E1510() > 2 ? 50 : 30;
    // each item of the tour as often as the level says
    u32 count = 3;
    const u8 level = Func76F7E4(&m_pTourData->m_Seed);
    if (level == 0) {
        count = 7;
    } else if (level == 1) {
        count = 4;
    }
    const u16* tourItems = m_pTourData->m_Items;
    if (tourItems != NULL) {
        for (s32 i = 0; i < 12; i++) {
            if (tourItems[i] == SvFgName::NAME_EMPTY) {
                continue;
            }
            const SvFgName item(static_cast<SvFgName::ID>(tourItems[i]));
            RemoveFgName(&fruits, &item);
            for (u32 j = 0; j < count; j++) {
                PlaceAtNext(m_pField, &places, item);
            }
            rest -= count;
        }
    }
    for (u32 i = 0; i < rest; i++) {
        const SvFgName fruit = Func1AD2E0(&fruits, m_pRandom->getU32());
        PlaceAtNext(m_pField, &places, fruit);
    }
}

} // namespace isfactory
