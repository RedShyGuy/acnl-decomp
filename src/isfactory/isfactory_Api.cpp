#include "isfactory/isfactory_Api.h"
#include "field/dField.h"
#include "field/dFieldFactory.h"
#include "field/dFieldRect.h"
#include "field/dJmpBlock.h"
#include "isfactory/dBalloonBuilder.h"
#include "isfactory/dBorrowingRaceBuilder.h"
#include "isfactory/dDivingBuilder.h"
#include "isfactory/dFishingBuilder.h"
#include "isfactory/dFishingIslandBuilder.h"
#include "isfactory/dFlowerPickingBuilder.h"
#include "isfactory/dFossilDigBuilder.h"
#include "isfactory/dHideAndSeekBuilder.h"
#include "isfactory/dInsectCatchingButterflyBuilder.h"
#include "isfactory/dInsectCatchingDefaultBuilder.h"
#include "isfactory/dInsectCatchingReduceTreeBuilder.h"
#include "isfactory/dInsectCatchingRiseStoneBuilder.h"
#include "isfactory/dInsectCatchingRiseTreeBuilder.h"
#include "isfactory/dMazeBuilder.h"
#include "isfactory/dMiningBuilder.h"
#include "isfactory/dPikohanBuilder.h"
#include "isfactory/dTreasureHuntBuilder.h"
#include "isfactory/isfactory_Internal.h"
#include "sead/seadNew.h"
#include "stage/stage_Extern.h"
#include "Sv/dSvFgName.h"
#include "field/field_Extern.h"
#include "Other/dTourData.h"
#include "sead/seadObjArray.h"
#include "sead/seadRandom.h"
#include <cstring>

namespace {
// the four keys from first on in a random order
inline void ShuffleKeys(u8* keys, u32 first, sead::Random* random)
{
    sead::FixedObjArray<u32, 4> list;
    for (u32 i = 0; i < 4; i++) {
        list.pushBack(first + i);
    }
    list.shuffle(random);
    for (u32 i = 0; i < 4; i++) {
        keys[i] = static_cast<u8>(*list.get(i));
    }
}
} // namespace

