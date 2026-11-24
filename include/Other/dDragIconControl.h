#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "Sound/dSoundLabel.h"
#include "ssys/st/dList.h"

class DragIconGroup;
namespace script {
class ChoiceStandardItem;
} // namespace script
class DragIconNode;
namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// The control of the DragIconNodes of a menu (class name from the RTTI, all other names are ours): the icons are
// buttons that can also be picked up (by a touch that is held or moves, or by A with the hand cursor) and dropped on
// another icon; the chosen pair then becomes the result (OK_DONE). An icon that is let go elsewhere flies back.
// RTTI 15DragIconControl @ 0x008CC020
// vtable 0x008F18D4 (vptr 0x008F18DC), offset_to_top 0, 15 entries
class DragIconControl : public ::ButtonActionControl
{
public:
    // whether the icon of index src may be dropped on the one of index dst
    typedef bool (*CanDropFunc)(s32 src, s32 dst);
    // an icon of index was picked up
    typedef void (*DragStartFunc)(s32 index);

    DragIconControl(); // 0x0029BD7C
    // 0x0029BDD0 (deleting dtor)
    ~DragIconControl() override; // 0x0029BDEC slot 0x00

    // the buttons and the marker animations of the groups
    void Update() override; // 0x0029BCB4 slot 0x08
    void Reset() override; // 0x0029BBB8 slot 0x0C
    // the touch: choose, pick up, drag and drop the icons
    void Process() override; // 0x0029A330 slot 0x10
    bool CheckInputForNode() override; // 0x0029B5EC slot 0x18
    // the hand cursor: the same with A
    void CheckInputForNodeTouch() override; // 0x0029AC30 slot 0x20
    void CheckRelease() override; // 0x0029AC1C slot 0x24
    // the dropped pair is chosen (when the callback allows it; else the icon flies back)
    void SelectCurrent() override; // 0x0029B6E8 slot 0x2C
    void OnBegin() override; // 0x0029B5C0 slot 0x34
    // the dragged icon flies back to its place
    void FUN_002f7300() override; // 0x0029B760 slot 0x38

    // the control is set up for dragging with the hand cursor
    void InitializeDrag(); // 0x0029ABB4
    // the result was used: the icons end their drag (keep: the dragged icon flies back instead)
    void EndDrag(bool keep, SeID se); // 0x0029B468
    // the icon flies from the position of from to its place
    void StartReturn(DragIconNode* node, DragIconNode* from); // 0x0029B984
    // the parent pane of the current icon (NULL: none)
    nw::lyt::Pane* GetCurrentParentPane(); // 0x0029BA20
    // the drag is cancelled
    void Cancel(); // 0x0029BA3C
    // the icons that are dragged are drawn (DragIconNode::DrawDragged)
    void DrawDraggedIcons(); // 0x0029BB80
    // the pan of the sounds of the current icon (0: none)
    f32 GetCurrentPan(); // 0x0029BC84
    // the icons of the group join the control (the group is kept for its marker)
    void AddGroup(DragIconGroup* group); // 0x0029BD28
    // the icons are released, the marker animations of the groups unbound, the hand cursor disabled (name is ours)
    void Finalize(); // 0x002F7594

    // (inline) the current icon
    DragIconNode* GetCurrentIcon() { return reinterpret_cast<DragIconNode*>(m_pCurrent); }
    // (inline, copies for the menus) the indices of the drop target and the current icon (-1: none), the global position
    // of the current icon, the drop target while nothing is chosen
    DECOMP_EMIT_COPY s32 GetDropTargetIndex() const; // 0x0071BA88
    DECOMP_EMIT_COPY void GetCurrentPosition(nn::math::VEC3* pos); // 0x0071BA9C
    DECOMP_EMIT_COPY s32 GetCurrentIndex() const { return m_pCurrent != NULL ? m_pCurrent->m_Index : -1; } // 0x0071BAD4
    DECOMP_EMIT_COPY s32 GetHoverIndex() const; // 0x0071BDF8
    // the item window of the choice shows at the current icon (game code calls it; names are ours)
    void UpdateItemWindow(script::ChoiceStandardItem* choice); // 0x0029B604

    ssys::st::List m_Groups;              // 0x0E4 the DragIconGroups (by their list node)
    u32 m_HoldFrames;                     // 0x0F0 the frames the icon is held before the drag
    f32 m_MoveDistance;                   // 0x0F4 the distance the touch / cursor moved before the drag
    bool m_IsDragging;                    // 0x0F8
    nn::math::VEC2 m_LastPosition;        // 0x0FC the last touch / cursor position
    DragIconNode* m_pDropTarget;          // 0x104
    DragIconNode* m_pTouchedIcon;         // 0x108 the current icon while it is touched (no drag)
    bool m_IsDragEnabled;                 // 0x10C
    bool m_IsTapEnabled;                  // 0x10D
    CanDropFunc m_pCanDrop;               // 0x110
    DragStartFunc m_pOnDragStart;         // 0x114
};
ASSERT_OFFSET(DragIconControl, m_Groups, 0xE4);
ASSERT_OFFSET(DragIconControl, m_pDropTarget, 0x104);
ASSERT_OFFSET(DragIconControl, m_pOnDragStart, 0x114);
ASSERT_SIZE(DragIconControl, 0x118);

#include "Other/dDragIconNode.h"

inline s32 DragIconControl::GetDropTargetIndex() const
{
    return m_pDropTarget != NULL ? m_pDropTarget->m_Index : -1;
}

inline void DragIconControl::GetCurrentPosition(nn::math::VEC3* pos)
{
    if (m_pCurrent != NULL) {
        const nw::lyt::Pane* pane = GetCurrentIcon()->GetIconPane();
        pos->x = pane->m_GlbMtx.m[0][3];
        pos->y = pane->m_GlbMtx.m[1][3];
        pos->z = pane->m_GlbMtx.m[2][3];
    }
}

inline s32 DragIconControl::GetHoverIndex() const
{
    if (m_pDropTarget == NULL || IsSelectOkDone() || IsSelectOkOngoing()) {
        return -1;
    }
    return m_pDropTarget->m_Index;
}
