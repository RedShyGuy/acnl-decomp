#include "Other/dDragIconControl.h"

#include <cmath>
#include "Bs/dBsHandCursor.h"
#include "Bs/dBsMenuMgr.h"
#include "Other/dDragIconGroup.h"
#include "Other/dDragIconNode.h"
#include "Other/dTouchState.h"
#include "Item/dItemSelectWindow.h"
#include "Sound/dSoundMgr.h"
#include "script/dChoiceStandardItem.h"
#include "nw/lyt/lyt_Api.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dLayout.h"

namespace {
// the length of a vector of two components
inline f32 Length(f32 x, f32 y)
{
    return std::sqrt(x * x + y * y);
}

// the touch position of the screen
inline nn::math::VEC2 GetTouchPosition()
{
    return nn::math::VEC2(g_TouchState.lastX, g_TouchState.lastY);
}

// the hand cursor of the menus
inline BsHandCursor* GetHandCursor()
{
    return BsMenuMgr::Get()->m_pBsHandCursor;
}
} // namespace

// the frames a touched icon is held / a pressed icon is held with the cursor before it is picked up
// 0x00954404
u32 g_DragIconTouchFrames = 30;
// 0x00954408
u32 g_DragIconCursorFrames = 15;
// the distance the touch / the cursor moves before the icon is picked up
// 0x0095440C
f32 g_DragIconTouchDistance = 15.0f;
// 0x00954410
f32 g_DragIconCursorDistance = 0.0f;
// the distance an icon flies back in one frame
// 0x00954414
f32 g_DragIconReturnSpeed = 40.0f;

// 0x0029A330 slot 0x10
void DragIconControl::Process()
{
    if ((m_Flags2 & ENABLE_TOUCH) == 0) {
        return;
    }
    if (!g_TouchState.isTouch) {
        // the touch ended
        DragIconNode* node = GetCurrentIcon();
        if (node == NULL) {
            return;
        }
        if (m_IsDragging) {
            if (m_pDropTarget == NULL || m_pDropTarget == node) {
                // the icon flies back
                node->ResetDragAnimation();
                GetCurrentIcon()->OnDropTargetLeave();
                m_pFunc = &ButtonActionControl::FUN_002f7300;
                SoundMgr::PlaySound(SE_SYS_ITEM_ICON_SET, GetCurrentIcon()->GetPan());
                return;
            }
            // dropped on another icon
            node->RewindDragAnimation();
            m_LastPosition = GetTouchPosition();
            SelectCurrent();
            return;
        }
        m_pDropTarget = NULL;
        if (node->IsTouch() && m_IsTapEnabled && GetCurrentIcon()->m_pIconGroup->m_IsTapEnabled) {
            m_LastPosition = GetTouchPosition();
            if (!m_IsDragEnabled) {
                node->SetDropMarker(true);
            }
            SelectCurrent();
            return;
        }
        GetCurrentIcon()->Select(false, false);
        m_pCurrent = NULL;
        return;
    }

    bool isBegin;
    if (BsMenuMgr::Get()->m_HandMode == 1) {
        isBegin = true;
    } else if (g_TouchState.prevIsTouch) {
        isBegin = false;
    } else {
        isBegin = m_pCurrent != NULL;
    }
    if (isBegin) {
        // the touch takes over from the hand cursor
        GetHandCursor()->Disable();
        BsMenuMgr::Get()->m_HandMode = 0;
        if (m_IsDragging) {
            DragIconNode* node = GetCurrentIcon();
            node->RewindDragAnimation();
            node->Select(false, false);
            GetCurrentIcon()->OnDropTargetLeave();
            m_pFunc = &ButtonActionControl::FUN_002f7300;
            SoundMgr::PlaySound(SE_SYS_ITEM_ICON_SET, GetCurrentIcon()->GetPan());
            return;
        }
        // 0x00A197CC (guard 0x0095442C)
        static nn::math::VEC3 s_DragIconZeroTranslate(0.0f, 0.0f, 0.0f);
        DragIconNode* node = GetCurrentIcon();
        if (node != NULL) {
            node->RewindDragAnimation();
            node->Select(false, false);
            if (m_IsDragging) {
                GetCurrentIcon()->GetIconPane()->SetTranslate(s_DragIconZeroTranslate);
            }
            GetCurrentIcon()->Select(false, false);
            m_pCurrent = NULL;
        }
        if (m_pDropTarget != NULL) {
            m_pDropTarget->OnDropTargetLeave();
            if (m_IsDragging && m_pDropTarget->GetIconPane() != NULL) {
                m_pDropTarget->GetIconPane()->SetTranslate(s_DragIconZeroTranslate);
            }
            m_pDropTarget->Select(false, false);
            m_pDropTarget = NULL;
        }
        if (!g_TouchState.prevIsTouch) {
            m_pCurrent = NULL;
            m_pTouchedIcon = NULL;
            goto FIND_TOUCHED;
        }
    } else if (!g_TouchState.prevIsTouch) {
        m_pCurrent = NULL;
        m_pTouchedIcon = NULL;
        goto FIND_TOUCHED;
    }
    if (m_pCurrent == NULL) {
        return;
    }
    goto HOLD;

FIND_TOUCHED:
    // a touch begins: the icon under it
    for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
        if (node->IsTouch() && m_pCurrent == NULL) {
            node->TouchSelect(true, true);
            m_pCurrent = node;
        } else {
            node->TouchSelect(false, false);
        }
    }
    if (m_pCurrent == NULL) {
        return;
    }
    m_MoveDistance = 0.0f;
    m_IsDragging = false;
    m_HoldFrames = 0;
    m_LastPosition = GetTouchPosition();
    m_pDropTarget = NULL;
    GetCurrentIcon()->m_pLayout->m_IsAnimationChanged = true;
    GetCurrentIcon()->m_TouchX = g_TouchState.lastX;
    GetCurrentIcon()->m_TouchY = g_TouchState.lastY;
    BsMenuMgr::Get()->m_HandMode = 0;
    m_pTouchedIcon = GetCurrentIcon();
    if (m_pTouchedIcon == NULL) {
        return;
    }

