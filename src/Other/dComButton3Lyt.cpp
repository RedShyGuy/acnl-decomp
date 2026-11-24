#include "Other/dComButton3Lyt.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x2500;
// the layer of the layout
const u8 LAYOUT_LAYER = 155;
} // namespace

// 0x0022C5FC
bool ComButton3Lyt::Setup()
{
    m_Layout.m_pArcReader = &m_Reader;
    m_Layout.Build("btn_pos.bclyt", NULL, CMD_CACHE_SIZE);
    m_Layout.m_Layer = LAYOUT_LAYER;
    m_Control.Initialize();
    m_Control.m_pLayout = &m_Layout;
    nw::lyt::Pane* right = m_Layout.FindPane("N_cmnbtn_pos_R");
    nw::lyt::Pane* center = m_Layout.FindPane("N_cmnbtn_pos_C");
    m_Control.SetPanes(m_Layout.FindPane("N_cmnbtn_pos_L"), center, right);
    return true;
}

// 0x0022C6B4
bool ComButton3Lyt::Finalize()
{
    m_Control.Finalize();
    m_Reader.Clear();
    return true;
}

// 0x0022C6D4
void ComButton3Lyt::Update()
{
    m_Control.Update();
}

// 0x0022C750 slot 0x00
// 0x0022C6DC (deleting dtor)
ComButton3Lyt::~ComButton3Lyt()
{
    // (only the members are destroyed)
}
