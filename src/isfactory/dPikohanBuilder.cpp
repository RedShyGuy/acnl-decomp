#include "isfactory/dPikohanBuilder.h"
#include "Bs/dBsTourFieldResMgr.h"
#include "field/dField.h"
#include "field/dFieldBuilderUtil.h"
#include "field/field_Extern.h"
#include "isfactory/isfactory_Api.h"
#include "Other/dTourData.h"
#include "Sv/dSvBgName.h"

namespace {
// the acre maps of the tour (7 x 6 kinds; the second one with fewer than three players)
// 0x0090C99C
const u8 s_PikohanMap[42] = {
    83, 75, 73, 73, 73, 74, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 69, 6, 7, 6, 59, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 72, 40, 52, 36, 62, 82,
    81, 79, 79, 79, 79, 79, 80,
};
// 0x0090C9C6
const u8 s_PikohanMapSmall[42] = {
    83, 75, 73, 73, 73, 74, 82,
    83, 69, 6, 7, 6, 59, 82,
    83, 69, 6, 6, 6, 59, 82,
    83, 69, 0, 0, 0, 59, 82,
    83, 72, 40, 52, 36, 62, 82,
    81, 79, 79, 79, 79, 79, 80,
};

// the ground of a block
inline void SetGround(Field* field, s32 x, s32 z, SvBgName name)
{
    BgInfo* bg = field->GetBg(x, z);
    if (bg != NULL) {
        bg->SetName(name);
    }
}
} // namespace

namespace isfactory {
// 0x00704CF0 slot 0x00
// 0x00704CE0 (deleting dtor)
isfactory::PikohanBuilder::~PikohanBuilder()
{
    // empty (in the original too)
}

// 0x00704860 slot 0x08 (name is ours)
void isfactory::PikohanBuilder::SetupBlocks()
{
    if (m_pField == NULL) {
        return;
    }
    // the blocks in the middle (fewer than three players: the southern row is smaller)
    if (Func6E1510() < 3) {
        ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, s_PikohanMapSmall, m_pRandom));
        SetGround(m_pField, 2, 1, BG_NAME_GRD_F_M_0);
        SetGround(m_pField, 3, 1, BG_NAME_GRD_F_T_0);
        SetGround(m_pField, 4, 1, BG_NAME_GRD_F_M_1);
        SetGround(m_pField, 2, 2, BG_NAME_GRD_F_M_2);
        SetGround(m_pField, 3, 2, BG_NAME_GRD_F_M_3);
        SetGround(m_pField, 4, 2, BG_NAME_GRD_F_M_4);
        SetGround(m_pField, 2, 3, BG_NAME_GRD_F_0);
        SetGround(m_pField, 3, 3, BG_NAME_GRD_F_1);
        SetGround(m_pField, 4, 3, BG_NAME_GRD_F_2);
    } else {
        ForEachBlock(m_pField->GetBlockBounds(), RandomAcreSetter(m_pField, s_PikohanMap, m_pRandom));
        SetGround(m_pField, 2, 1, BG_NAME_GRD_F_M_0);
        SetGround(m_pField, 3, 1, BG_NAME_GRD_F_M_5);
        SetGround(m_pField, 4, 1, BG_NAME_GRD_F_M_1);
        SetGround(m_pField, 2, 2, BG_NAME_GRD_F_M_6);
        SetGround(m_pField, 3, 2, BG_NAME_GRD_F_T_0);
        SetGround(m_pField, 4, 2, BG_NAME_GRD_F_M_7);
        SetGround(m_pField, 2, 3, BG_NAME_GRD_F_M_2);
        SetGround(m_pField, 3, 3, BG_NAME_GRD_F_M_3);
        SetGround(m_pField, 4, 3, BG_NAME_GRD_F_M_4);
    }
}

// 0x00704CD0 slot 0x10 (name is ours)
void isfactory::PikohanBuilder::Setup10()
{
    SetEdgeAttributes(m_pField, 2, 4);
}

// 0x00704B18 slot 0x18 (name is ours)
void isfactory::PikohanBuilder::SetupItems()
{
    BsTourFieldResMgr* mgr = BsTourFieldResMgr::GetInstance();
    if (mgr == NULL) {
        return;
    }
    FgDataLoader* data;
    switch (Func76F7E4(&m_pTourData->m_Seed)) {
    case 0:
        data = mgr->GetFgData("FgDataTour/G0.bin");
        break;
    case 1:
        data = mgr->GetFgData("FgDataTour/G1.bin");
        break;
    case 2:
        data = mgr->GetFgData("FgDataTour/G2.bin");
        break;
    default:
        return;
    }
    if (data == NULL) {
        return;
    }
    ForEachBlock(m_pField->GetBlockBounds(), BlockItemsSetter(m_pField, data));
}

} // namespace isfactory
