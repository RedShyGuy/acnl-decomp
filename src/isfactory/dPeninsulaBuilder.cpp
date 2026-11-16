#include "isfactory/dPeninsulaBuilder.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dFieldFactory.h"
#include "isfactory/isfactory_Api.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"

namespace isfactory {
// 0x0070529C slot 0x08 (name is ours)
void isfactory::PeninsulaBuilder::SetupBlocks()
{
    // until a block in the middle can get the kind 1
    u8 map[FieldFactory::ACRE_MAP_WIDTH * FieldFactory::ACRE_MAP_HEIGHT];
    do {
        const u8 state = m_pTourData->m_State;
        const u8 oceanSide = m_pTourData->m_OceanSide;
        u16 kinds[ACRE_KIND_NUM];
        GetAcreKinds(kinds, state, oceanSide, m_pRandom);
        FieldFactory::ReplacePlaceholders(map, kinds, s_PeninsulaMap);
    } while (!ReplaceRandomInnerCell(map, 0, 1, m_pRandom));
    FieldFactory::ReplaceRandomCell8To19(map, m_pRandom);
    FieldFactory::ReplaceRandomCell34Or35Or39(map, m_pRandom);
    FieldFactory::SetNeighbors(map);
    ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, map, m_pRandom));
}

// 0x007053F0 slot 0x10 (name is ours)
void isfactory::PeninsulaBuilder::Setup10()
{
    SetEdgeAttributes(m_pField, 1, 5);
}

} // namespace isfactory
