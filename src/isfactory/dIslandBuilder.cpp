#include "isfactory/dIslandBuilder.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/dFieldFactory.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"

namespace isfactory {
// 0x00703954 slot 0x08 (name is ours)
void isfactory::IslandBuilder::SetupBlocks()
{
    const u8 state = m_pTourData->m_State;
    const u8 oceanSide = m_pTourData->m_OceanSide;
    u16 kinds[ACRE_KIND_NUM];
    GetAcreKinds(kinds, state, oceanSide, m_pRandom);
    u8 map[FieldFactory::ACRE_MAP_WIDTH * FieldFactory::ACRE_MAP_HEIGHT];
    FieldFactory::ReplacePlaceholders(map, kinds, s_IslandMap);
    FieldFactory::ReplaceRandomCell(map, 96, 97, m_pRandom);
    ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, map, m_pRandom));
}

} // namespace isfactory