HOLD:
    // the touch is held
    if (m_IsDragEnabled && GetCurrentIcon()->m_pIconGroup->m_IsDragEnabled) {
        if (m_IsDragging) {
            DragIconNode* node = GetCurrentIcon();
            node->m_Offset.z = 0.0f;
            node->m_Offset.x = g_TouchState.lastX - node->m_TouchX;
            node->m_Offset.y = node->m_TouchY - g_TouchState.lastY;
            node->m_pLayout->m_IsAnimationChanged = true;
            // the icon under the touch that accepts drops
            DragIconNode* target = reinterpret_cast<DragIconNode*>(m_pFirst);
            while (target != NULL) {
                if (target->m_IsDropTarget && target->m_pIconGroup->m_IsDropEnabled) {
                    bool isContained;
                    if (target->m_pParentPane != NULL) {
                        nn::math::VEC2 pos;
                        target->m_pLayout->ScreenToLayout(nn::math::VEC2(g_TouchState.lastX, g_TouchState.lastY), &pos);
                        isContained = nw::lyt::IsContain(target->m_pParentPane, pos);
                    } else {
                        isContained = target->IsTouch();
                    }
                    if (isContained) {
                        break;
                    }
                }
                target = reinterpret_cast<DragIconNode*>(target != NULL ? target->m_pNext : m_pFirst);
            }
            if (m_pDropTarget == target) {
                return;
            }
            if (m_pDropTarget != NULL) {
                m_pDropTarget->OnDropTargetLeave();
            }
            if (target != NULL) {
                target->OnDropTargetEnter();
            }
            m_pDropTarget = target;
            return;
        }
        const nn::math::VEC2 pos = GetTouchPosition();
        m_MoveDistance += Length(pos.x - m_LastPosition.x, pos.y - m_LastPosition.y);
        m_LastPosition = pos;
        bool isStart;
        if (m_MoveDistance >= g_DragIconTouchDistance) {
            isStart = true;
        } else {
            m_HoldFrames++;
            isStart = m_HoldFrames >= g_DragIconTouchFrames;
        }
        m_IsDragging = isStart;
        if (!isStart) {
            return;
        }
        SoundMgr::PlaySound(SE_SYS_ITEM_ICON_PICK_UP, GetCurrentIcon()->GetPan());
        DragIconNode* node = GetCurrentIcon();
        node->StartDragAnimation();
        node->SetAttached(false);
        if (m_pOnDragStart != NULL) {
            m_pOnDragStart(GetCurrentIcon()->m_Index);
        }
        m_pTouchedIcon = NULL;
        return;
    }
    const bool isTouch = GetCurrentIcon()->IsTouch();
    if (GetCurrentIcon()->m_IsPressed != isTouch) {
        GetCurrentIcon()->TouchSelect(isTouch, true);
    }
    m_pTouchedIcon = isTouch ? GetCurrentIcon() : NULL;
}