namespace isfactory {
// 0x00872997
extern const u8 s_VillageMap0[42] = {
    83, 77, 73, 73, 73, 73, 74,
    83, 120, 120, 120, 120, 120, 59,
    83, 120, 120, 120, 120, 120, 59,
    83, 120, 120, 120, 120, 120, 59,
    83, 120, 120, 120, 120, 120, 62,
    81, 79, 79, 79, 79, 79, 79,
};
// 0x008729C1
extern const u8 s_VillageMap1[42] = {
    75, 73, 73, 73, 73, 76, 82,
    69, 120, 120, 120, 120, 120, 82,
    69, 120, 120, 120, 120, 120, 82,
    69, 120, 120, 120, 120, 120, 82,
    72, 120, 120, 120, 120, 120, 82,
    79, 79, 79, 79, 79, 79, 80,
};
// 0x008729EB
extern const u8 s_PeninsulaMap[42] = {
    75, 73, 73, 73, 73, 73, 74,
    69, 120, 120, 120, 120, 120, 59,
    69, 120, 120, 120, 120, 120, 59,
    69, 120, 120, 120, 120, 120, 59,
    72, 120, 120, 120, 120, 120, 62,
    81, 79, 79, 79, 79, 79, 80,
};
// 0x00872A15
extern const u8 s_IslandMap[42] = {
    84, 85, 85, 85, 85, 85, 86,
    83, 120, 120, 120, 120, 120, 82,
    83, 120, 120, 120, 120, 120, 82,
    83, 120, 120, 120, 120, 120, 82,
    83, 120, 120, 120, 120, 120, 82,
    81, 79, 79, 79, 79, 79, 80,
};

// 0x00872A40
extern const u16 s_Fossils2[4][2] = {
    {SvFgName::NAME_MAMMOTH_SKULL, SvFgName::NAME_MAMMOTH_TORSO},
    {SvFgName::NAME_SABERTOOTH_SKULL, SvFgName::NAME_SABERTOOTH_TORSO},
    {SvFgName::NAME_ICHTHYO_SKULL, SvFgName::NAME_ICHTHYO_TORSO},
    {SvFgName::NAME_RAPTOR_SKULL, SvFgName::NAME_RAPTOR_TORSO},
};
// 0x00872A50
extern const u16 s_Fossils3[11][3] = {
    {SvFgName::NAME_T_REX_SKULL, SvFgName::NAME_T_REX_TORSO, SvFgName::NAME_T_REX_TAIL},
    {SvFgName::NAME_TRICERA_SKULL, SvFgName::NAME_TRICERA_TORSO, SvFgName::NAME_TRICERA_TAIL},
    {SvFgName::NAME_ANKYLO_SKULL, SvFgName::NAME_ANKYLO_TORSO, SvFgName::NAME_ANKYLO_TAIL},
    {SvFgName::NAME_APATO_SKULL, SvFgName::NAME_APATO_TORSO, SvFgName::NAME_APATO_TAIL},
    {SvFgName::NAME_DIMETRODON_SKULL, SvFgName::NAME_DIMETRODON_TORSO, SvFgName::NAME_DIMETRODON_TAIL},
    {SvFgName::NAME_IGUANODON_SKULL, SvFgName::NAME_IGUANODON_TORSO, SvFgName::NAME_IGUANODON_TAIL},
    {SvFgName::NAME_PACHYSAURUS_SKULL, SvFgName::NAME_PACHYSAURUS_TORSO, SvFgName::NAME_PACHYSAURUS_TAIL},
    {SvFgName::NAME_PARASAUR_SKULL, SvFgName::NAME_PARASAUR_TORSO, SvFgName::NAME_PARASAUR_TAIL},
    {SvFgName::NAME_PLESIO_SKULL, SvFgName::NAME_PLESIO_NECK, SvFgName::NAME_PLESIO_TORSO},
    {SvFgName::NAME_STEGO_SKULL, SvFgName::NAME_STEGO_TORSO, SvFgName::NAME_STEGO_TAIL},
    {SvFgName::NAME_STYRACO_SKULL, SvFgName::NAME_STYRACO_TORSO, SvFgName::NAME_STYRACO_TAIL},
};


// 0x00707DBC (name is ours)
void GetHutFossilKeys(u8* keys, TourData* tourData, sead::Random* random)
{
    std::memset(keys, 0, 12);
    switch (Func76F7E4(&tourData->m_Seed)) {
    case 0:
        ShuffleKeys(keys, 0, random);
        break;
    case 1:
        ShuffleKeys(keys, 4, random);
        break;
    case 2:
        ShuffleKeys(keys, 8, random);
        break;
    default:
        break;
    }
}

// 0x007034A8 (name is ours)
FieldBuilder* CreateBuilder(sead::Heap* heap, Field* field, sead::Random* random, TourData* tourData)
{
    if (heap == NULL) {
        return NULL;
    }
    FieldBuilder* builder = NULL;
    switch (GetTourType(tourData->m_Tour)) {
    case TOUR_TYPE_HIDE_AND_SEEK:
        builder = new (heap, 4) HideAndSeekBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_FLOWER_PICKING:
        builder = new (heap, 4) FlowerPickingBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_FISHING: {
        // (the kinds 5 and 6 are fished on the island)
        const s32 kind = Func76FB3C(&tourData->m_Seed);
        if (kind == 5 || kind == 6) {
            builder = new (heap, 4) FishingIslandBuilder(field, random, tourData);
        } else {
            builder = new (heap, 4) FishingBuilder(field, random, tourData);
        }
        break;
    }
    case TOUR_TYPE_INSECT_CATCHING: {
        const s32 kind = Func76FB7C(&tourData->m_Seed);
        if (kind == 1) {
            builder = new (heap, 4) InsectCatchingButterflyBuilder(field, random, tourData);
        } else if (kind == 2 || kind == 5 || kind == 7) {
            builder = new (heap, 4) InsectCatchingRiseTreeBuilder(field, random, tourData);
        } else if (kind == 8) {
            builder = new (heap, 4) InsectCatchingRiseStoneBuilder(field, random, tourData);
        } else if (kind == 4 || kind == 6) {
            builder = new (heap, 4) InsectCatchingReduceTreeBuilder(field, random, tourData);
        } else {
            builder = new (heap, 4) InsectCatchingDefaultBuilder(field, random, tourData);
        }
        break;
    }
    case TOUR_TYPE_BALLOON:
        builder = new (heap, 4) BalloonBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_TREASURE_HUNT:
        builder = new (heap, 4) TreasureHuntBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_PIKOHAN:
        builder = new (heap, 4) PikohanBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_MAZE:
        builder = new (heap, 4) MazeBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_MINING:
        builder = new (heap, 4) MiningBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_FOSSIL_DIG:
        builder = new (heap, 4) FossilDigBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_DIVING:
        builder = new (heap, 4) DivingBuilder(field, random, tourData);
        break;
    case TOUR_TYPE_BORROWING_RACE:
        builder = new (heap, 4) BorrowingRaceBuilder(field, random, tourData);
        break;
    default:
        break;
    }
    return builder;
}

// 0x00706EB8 (name is ours)
bool ReplaceRandomInnerCell(u8* map, u8 kind, u8 newKind, sead::Random* random)
{
    const FieldRect blocks(Vector2i(2, 2), Vector2i(4, 3));
    u32 num = 0;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (map[(*it).y * FieldFactory::ACRE_MAP_WIDTH + (*it).x] == kind) {
            num++;
        }
    }
    if (num == 0) {
        return false;
    }
    const u32 index = random->getU32(num);
    u32 i = 0;
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        u8& cell = map[(*it).y * FieldFactory::ACRE_MAP_WIDTH + (*it).x];
        if (cell == kind) {
            if (i == index) {
                cell = newKind;
                return true;
            }
            i++;
        }
    }
    return true;
}

