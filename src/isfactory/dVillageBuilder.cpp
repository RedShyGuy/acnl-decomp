#include "isfactory/dVillageBuilder.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dFieldFactory.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"

namespace isfactory {
// 0x00704CF4 slot 0x08 (name is ours)
void isfactory::VillageBuilder::SetupBlocks()
{
    // the ocean side of the village at random
    const u8 oceanSide = static_cast<u8>(m_pRandom->getU32(2));
    TourData::s_pInstance->m_OceanSide = oceanSide;
    // until a block in the middle can get the kind 1
    u8 map[FieldFactory::ACRE_MAP_WIDTH * FieldFactory::ACRE_MAP_HEIGHT];
    do {
        const u8 state = m_pTourData->m_State;
        u16 kinds[ACRE_KIND_NUM];
        GetAcreKinds(kinds, state, oceanSide, m_pRandom);
        FieldFactory::ReplacePlaceholders(map, kinds, oceanSide != 0 ? s_VillageMap1 : s_VillageMap0);
    } while (!ReplaceRandomInnerCell(map, 0, 1, m_pRandom));
    FieldFactory::ReplaceRandomCell8To19(map, m_pRandom);
    FieldFactory::ReplaceRandomCell34Or35Or39(map, m_pRandom);
    if (oceanSide != 0) {
        FieldFactory::ReplaceRandomCell43Or38(map, m_pRandom);
    } else {
        FieldFactory::ReplaceRandomCell44Or42(map, m_pRandom);
    }
    FieldFactory::SetNeighbors(map);
    ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, map, m_pRandom));
}

// 0x00704EB4 slot 0x10 (name is ours)
void isfactory::VillageBuilder::Setup10()
{
    if (m_pField != NULL) {
        Func6FAA84(m_pField);
    }
}

} // namespace isfactory