// 0x0029ABB4
void DragIconControl::InitializeDrag()
{
    m_Flags = 0x20C;
    m_Flags2 = ENABLE_TOUCH | ENABLE_CURSOR | ENABLE_NODE_SHORTCUT;
    m_CursorTimer = 7;
    BsMenuMgr::Get()->m_HandMode = 1;
    GetHandCursor()->Disable();
    BsMenuMgr::s_pInstance->m_HandGrabTimer = m_CursorTimer;
    BsMenuMgr* mgr = BsMenuMgr::Get();
    mgr->m_pBsHandCursor->Disable();
    mgr->m_HandMode = 0;
}

// 0x0029AC1C slot 0x24
void DragIconControl::CheckRelease()
{
    if (!m_IsDragging) {
        ButtonActionControl::CheckRelease();
    }
}

// 0x0029AC30 slot 0x20
void DragIconControl::CheckInputForNodeTouch()
{
    BsHandCursor* cursor = GetHandCursor();
    if (!cursor->m_IsEnabled) {
        return;
    }
    const nn::math::VEC2 pos = cursor->m_Position;
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    if (mgr->IsHold(BsMenuMgr::INPUT_A)) {
        if (BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_A)) {
            // A is pressed: the icon under the cursor
            if (m_pCurrent != NULL) {
                m_pCurrent->Select(false, false);
                m_pCurrent = NULL;
            }
            for (ButtonActionNode* node = GetNextActive(NULL); node != NULL; node = GetNextActive(node)) {
                if (!node->IsCursorHover()) {
                    continue;
                }
                m_pCurrent = node;
                node->Select(true, false);
                if (GetCurrentIcon()->IsNotDraggable()) {
                    m_LastPosition = pos;
                    SelectCurrent();
                    return;
                }
                m_MoveDistance = 0.0f;
                m_IsDragging = false;
                m_HoldFrames = 0;
                m_LastPosition = GetHandCursor()->m_Position;
                m_pDropTarget = NULL;
                DragIconNode* icon = GetCurrentIcon();
                const nn::math::VEC2& position = GetHandCursor()->m_Position;
                icon->m_TouchX = position.x;
                icon->m_TouchY = position.y;
                return;
            }
            return;
        }
        // A is held
        DragIconNode* node = GetCurrentIcon();
        if (node == NULL) {
            return;
        }
        if (m_IsDragEnabled && node->m_pIconGroup->m_IsDragEnabled) {
            if (m_IsDragging) {
                const nn::math::VEC2& position = GetHandCursor()->m_Position;
                node->m_Offset.x = position.x - node->m_TouchX;
                node->m_Offset.y = position.y - node->m_TouchY;
                node->m_Offset.z = 0.0f;
                node->m_pLayout->m_IsAnimationChanged = true;
                // the icon under the cursor that accepts drops
                DragIconNode* target = reinterpret_cast<DragIconNode*>(m_pFirst);
                while (target != NULL) {
                    if (target->m_IsDropTarget && target->m_pIconGroup->m_IsDropEnabled && BsMenuMgr::Get()->m_HandMode) {
                        bool isContained;
                        if (target->m_pParentPane != NULL) {
                            const nn::math::VEC2 cursorPos = GetHandCursor()->m_Position;
                            isContained = nw::lyt::IsContain(target->m_pParentPane, cursorPos);
                        } else {
                            isContained = target->IsCursorHover();
                        }
                        if (isContained) {
                            break;
                        }
                    }
                    target = reinterpret_cast<DragIconNode*>(target != NULL ? target->m_pNext : m_pFirst);
                }
                if (m_pDropTarget == target) {
                    return;
                }
                if (m_pDropTarget != NULL) {
                    m_pDropTarget->OnDropTargetLeave();
                }
                if (target != NULL) {
                    target->OnDropTargetEnter();
                }
                m_pDropTarget = target;
                return;
            }
            m_MoveDistance += Length(m_LastPosition.x - pos.x, m_LastPosition.y - pos.y);
            m_HoldFrames++;
            const bool isStart = m_HoldFrames >= g_DragIconCursorFrames || m_MoveDistance > g_DragIconCursorDistance;
            m_IsDragging = isStart;
            m_LastPosition = pos;
            if (!isStart) {
                return;
            }
            SoundMgr::PlaySound(SE_SYS_ITEM_ICON_PICK_UP, node->GetPan());
            DragIconNode* icon = GetCurrentIcon();
            icon->StartDragAnimation();
            icon->SetAttached(false);
            GetHandCursor()->Func1F9FC4();
            if (m_pOnDragStart != NULL) {
                m_pOnDragStart(GetCurrentIcon()->m_Index);
            }
            return;
        }
        if (node->IsNotDraggable()) {
            return;
        }
        const bool isHover = GetCurrentIcon()->IsCursorHover();
        if (GetCurrentIcon()->m_IsPressed == isHover) {
            return;
        }
        GetCurrentIcon()->TouchSelect(isHover, true);
        return;
    }

    if (m_pCurrent != NULL && BsMenuMgr::s_pInstance->IsRelease(BsMenuMgr::INPUT_A)) {
        // A is let go
        if (m_IsDragging && m_IsDragEnabled && GetCurrentIcon()->m_pIconGroup->m_IsDragEnabled) {
            DragIconNode* node = GetCurrentIcon();
            if (m_pDropTarget != NULL && m_pDropTarget != node) {
                node->RewindDragAnimation();
                m_LastPosition = pos;
                SelectCurrent();
                GetHandCursor()->Func1FA0C4(false);
                return;
            }
            node->RewindDragAnimation();
            node->Select(false, false);
            GetCurrentIcon()->OnDropTargetLeave();
            m_pFunc = &ButtonActionControl::FUN_002f7300;
            GetHandCursor()->Func1FA0C4(false);
            SoundMgr::PlaySound(SE_SYS_ITEM_ICON_SET, GetCurrentIcon()->GetPan());
            return;
        }
        if (GetCurrentIcon()->IsCursorHover()) {
            if (!m_IsTapEnabled || !GetCurrentIcon()->m_pIconGroup->m_IsTapEnabled) {
                return;
            }
            if (GetCurrentIcon()->IsNotDraggable()) {
                return;
            }
            if (!m_IsDragEnabled) {
                GetCurrentIcon()->SetDropMarker(true);
            }
            m_LastPosition = pos;
            SelectCurrent();
            return;
        }
        GetCurrentIcon()->Select(false, false);
        m_pCurrent = NULL;
        return;
    }

    // nothing is pressed: the icon under the cursor is selected
    m_pDropTarget = NULL;
    ButtonActionNode* node = GetNextActive(NULL);
    while (node != NULL && !node->IsCursorHover()) {
        node = GetNextActive(node);
    }
    if (m_pCurrent == node) {
        return;
    }
    if (m_pCurrent != NULL) {
        m_pCurrent->Select(false, false);
    }
    m_pCurrent = node;
    if (node != NULL) {
        node->Select(true, true);
    }
}

