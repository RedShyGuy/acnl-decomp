#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dAnimation.h"

class DragIconGroup;
namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// An icon that can be dragged between the panes of a DragIconControl (class name from the RTTI, all other names are
// ours): the root pane of its own layout is a child of a parent pane of the menu while it rests there; while it is
// dragged it is drawn on its own at its position. The icons of a DragIconGroup share the animations of the group.
// RTTI 12DragIconNode @ 0x008CB5F0
// vtable 0x008EE39C (vptr 0x008EE3A4), offset_to_top 0, 29 entries
class DragIconNode : public ::ButtonActionNode
{
public:
    DragIconNode(); // 0x001FFEE8
    // 0x001FFF5C (deleting dtor)
    ~DragIconNode() override; // 0x001FFF94 slot 0x00

    // active and its group enabled
    bool IsActivated() const override; // 0x00713028 slot 0x08 (inline below)
    // the drag animation and the position at the parent pane
    void FUN_002b72e0() override; // 0x001FFE18 slot 0x10

    // the root pane of the layout of the icon
    virtual nw::lyt::Pane* GetIconPane() { return m_pLayout->GetRootPane(); } // 0x00712FF0 slot 0x40
    // the icon becomes a child of the parent pane (or stops being one)
    virtual void SetAttached(bool attach); // 0x001FFC90 slot 0x44
    // the marker of the group shows on the icon (or is hidden)
    virtual void SetMarker(bool show); // 0x001FFE9C slot 0x48
    // the same (shown while the icon is the target of a drop)
    virtual void SetDropMarker(bool show); // 0x001FFDD0 slot 0x4C
    // the sounds of the icon (derived classes; the argument selects their kind)
    virtual void SetSounds(bool isOn); // 0x001FFEE4 slot 0x50
    // the icon leaves the parent pane; the animations of its layout are unbound
    virtual void Release(); // 0x001FFD84 slot 0x54
    // while the icon is dragged it is drawn on its own at its position (LayoutMgr)
    virtual void DrawDragged(); // 0x001FFBEC slot 0x58
    // the pane the icon rests on (the icon moves there)
    virtual void SetParentPane(nw::lyt::Pane* pane); // 0x001FFA3C slot 0x5C
    virtual nw::lyt::Pane* GetParentPane(); // 0x001FFA34 slot 0x60
    // the dragged icon is over this one (marker and sound) / no longer
    virtual void OnDropTargetEnter(); // 0x001FFB40 slot 0x64
    virtual void OnDropTargetLeave(); // 0x001FFBDC slot 0x68
    // the icon is chosen like a button and not dragged
    virtual bool IsNotDraggable() { return false; } // 0x00712FFC slot 0x6C
    // the pan of the sounds of the icon
    virtual f32 GetPan() { return GetIconPane()->m_GlbMtx.m[0][3] * 0.00625f; } // 0x00713004 slot 0x70

    // the drag animation from its first frame, unbound again; the icon is no longer touched
    void ResetDragAnimation(); // 0x001FFABC
    // (inline) the drag animation from its first frame, unbound again
    void RewindDragAnimation()
    {
        if (m_DragAnim.GetAnimTransform() != NULL) {
            if (m_DragAnim.IsBound()) {
                SetAnimationFrame(m_DragAnim, 0.0f);
            } else {
                BindAnimation(m_DragAnim);
            }
            m_pLayout->Animate();
            UnbindAnimation(m_DragAnim);
        }
    }
    // (inline) the drag animation starts (the icon is picked up)
    void StartDragAnimation()
    {
        if (m_DragAnim.GetAnimTransform() != NULL) {
            if (!m_DragAnim.IsBound()) {
                m_pLayout->BindAnimation(&m_DragAnim);
            }
            SetAnimationFrame(m_DragAnim, 0.0f);
            m_pLayout->m_IsAnimationChanged = true;
        }
    }
    // the offset follows a moving parent pane
    void FollowParentPane(); // 0x001FFB7C

    ssys::ma::lyt::Animation m_DragAnim;  // 0x0E0 (the transform of the group)
    nw::lyt::Pane* m_pParentPane;         // 0x108
    nn::math::VEC3 m_Offset;              // 0x10C from the parent pane
    nn::math::VEC3 m_ParentPosition;      // 0x118 the global position of the parent pane when the icon left it
    DragIconGroup* m_pIconGroup;          // 0x124
    s16 m_TouchX;                         // 0x128 where the touch began (DragIconControl)
    s16 m_TouchY;                         // 0x12A
    bool m_IsAttached;                    // 0x12C a child of the parent pane
    bool m_IsDropTarget;                  // 0x12D the icon accepts drops (DragIconControl)
    // never accessed, but the members of the derived classes start at 0x130
    u8 _padding12E[2];                    // 0x12E
};
ASSERT_OFFSET(DragIconNode, m_DragAnim, 0xE0);
ASSERT_OFFSET(DragIconNode, m_pParentPane, 0x108);
ASSERT_OFFSET(DragIconNode, m_pIconGroup, 0x124);
ASSERT_OFFSET(DragIconNode, m_IsDropTarget, 0x12D);
ASSERT_SIZE(DragIconNode, 0x130);

#include "Other/dDragIconGroup.h"

inline bool DragIconNode::IsActivated() const
{
    return m_IsActive && m_pIconGroup->m_IsEnabled;
}