namespace {
// the attribute of an edge unit is changed: soil -> 0x32, sand -> 0x80
inline void ChangeEdgeAttribute(JmpBlock* block, s32 unitX, s32 unitZ)
{
    const FieldAttribute attribute = block->GetAttribute(unitX, unitZ);
    if (attribute == FIELD_ATTRIBUTE_SOIL_14) {
        block->SetAttribute(unitX, unitZ, FIELD_ATTRIBUTE_UNKNOWN_32);
    } else if (attribute == FIELD_ATTRIBUTE_SAND_16) {
        block->SetAttribute(unitX, unitZ, FIELD_ATTRIBUTE_UNKNOWN_80);
    }
}
} // namespace

// 0x005279A4 (name is ours)
void SetEdgeAttributes(Field* field, s32 left, s32 right)
{
    if (field == NULL) {
        return;
    }
    for (u32 z = 0; z < static_cast<u32>(field->GetBlockHeight()); z++) {
        JmpBlock* block = field->GetJmpBlock(left, z);
        if (block == NULL) {
            continue;
        }
        for (s32 i = 0; i < static_cast<s32>(UNIT_BASE_NUM); i++) {
            ChangeEdgeAttribute(block, 0, i);
        }
    }
    for (u32 z = 0; z < static_cast<u32>(field->GetBlockHeight()); z++) {
        JmpBlock* block = field->GetJmpBlock(right, z);
        if (block == NULL) {
            continue;
        }
        for (s32 i = 0; i < static_cast<s32>(UNIT_BASE_NUM); i++) {
            ChangeEdgeAttribute(block, UNIT_BASE_NUM - 1, i);
        }
    }
    for (u32 x = 0; x < static_cast<u32>(field->GetBlockWidth()); x++) {
        JmpBlock* block = field->GetJmpBlock(x, 1);
        if (block == NULL) {
            continue;
        }
        for (s32 i = 0; i < static_cast<s32>(UNIT_BASE_NUM); i++) {
            ChangeEdgeAttribute(block, i, 0);
        }
    }
}
} // namespace isfactory