// 0x0029B468
void DragIconControl::EndDrag(bool keep, SeID se)
{
    if (IsSelectOkDone() && m_pDropTarget != NULL) {
        m_Flags &= ~OK_DONE;
        GetCurrentIcon()->OnDropTargetLeave();
        GetCurrentIcon()->ResetDragAnimation();
        m_pDropTarget->OnDropTargetLeave();
        if ((GetCurrentIcon()->IsActivated() || m_pDropTarget->m_pIconGroup->m_IsDragEnabled) && se != 0) {
            SoundMgr::PlaySound(se, GetCurrentIcon()->GetPan());
        }
        if (keep) {
            m_pFunc = &ButtonActionControl::FUN_002f7300;
        } else {
            GetCurrentIcon()->SetAttached(true);
            Reset();
            UnselectActive();
            m_pFunc = &ButtonActionControl::State_Normal;
            m_pDropTarget = NULL;
        }
        m_IsDragging = false;
        return;
    }
    if (!IsSelectOkDone()) {
        return;
    }
    if (m_pDropTarget != NULL) {
        m_pDropTarget->SetAttached(true);
        m_pDropTarget->OnDropTargetLeave();
    }
    Reset();
    UnselectActive();
}

// 0x0029B5C0 slot 0x34
void DragIconControl::OnBegin()
{
    if (m_pCurrent != NULL) {
        GetCurrentIcon()->ResetDragAnimation();
    }
    if (m_pDropTarget != NULL) {
        m_pDropTarget->ResetDragAnimation();
    }
    m_pDropTarget = NULL;
}

