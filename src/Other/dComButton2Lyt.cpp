#include "Other/dComButton2Lyt.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x2500;
// the layer of the layout
const u8 LAYOUT_LAYER = 155;
} // namespace

// 0x0022C3AC
void ComButton2Lyt::ShowBoth()
{
    nw::lyt::Pane* right = m_Layout.FindPane("N_cmnbtn_pos_R");
    m_Control.SetPanes(m_Layout.FindPane("N_cmnbtn_pos_L"), right);
    m_Control.SetVisibleNum(2);
    m_Layout.Animate();
}

// 0x0022C418
void ComButton2Lyt::ShowOne(s32 position)
{
    m_Control.SetPanes(NULL, m_Layout.FindPane(s_PositionNames[position]));
    m_Control.SetVisibleNum(1);
    m_Layout.Animate();
}

// 0x0022C45C
bool ComButton2Lyt::Setup()
{
    m_Layout.m_pArcReader = &m_Reader;
    m_Layout.Build("btn_pos.bclyt", NULL, CMD_CACHE_SIZE);
    m_Layout.m_Layer = LAYOUT_LAYER;
    m_Control.Initialize();
    m_Control.m_pLayout = &m_Layout;
    nw::lyt::Pane* right = m_Layout.FindPane("N_cmnbtn_pos_R");
    m_Control.SetPanes(m_Layout.FindPane("N_cmnbtn_pos_L"), right);
    return true;
}

// 0x0022C4F0
bool ComButton2Lyt::Finalize()
{
    m_Control.Finalize();
    m_Reader.Clear();
    return true;
}

// 0x0022C510
void ComButton2Lyt::Update()
{
    m_Control.Update();
}

// 0x0022C58C slot 0x00
// 0x0022C518 (deleting dtor)
ComButton2Lyt::~ComButton2Lyt()
{
    // (only the members are destroyed)
}
