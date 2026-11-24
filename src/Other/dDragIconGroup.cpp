#include "Other/dDragIconGroup.h"

#include "Other/dDragIconNode.h"
#include "Other/dPaneVisible.h"
#include "Other/dReactionDragIconNode.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dLayout.h"

// 0x0022C7C0
void DragIconGroup::AddNode(ReactionDragIconNode* node, nw::lyt::Pane* pane)
{
    node->m_pGroupAnim = &m_Extra;
    node->m_Touch.SetAnimTransform(m_Touch.GetAnimTransform());
    node->m_Select.SetAnimTransform(m_Select.GetAnimTransform());
    node->m_TouchOk.SetAnimTransform(m_TouchOk.GetAnimTransform());
    node->m_SelectOk.SetAnimTransform(m_SelectOk.GetAnimTransform());
    node->m_DragAnim.SetAnimTransform(m_Drag.GetAnimTransform());
    if (node->m_pLayout == NULL) {
        node->m_pLayout = m_pLayout;
    }
    node->m_pIconGroup = this;
    node->SetParentPane(pane);
    node->SetAttached(true);
    PushBack(node);
}

// 0x0022C878
void DragIconGroup::ShowMarker(DragIconNode* node)
{
    if (m_pMarker == NULL) {
        return;
    }
    SetFlagVisible(m_pMarker, true);
    m_pMarker->m_pParent->RemoveChild(m_pMarker);
    node->GetParentPane()->PrependChild(m_pMarker);
    if (m_MarkerAnim.GetAnimTransform() == NULL) {
        return;
    }
    if (!m_MarkerAnim.IsBound()) {
        if (m_pMarkerGroup != NULL) {
            m_pLayout->BindAnimation(&m_MarkerAnim, m_pMarkerGroup, false);
        } else {
            m_pLayout->BindAnimation(&m_MarkerAnim);
        }
    }
    m_MarkerAnim.SetFrame(0.0f);
    m_pLayout->m_IsAnimationChanged = true;
}

// 0x0022C924
void DragIconGroup::HideMarker()
{
    if (m_pMarker != NULL) {
        SetFlagVisible(m_pMarker, false);
        m_pLayout->m_IsAnimationChanged = true;
    }
}

// 0x0022C94C
void DragIconGroup::Setup(const SetupArg& arg)
{
    m_pLayout = arg.layout;
    m_pMarker = arg.marker;
    if (m_pMarker != NULL) {
        SetFlagVisible(m_pMarker, false);
        m_pMarker->SetTranslate(nn::math::VEC3(0.0f, 0.0f, 0.0f));
    }
    m_pMarkerGroup = arg.markerGroup;
    if (arg.markerAnim != NULL) {
        m_MarkerAnim.Initialize(arg.markerAnim, arg.markerResAcc);
    }
    if (arg.touch != NULL) {
        m_Touch.Initialize(arg.touch, arg.resAcc);
    }
    if (arg.select != NULL) {
        m_Select.Initialize(arg.select, arg.resAcc);
    }
    if (arg.touchOk != NULL) {
        m_TouchOk.Initialize(arg.touchOk, arg.resAcc);
    }
    if (arg.selectOk != NULL) {
        m_SelectOk.Initialize(arg.selectOk, arg.resAcc);
    }
    if (arg.drag != NULL) {
        m_Drag.Initialize(arg.drag, arg.resAcc);
    }
}

// 0x0022CA48
void DragIconGroup::AddNode(DragIconNode* node, nw::lyt::Pane* pane)
{
    node->m_Touch.SetAnimTransform(m_Touch.GetAnimTransform());
    node->m_Select.SetAnimTransform(m_Select.GetAnimTransform());
    node->m_TouchOk.SetAnimTransform(m_TouchOk.GetAnimTransform());
    node->m_SelectOk.SetAnimTransform(m_SelectOk.GetAnimTransform());
    node->m_DragAnim.SetAnimTransform(m_Drag.GetAnimTransform());
    if (node->m_pLayout == NULL) {
        node->m_pLayout = m_pLayout;
    }
    node->m_pIconGroup = this;
    node->SetParentPane(pane);
    node->SetAttached(true);
    PushBack(node);
}

// 0x0022CAF8
DragIconGroup::DragIconGroup()
    : m_pLayout(NULL), m_pMarker(NULL), m_pMarkerGroup(NULL), m_IsEnabled(true), m_IsDragEnabled(true),
      m_IsTapEnabled(true), m_IsDropEnabled(true)
{
}

// 0x0022CB94 slot 0x00
// 0x0022CB84 (deleting dtor)
DragIconGroup::~DragIconGroup()
{
    // (only the members are destroyed)
}