// 0x0029B5EC slot 0x18
bool DragIconControl::CheckInputForNode()
{
    if (m_IsDragging) {
        return false;
    }
    return ButtonActionControl::CheckInputForNode();
}

// 0x0029B604
void DragIconControl::UpdateItemWindow(script::ChoiceStandardItem* choice)
{
    bool isNameVisible;
    if (!BsMenuMgr::Get()->m_HandMode) {
        isNameVisible = true;
    } else {
        isNameVisible = (IsSelectOkDone() || IsSelectOkOngoing()) && m_pDropTarget != NULL;
    }
    choice->m_Window.SetNameVisible(isNameVisible);
    choice->m_Window.m_pAttachPane = m_pCurrent != NULL ? GetCurrentIcon()->GetParentPane() : NULL;
    nn::math::VEC3 position;
    if (m_pCurrent != NULL) {
        const nw::lyt::Pane* pane = GetCurrentIcon()->GetIconPane();
        position.x = pane->m_GlbMtx.m[0][3];
        position.y = pane->m_GlbMtx.m[1][3];
        position.z = pane->m_GlbMtx.m[2][3];
    }
    choice->m_Window.m_AttachPosition = position;
}

// 0x0029B6E8 slot 0x2C
void DragIconControl::SelectCurrent()
{
    if (m_pCanDrop == NULL || m_pDropTarget == NULL ||
        m_pCanDrop(GetCurrentIcon()->m_Index, m_pDropTarget->m_Index)) {
        ButtonActionControl::SelectCurrent();
        return;
    }
    // not allowed: the icon flies back
    GetCurrentIcon()->Select(false, false);
    m_Flags |= OK_DONE;
    m_pFunc = &ButtonActionControl::State_Selecting;
}

// 0x0029B760 slot 0x38
void DragIconControl::FUN_002f7300()
{
    if (m_pCurrent != NULL) {
        GetCurrentIcon()->FollowParentPane();
    } else if (m_pDropTarget == NULL) {
        if (GetHandCursor()->Func1FA040()) {
            m_IsDragging = false;
            m_pFunc = &ButtonActionControl::State_Normal;
        }
        return;
    }
    nn::math::VEC3& offset = GetCurrentIcon()->m_Offset;
    const f32 length = std::sqrt(offset.x * offset.x + offset.y * offset.y + offset.z * offset.z);
    if (length == 0.0f) {
        // it arrived
        if (m_pDropTarget != NULL) {
            m_pDropTarget->SetAttached(true);
            m_pDropTarget->OnDropTargetLeave();
            m_pDropTarget = NULL;
        }
        if (m_pCurrent != NULL) {
            GetCurrentIcon()->SetAttached(true);
            m_pCurrent = NULL;
        }
        if (GetHandCursor()->Func1FA040()) {
            m_pFunc = &ButtonActionControl::State_Normal;
        }
        m_IsDragging = false;
        return;
    }
    if (g_DragIconReturnSpeed > length) {
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.0f;
        return;
    }
    const f32 scale = (length - g_DragIconReturnSpeed) / length;
    offset.x *= scale;
    offset.y *= scale;
    offset.z *= scale;
}

// 0x0029B984
void DragIconControl::StartReturn(DragIconNode* node, DragIconNode* from)
{
    m_pCurrent = node;
    m_pDropTarget = from;
    node->SetAttached(false);
    const nw::lyt::Pane* fromPane = from->GetIconPane();
    const f32 fromX = fromPane->m_GlbMtx.m[0][3];
    const f32 fromY = fromPane->m_GlbMtx.m[1][3];
    const f32 fromZ = fromPane->m_GlbMtx.m[2][3];
    const nw::lyt::Pane* pane = node->GetIconPane();
    node->m_Offset.x = fromX - pane->m_GlbMtx.m[0][3];
    node->m_Offset.y = fromY - pane->m_GlbMtx.m[1][3];
    node->m_Offset.z = fromZ - pane->m_GlbMtx.m[2][3];
    m_Flags |= OK_DONE;
}

// 0x0029BA20
nw::lyt::Pane* DragIconControl::GetCurrentParentPane()
{
    if (m_pCurrent == NULL) {
        return NULL;
    }
    return GetCurrentIcon()->GetParentPane();
}

