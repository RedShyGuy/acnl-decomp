#include "isfactory/dBasicBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dFdInfo.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "isfactory/isfactory_Internal.h"
#include "Other/dTourData.h"
#include "stage/stage_Extern.h"

namespace isfactory {
// 0x007033FC slot 0x0C (name is ours)
void isfactory::BasicBuilder::SetupHeights()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockHeightsSetter(m_pField));
}

// 0x0070326C slot 0x10 (name is ours)
void isfactory::BasicBuilder::Setup10()
{
    // empty (in the original too)
}

// 0x00703350 slot 0x14 (name is ours)
void isfactory::BasicBuilder::ClearItems()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsCleaner(m_pField));
}

// 0x00703134 slot 0x18 (name is ours)
void isfactory::BasicBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data = mgr->GetFgData("FgDataTour/Default.bin");
    if (data == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsSetter(m_pField, data));
}

// 0x00703268 slot 0x1C (name is ours)
void isfactory::BasicBuilder::Setup1C()
{
    // empty (in the original too)
}

// 0x00703270 slot 0x20 (name is ours)
void isfactory::BasicBuilder::Setup20()
{
    SetupTourObjects(m_pTourData, m_pRandom, 7, 6);
}

// 0x0070334C slot 0x24 (name is ours)
void isfactory::BasicBuilder::Setup24()
{
    // empty (in the original too)
}

// 0x00703130 slot 0x28 (name is ours)
void isfactory::BasicBuilder::SetupGrass()
{
    // empty (in the original too)
}

} // namespace isfactory
