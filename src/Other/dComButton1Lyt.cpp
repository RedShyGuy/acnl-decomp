#include "Other/dComButton1Lyt.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x2500;
// the layer of the layout
const u8 LAYOUT_LAYER = 155;
} // namespace

// 0x0022C23C
bool ComButton1Lyt::Setup(s32 position)
{
    m_Layout.m_pArcReader = &m_Reader;
    m_Layout.Build("btn_pos.bclyt", NULL, CMD_CACHE_SIZE);
    m_Layout.m_Layer = LAYOUT_LAYER;
    m_Control.Initialize();
    m_Control.m_pLayout = &m_Layout;
    m_Control.SetPane(m_Layout.FindPane(s_PositionNames[position]));
    return true;
}

// 0x0022C2A8
bool ComButton1Lyt::Finalize()
{
    m_Control.Finalize();
    m_Reader.Clear();
    return true;
}

// 0x002CD740 (a nop entry that falls into ControlButtonBase::Update)
void ComButton1Lyt::Update()
{
    m_Control.Update();
}

// 0x0022C33C slot 0x00
// 0x0022C2C8 (deleting dtor)
ComButton1Lyt::~ComButton1Lyt()
{
    // (only the members are destroyed)
}
