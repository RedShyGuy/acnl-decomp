#include "Other/dReactionDragIconNode.h"

#include "Other/dFormatBuffer.h"
#include "Other/dPaneVisible.h"
#include "Sound/dSoundLabel.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "script/dScriptExtern.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "stage/stage_Api.h"

namespace {
// the command cache size and the layer of the layout
const u32 CMD_CACHE_SIZE = 0x1000;
const u8 LAYOUT_LAYER = 158;
} // namespace

// 0x0032155C
ReactionDragIconNode::ReactionDragIconNode() : m_pReactionAnim(NULL), m_pGroupAnim(NULL)
{
    m_pLayout = &m_IconLayout;
}

// 0x003215B4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
// 0x00321590 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
ReactionDragIconNode::~ReactionDragIconNode()
{
    // (only the members are destroyed)
}

// 0x003212D8
void ReactionDragIconNode::SetGroupAnimation(bool rewind)
{
    if (m_pGroupAnim == NULL) {
        return;
    }
    m_IconLayout.BindAnimation(m_pGroupAnim);
    if (rewind) {
        m_pGroupAnim->SetFrame(0.0f);
    } else {
        m_pGroupAnim->SetLastFrame();
    }
    m_IconLayout.Animate();
    m_IconLayout.UnbindAnimation(m_pGroupAnim);
}

// 0x00321340
void ReactionDragIconNode::UpdateIcon()
{
    bool isShown = true;
    if (stage::GetCurrent() == stage::NAME_AMIIBO_CAMERA && Func71AD8C(&GetGardenPlus()->data.players) == NULL) {
        isShown = false;
        for (u32 i = 0; i < 4; i++) {
            if (GetGardenPlus()->data.players.Get(static_cast<u8>(i)) != NULL) {
                isShown = true;
                break;
            }
        }
    }
    if (isShown && g_pSvCurrentPlayer != NULL && m_pReactionAnim != NULL) {
        const s32 index = Func7173E4(&g_pSvCurrentPlayer->emotes, m_Index);
        if (index != -1) {
            SetFlagVisible(GetIconPane(), true);
            m_IsActive = true;
            m_IconLayout.BindAnimation(m_pReactionAnim);
            m_pReactionAnim->SetFrame(index);
            m_IconLayout.Animate();
            m_IconLayout.UnbindAnimation(m_pReactionAnim);
            return;
        }
    }
    SetFlagVisible(GetIconPane(), false);
    m_IsActive = false;
}

// 0x0032146C
void ReactionDragIconNode::Setup(s32 index, ssys::ma::lyt::Animation* reactionAnim, ssys::ma::lyt::ArcResAccReader* reader)
{
    m_IconLayout.SetArcResAccReader(reader);
    g_FormatBuffer.format("%s.bclyt", "react_icon");
    m_IconLayout.Build(g_FormatBuffer.getCString(), NULL, CMD_CACHE_SIZE);
    m_pLayout->m_Layer = LAYOUT_LAYER;
    m_pReactionAnim = reactionAnim;
    ButtonActionNode::Info info = {NULL, NULL, nn::math::VEC2(0.0f, 0.0f), 0, 0, NULL};
    info.pane = m_IconLayout.FindPane("B_btn");
    info.pos = nn::math::VEC2(0.0f, 0.0f);
    info.index = index;
    Initialize(info);
    m_OkSoundEnabled = false;
    m_Se = SE_SYS_REACT_BTN_ACTIVE;
    m_SoundEnabled = true;
    m_SoundMode = true;
}
