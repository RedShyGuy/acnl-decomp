#include "ishutfact2/dBasicBuilder.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "Sv/dSvBgName.h"

namespace ishutfact2 {
// 0x00201150 slot 0x00
// 0x001BA61C (deleting dtor)
ishutfact2::BasicBuilder::~BasicBuilder()
{
    // empty (in the original too)
}

// 0x001BA3F0 slot 0x08 (name is ours)
void ishutfact2::BasicBuilder::SetupBlocks()
{
    // the hut has one block (the second name is not used)
    const u16 names[] = {BG_NAME_IDR_ROOM_N_P_C, 0xFFFF};
    ForEachBlock(m_pField->GetBlockBounds(), BlockNamesSetter(m_pField, names));
}

// 0x001BA570 slot 0x0C (name is ours)
void ishutfact2::BasicBuilder::SetupHeights()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockHeightsSetter(m_pField));
}

// 0x001BA4B8 slot 0x10 (name is ours)
void ishutfact2::BasicBuilder::Setup10()
{
    // empty (in the original too)
}

// 0x001BA4C4 slot 0x14 (name is ours)
void ishutfact2::BasicBuilder::ClearItems()
{
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsCleaner(m_pField));
}

// 0x001BA4B0 slot 0x18 (name is ours)
void ishutfact2::BasicBuilder::SetupItems()
{
    // empty (in the original too)
}

// 0x001BA4B4 slot 0x1C (name is ours)
void ishutfact2::BasicBuilder::Setup1C()
{
    // empty (in the original too)
}

// 0x001BA4BC slot 0x20 (name is ours)
void ishutfact2::BasicBuilder::Setup20()
{
    // empty (in the original too)
}

// 0x001BA4C0 slot 0x24 (name is ours)
void ishutfact2::BasicBuilder::Setup24()
{
    // empty (in the original too)
}

// 0x001BA3EC slot 0x28 (name is ours)
void ishutfact2::BasicBuilder::SetupGrass()
{
    // empty (in the original too)
}

} // namespace ishutfact2
