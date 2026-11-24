#include "Other/dMyDesignDragIconNode.h"

#include "Other/dFormatBuffer.h"
#include "Other/dPaneVisible.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvMyDesign.h"
#include "nn/gxlow/CTR/CTR_Api.h"
#include "nw/gfx/res/gfx_ResTexture.h"
#include "nw/lyt/lyt_Material.h"
#include "nw/lyt/lyt_Picture.h"
#include "nw/lyt/lyt_TexMap.h"
#include "nw/ut/ut_Color8.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"

namespace {
// the command cache size of the layout
const u32 CMD_CACHE_SIZE = 0x1000;
// the layer of the layout
const u8 LAYOUT_LAYER = 158;
} // namespace

// 0x00320C88
void MyDesignDragIconNode::SetWearableMarkVisible(bool visible)
{
    if (m_pWearableMark != NULL) {
        SetFlagVisible(m_pWearableMark, visible);
    }
}

// 0x00320CA8
void MyDesignDragIconNode::UpdatePicture()
{
    UpdatePicture(&m_pTexture);
}

// 0x00320CB0
void MyDesignDragIconNode::UpdatePicture(const nw::gfx::res::ResImageTextureData* const* /* unused */)
{
    nw::lyt::Material* material = m_pPicture->GetMaterial(0);
    nw::lyt::TexMap texMap = material->GetTexMapAry()[0];
    material->m_Colors[0] = nw::ut::Color8(0, 0, 0, 255);
    const nw::gfx::res::ResImageTextureData* texture = m_pTexture;
    texMap.m_Size = nw::lyt::TexSize(texture->width, texture->height);
    texMap.Update();
    texMap.m_RealSize = nw::lyt::TexSize(texture->width, texture->height);
    texMap.Update();
    texMap.m_Bits.format = nw::lyt::TEXFORMAT_RGBA4;
    texMap.Update();
    const nw::gfx::res::ResImageData* image = texture->image.To<nw::gfx::res::ResImageData>();
    const void* address = image->pLocationAddress != NULL ? image->pLocationAddress : image->data.ToPtr();
    texMap.m_PhysicalAddress = nngxGetPhysicalAddr(reinterpret_cast<uptr>(address));
    material->GetTexMapAry()[0] = texMap;
    material->SetCacheDirty();
}

// 0x00320DC4
void MyDesignDragIconNode::SetVisible(bool visible)
{
    SetFlagVisible(m_IconLayout.GetRootPane(), visible);
    m_IconLayout.Animate();
}

// 0x00320DE0
void MyDesignDragIconNode::Refresh()
{
    if (m_pDesign != NULL && m_pTexture != NULL) {
        SetFlagVisible(m_IconLayout.GetRootPane(), true);
        m_IconLayout.Animate();
        m_IsActive = true;
        m_pDesign->DrawToTextureByPairs(m_pTexture, true);
        UpdatePicture(&m_pTexture);
        const bool isWearable = m_pDesign->IsWearable();
        if (m_pWearableMark != NULL) {
            SetFlagVisible(m_pWearableMark, isWearable);
        }
        return;
    }
    ButtonActionNode::Select(false);
    SetFlagVisible(m_IconLayout.GetRootPane(), false);
    m_IconLayout.Animate();
    m_IsActive = false;
}

// 0x00320E94
void MyDesignDragIconNode::ApplyAnim(bool isFirst)
{
    if (m_pAnim != NULL) {
        m_IconLayout.BindAnimation(m_pAnim);
        if (isFirst) {
            m_pAnim->SetFrame(0.0f);
        } else {
            m_pAnim->SetLastFrame();
        }
        m_IconLayout.Animate();
        m_IconLayout.UnbindAnimation(m_pAnim);
    }
    m_IsActive = !isFirst;
    m_IsDropTarget = !isFirst;
}

// 0x00320F08
void MyDesignDragIconNode::Setup(s32 index, ssys::ma::lyt::ArcResAccReader* reader)
{
    m_IconLayout.m_pArcReader = reader;
    g_FormatBuffer.format("%s.bclyt", "m_design_icon");
    m_IconLayout.Build(g_FormatBuffer.getCString(), NULL, CMD_CACHE_SIZE);
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
    SetFlagVisible(m_pWearableMark, false);
    m_pPicture = m_IconLayout.FindPicture("P_texture_00");
    SetSounds(true);
}

// 0x00321038 slot 0x50
void MyDesignDragIconNode::SetSounds(bool isOn)
{
    SetSe(SE_SYS_ITEM_DESIGN_ACTIVE);
    SetSoundEnabled(true);
    m_SoundMode = true;
    SetSeOk(isOn ? SE_SYS_ITEM_ICON_SELECTED : SE_SYS_ITEM_ICON_MARKING_ON);
    SetOkSoundEnabled(true);
}

// 0x0032106C
MyDesignDragIconNode::MyDesignDragIconNode()
    : m_pTexture(NULL), m_pWearableMark(NULL), m_pPicture(NULL), m_pAnim(NULL), m_pDesign(NULL)
{
    m_pLayout = &m_IconLayout;
}

// 0x003210F8 slot 0x00
// 0x003210B0 (deleting dtor)
MyDesignDragIconNode::~MyDesignDragIconNode()
{
    // (only the members are destroyed)
}