// 0x0029BA3C
void DragIconControl::Cancel()
{
    if (m_pCurrent != NULL) {
        GetCurrentIcon()->OnDropTargetLeave();
        GetCurrentIcon()->ResetDragAnimation();
        GetCurrentIcon()->SetAttached(true);
        if (GetCurrentIcon()->IsOkOngoing()) {
            GetCurrentIcon()->EndSelectOk();
            DragIconNode* node = GetCurrentIcon();
            node->m_IsPressed = false;
            node->Animate(node->m_TouchOk, node->m_IsPressed);
            node->Animate(node->m_SelectOk, node->m_IsPressed);
            GetCurrentIcon()->ButtonActionNode::Select(false);
        }
    }
    if (m_pDropTarget != NULL) {
        m_pDropTarget->SetAttached(true);
        m_pDropTarget->OnDropTargetLeave();
        m_pDropTarget->TouchSelect(false, false);
        m_pDropTarget = NULL;
    }
    Reset();
    UnselectActive();
    m_IsDragging = false;
    m_pFunc = &ButtonActionControl::State_Normal;
}

// 0x0029BB80
void DragIconControl::DrawDraggedIcons()
{
    for (DragIconNode* node = reinterpret_cast<DragIconNode*>(m_pFirst); node != NULL;
         node = reinterpret_cast<DragIconNode*>(node->m_pNext)) {
        node->DrawDragged();
    }
}

// 0x0029BBB8 slot 0x0C
void DragIconControl::Reset()
{
    if (BsMenuMgr::Get()->m_HandMode) {
        OnBegin();
        if (m_pCurrent != NULL && m_pCurrent->IsActivated() && m_pCurrent->IsCursorHover()) {
            m_pCurrent->Select(true, false);
        } else {
            m_pCurrent = NULL;
        }
        m_Flags &= ~OK_DONE;
        m_pFunc = &ButtonActionControl::State_Normal;
        return;
    }
    if (m_pCurrent != NULL) {
        GetCurrentIcon()->SetAttached(true);
    }
    ButtonActionControl::Reset();
}

// 0x0029BC84
f32 DragIconControl::GetCurrentPan()
{
    if (m_pCurrent == NULL) {
        return 0.0f;
    }
    return GetCurrentIcon()->GetPan();
}

// 0x0029BCB4 slot 0x08
void DragIconControl::Update()
{
    ButtonActionControl::Update();
    for (ssys::st::ListNode* link = m_Groups.m_pFirst; link != NULL; link = link->m_pNext) {
        DragIconGroup* group = static_cast<DragIconGroup*>(link);
        if (group->m_pMarker != NULL && group->m_MarkerAnim.GetAnimTransform() != NULL && group->m_MarkerAnim.IsBound() &&
            !group->m_MarkerAnim.IsDone()) {
            group->m_MarkerAnim.IncreaseFrame();
            group->m_pLayout->m_IsAnimationChanged = true;
        }
    }
}

// 0x0029BD28
void DragIconControl::AddGroup(DragIconGroup* group)
{
    while (ssys::st::ListNode* node = group->m_pFirst) {
        group->Erase(node);
        PushBack(node);
    }
    m_Groups.PushBack(group);
}

// 0x0029BD7C
DragIconControl::DragIconControl()
    : m_HoldFrames(0), m_MoveDistance(0.0f), m_IsDragging(false), m_pDropTarget(NULL), m_pTouchedIcon(NULL),
      m_IsDragEnabled(true), m_IsTapEnabled(true), m_pCanDrop(NULL), m_pOnDragStart(NULL)
{
}

// 0x0029BDEC slot 0x00
// 0x0029BDD0 (deleting dtor)
DragIconControl::~DragIconControl()
{
    // (only the members are destroyed)
}

// 0x002F7594
void DragIconControl::Finalize()
{
    ssys::st::ListNode* node = NULL;
    while ((node = node == NULL ? m_pFirst : node->m_pNext) != NULL) {
        static_cast<DragIconNode*>(node)->Release();
    }
    for (ssys::st::ListNode* link = m_Groups.m_pFirst; link != NULL; link = link->m_pNext) {
        DragIconGroup* group = static_cast<DragIconGroup*>(link);
        if (group->m_pMarker != NULL && group->m_MarkerAnim.m_IsBound != 0) {
            if (group->m_pMarkerGroup != NULL) {
                group->m_pLayout->UnbindAnimation(&group->m_MarkerAnim, group->m_pMarkerGroup, false);
            } else {
                group->m_pLayout->UnbindAnimation(&group->m_MarkerAnim);
            }
        }
    }
    DisableHandCursor();
}
