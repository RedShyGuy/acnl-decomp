#include "Other/dMyDesignProDragIconNode.h"

#include "Other/dPaneVisible.h"
#include "nw/lyt/lyt_Picture.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x1000;
// the layer of the layout
const u8 LAYOUT_LAYER = 158;
} // namespace

// 0x0095456C
const char* MyDesignProDragIconNode::s_LayoutNames[4] = {
    "edit_pro_1616.bclyt",
    "edit_pro_1632.bclyt",
    "m_design_icon.bclyt",
    "edit_pro_3248.bclyt",
};

// 0x00335548
void MyDesignProDragIconNode::Setup(s32 index, s32 kind, ssys::ma::lyt::ArcResAccReader* reader)
{
    m_IconLayout.m_pArcReader = reader;
    m_IconLayout.Build(s_LayoutNames[kind], NULL, CMD_CACHE_SIZE);
    m_pLayout->m_Layer = LAYOUT_LAYER;
    ButtonActionNode::Info info;
    info.pane = NULL;
    info.group = NULL;
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.input = 0;
    info.index = 0;
    info.cursorCallback = NULL;
    info.pane = m_IconLayout.FindPane("B_dsgn");
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.index = index;
    Initialize(info);
    m_pWearableMark = m_IconLayout.FindPane("P_pro_00");
    if (m_pWearableMark != NULL) {
        SetFlagVisible(m_pWearableMark, false);
    }
    m_pPicture = m_IconLayout.FindPicture("P_texture_00");
    if (kind == KIND_32X48) {
        // 0x00A1978C (guard 0x00954418)
        static const nn::math::VEC2 s_ProTexCoords3248[4] = {
            nn::math::VEC2(0.0f, 0.0f),
            nn::math::VEC2(1.0f, 0.0f),
            nn::math::VEC2(0.0f, 0.75f),
            nn::math::VEC2(1.0f, 0.75f),
        };
        m_pPicture->SetTexCoord(0, s_ProTexCoords3248);
    } else {
        // 0x00A197AC (guard 0x0095441C)
        static const nn::math::VEC2 s_ProTexCoords[4] = {
            nn::math::VEC2(0.0f, 0.0f),
            nn::math::VEC2(1.0f, 0.0f),
            nn::math::VEC2(0.0f, 1.0f),
            nn::math::VEC2(1.0f, 1.0f),
        };
        m_pPicture->SetTexCoord(0, s_ProTexCoords);
    }
}

// 0x00335740
MyDesignProDragIconNode::MyDesignProDragIconNode() : m_Kind(KIND_ICON)
{
}

// 0x003357E0 slot 0x00
// 0x00335798 (deleting dtor)
MyDesignProDragIconNode::~MyDesignProDragIconNode()
{
    // (only the members are destroyed)
}
