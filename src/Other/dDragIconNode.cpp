#include "Other/dDragIconNode.h"

#include "Other/dDragIconGroup.h"
#include "Other/dPaneVisible.h"
#include "Sound/dSoundLabel.h"
#include "Sound/dSoundMgr.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dLayout.h"
#include "ssys/ma/lyt/dLayoutMgr.h"

namespace {
// (inline) the global position of a pane
inline nn::math::VEC3 GetGlobalPosition(const nw::lyt::Pane* pane)
{
    return nn::math::VEC3(pane->m_GlbMtx.m[0][3], pane->m_GlbMtx.m[1][3], pane->m_GlbMtx.m[2][3]);
}

} // namespace

// 0x001FFA34 slot 0x60
nw::lyt::Pane* DragIconNode::GetParentPane()
{
    return m_pParentPane;
}

// 0x001FFA3C slot 0x5C
void DragIconNode::SetParentPane(nw::lyt::Pane* pane)
{
    if (m_pParentPane != NULL) {
        m_pLayout->GetRootPane()->SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
    }
    m_pParentPane = pane;
    m_IsAttached = false;
    if (pane != NULL) {
        m_pLayout->GetRootPane()->SetTranslate(GetGlobalPosition(pane));
    }
}

// 0x001FFABC
void DragIconNode::ResetDragAnimation()
{
    RewindDragAnimation();
    TouchSelect(false, false);
}

// 0x001FFB40 slot 0x64
void DragIconNode::OnDropTargetEnter()
{
    SetDropMarker(true);
    SoundMgr::SPlaySystemSound(SE_SYS_ITEM_SLOT_ACTIVE, GetPan());
}

// 0x001FFB7C
void DragIconNode::FollowParentPane()
{
    const nn::math::VEC3 position = GetGlobalPosition(m_pParentPane);
    m_Offset.x += m_ParentPosition.x - position.x;
    m_Offset.y += m_ParentPosition.y - position.y;
    m_Offset.z += m_ParentPosition.z - position.z;
    m_ParentPosition = position;
}

// 0x001FFBDC slot 0x68
void DragIconNode::OnDropTargetLeave()
{
    SetDropMarker(false);
}

// 0x001FFBEC slot 0x58
void DragIconNode::DrawDragged()
{
    if (m_pParentPane != NULL && !m_IsAttached) {
        const nn::math::VEC3 position(m_ParentPosition.x + m_Offset.x, m_ParentPosition.y + m_Offset.y,
                                      m_ParentPosition.z + m_Offset.z);
        m_pLayout->GetRootPane()->SetTranslate(position);
        m_pLayout->Animate();
    }
    if (!m_IsAttached) {
        ssys::ma::lyt::LayoutMgr::Get()->Register(m_pLayout, true);
    }
}

// 0x001FFC90 slot 0x44
void DragIconNode::SetAttached(bool attach)
{
    if (attach) {
        if (m_IsAttached) {
            return;
        }
        m_pParentPane->AppendChild(GetIconPane());
        m_IsAttached = true;
        m_Offset = nn::math::VEC3(0.0f, 0.0f, 0.0f);
        GetIconPane()->SetTranslate(m_Offset);
        m_pLayout->Animate();
        if (m_pIconGroup->m_pLayout != NULL) {
            m_pIconGroup->m_pLayout->m_IsAnimationChanged = true;
        }
    } else {
        if (!m_IsAttached) {
            return;
        }
        m_pParentPane->RemoveChild(GetIconPane());
        m_IsAttached = false;
        if (m_pIconGroup->m_pLayout != NULL) {
            m_pIconGroup->m_pLayout->m_IsAnimationChanged = true;
        }
        m_ParentPosition = GetGlobalPosition(m_pParentPane);
    }
}

// 0x001FFD84 slot 0x54
void DragIconNode::Release()
{
    if (m_IsAttached) {
        m_pParentPane->RemoveChild(GetIconPane());
    }
    if (m_pLayout != NULL) {
        m_pLayout->UnbindAllAnimation();
    }
}

// 0x001FFDD0 slot 0x4C
void DragIconNode::SetDropMarker(bool show)
{
    if (m_pIconGroup == NULL) {
        return;
    }
    if (show) {
        m_pIconGroup->ShowMarker(this);
    } else {
        m_pIconGroup->HideMarker();
    }
}

// 0x001FFE18 slot 0x10
void DragIconNode::FUN_002b72e0()
{
    if (m_DragAnim.IsBound()) {
        if (m_DragAnim.IsDone()) {
            m_pLayout->UnbindAnimation(&m_DragAnim);
        } else {
            m_DragAnim.IncreaseFrame();
        }
    }
    if (m_pParentPane != NULL && m_IsAttached) {
        GetIconPane()->SetTranslate(m_Offset);
        m_pLayout->Animate();
    }
}

// 0x001FFE9C slot 0x48
void DragIconNode::SetMarker(bool show)
{
    if (m_pIconGroup == NULL) {
        return;
    }
    if (show) {
        m_pIconGroup->ShowMarker(this);
    } else {
        m_pIconGroup->HideMarker();
    }
}

// 0x001FFEE4 slot 0x50
void DragIconNode::SetSounds(bool isOn)
{
    // empty (in the original too)
}

// 0x001FFEE8
DragIconNode::DragIconNode()
    : m_pParentPane(NULL), m_Offset(0.0f, 0.0f, 0.0f), m_ParentPosition(0.0f, 0.0f, 0.0f), m_pIconGroup(NULL),
      m_TouchX(0), m_TouchY(0), m_IsAttached(false), m_IsDropTarget(true)
{
    SetSeOk(SE_SYS_DECIDE);
    SetOkSoundEnabled(true);
}

// 0x001FFF94 slot 0x00
// 0x001FFF5C (deleting dtor)
DragIconNode::~DragIconNode()
{
    m_DragAnim.SetAnimTransform(NULL);
}
