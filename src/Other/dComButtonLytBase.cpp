#include "Other/dComButtonLytBase.h"

#include "ssys/ma/lyt/dLayoutMgr.h"

// 0x00952668
const char* ComButtonLytBase::s_PositionNames[3] = {"N_cmnbtn_pos_L", "N_cmnbtn_pos_C", "N_cmnbtn_pos_R"};

// 0x002B82F0 slot 0x00
// 0x002B82C8 (deleting dtor)
ComButtonLytBase::~ComButtonLytBase()
{
    // (only the members are destroyed)
}

// 0x002B827C
void ComButtonLytBase::Draw()
{
    m_Layout.Animate();
    ssys::ma::lyt::LayoutMgr::Get()->Register(&m_Layout, true);
}

// 0x002B82A0
bool ComButtonLytBase::ReadArchive()
{
    return m_Reader.ReadArc("Layout/common/btn_pos.arc");
}
