#pragma once

#include "decomp.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dJmpBlock.h"
#include "field/field_Extern.h"
#include "sead/seadRandom.h"
#include "Sv/dSvFgName.h"
#include "Other/dTourData.h"
#include "stage/stage_Extern.h"

namespace isfactory {
// the kinds for the placeholders of an acre map: a random entry of the acre list of the state (and ocean side) of the
// tour (nothing without the BsTourFieldResMgr or its list; inline in the SetupBlocks of the island, peninsula and village)
DECOMP_ALWAYS_INLINE void GetAcreKinds(u16* kinds, u8 state, u8 oceanSide, sead::Random* random)
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    TourAcreList* list;
    switch (state) {
    case 0:
        list = oceanSide != 0 ? mgr->GetAcreList0WithOceanSide() : mgr->GetAcreList0();
        break;
    case 1:
        list = mgr->GetAcreList1();
        break;
    case 2:
        list = mgr->GetAcreList2();
        break;
    default:
        return;
    }
    if (list == NULL) {
        return;
    }
    const u32 num = list->m_Num;
    list->Copy(kinds, random->getU32(num));
}

// the number of the kinds of the placeholders of a source map
const u32 ACRE_KIND_NUM = 20;

// the source acre maps (7 x 6 kinds, 120 = placeholder): the village (ocean side 0 / 1), the peninsula, the island
extern const u8 s_VillageMap0[42];
extern const u8 s_VillageMap1[42];
extern const u8 s_PeninsulaMap[42];
extern const u8 s_IslandMap[42];
// the fossils of two / three parts (the skull first; the fossil layouts of the tours)
extern const u16 s_Fossils2[4][2];
extern const u16 s_Fossils3[11][3];

// buried pitfall seeds on free soil units of all blocks but one (Func6FAD4C), each unit with a chance in percent
DECOMP_ALWAYS_INLINE void PlacePitfalls(Field* field, sead::Random* random, u32 chance)
{
    SvFgName pitfall(SvFgName::NAME_PITFALL_SEED);
    pitfall.SetBuried();
    const FieldRect& blocks = field->GetBlockBounds();
    Vector2i excluded;
    Func6FAD4C(&excluded.x, &excluded.y);
    const FieldRect& units = BgInfo::GetLocalUnitRect();
    for (FieldRect::iterator it = blocks.begin(); it != blocks.end(); ++it) {
        if (!(excluded != *it)) {
            continue;
        }
        JmpBlock* jmpBlock = field->GetJmpBlock((*it).x, (*it).y);
        if (jmpBlock == NULL) {
            continue;
        }
        for (FieldRect::iterator unit = units.begin(); unit != units.end(); ++unit) {
            if (random->getU32() % 100 >= chance) {
                continue;
            }
            if (jmpBlock->GetAttribute((*unit).x, (*unit).y) != FIELD_ATTRIBUTE_SOIL_14) {
                continue;
            }
            const Vector2i position = FdInfo::ToUnit(*it, *unit);
            const SvFgName* item = field->GetFg(position.x, position.y, 0);
            if (item != NULL && item->IsEmpty()) {
                field->PlaceItem(pitfall, position.x, position.y, 0);
            }
        }
    }
}

// the objects of the tour (the slot Setup20): Func1DC370 unless the state is 2, and with the items of the tour type the
// kind 106 at a unit of the block of Func6FAD4C
DECOMP_ALWAYS_INLINE void SetupTourObjects(TourData* tourData, sead::Random* random, s32 unitX, s32 unitZ)
{
    if (tourData->m_State != 2) {
        Unknown1DC370 object;
        object.Func1DC370(random);
    }
    if (tourData->Func76F678(GetTourType(tourData->m_Tour))) {
        Vector2i block;
        Func6FAD4C(&block.x, &block.y);
        const Vector2i position(block);
        const Vector2i unit(unitX, unitZ);
        const Vector2i target = FdInfo::ToUnit(position, unit);
        Func2427C8(target.x, target.y, 106);
    }
}
} // namespace isfactory
