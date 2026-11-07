#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dLayout.h"
#include "ssys/ma/lyt/dResAccInterface.h"
#include "Button/dButtonActionNode.h"
#include "Bs/dBsMenuMgr.h"

namespace nw {
namespace lyt {
class Bounding;
} // namespace lyt
} // namespace nw

// The buttons of a menu layout: a list of ButtonActionNodes chosen by touch, by the hand cursor (circle pad), by the
// cursor keys or directly by their pad bits. Update calls the current state: the normal state handles the input, the
// "selecting" state plays the ok animations of the chosen button. Class, slot and member names after libgarden except
// where noted; the cursor of the menus (BsMenuMgr::m_HandGrabTimer) belongs to the control whose m_CursorTimer it holds.
// RTTI 19ButtonActionControl @ 0x008CC9C4
// vtable 0x008F4F58 (vptr 0x008F4F60), offset_to_top 0, 15 entries
class ButtonActionControl : public ::ssys::st::List
{
public:
    enum Flag : u32
    {
        OK_DONE = 1,                  // a button was chosen (or the control was cancelled)
        IS_SELECTING_OK = 2,
        FLAG_4 = 4,
        DISABLED = 8,
        CURSOR_KEYS = 0x10,           // SetCursor (name is ours)
        FLAG_20 = 0x20,
        ENABLE_BUTTON_SHORTCUT = 0x40,
        FLAG_80 = 0x80,
        FLAG_100 = 0x100,
    };

    enum Flag2 : u16
    {
        ENABLE_TOUCH = 0x1,           // (name is ours)
        ENABLE_CURSOR = 0x2,
        ENABLE_CURSOR_PERMANENT = 0x4,
        ENABLE_NODE_SHORTCUT = 0x8,
        CAN_RELEASE = 0x10,
        WAIT_FOR_SELECT_RELEASE = 0x20,
        NODE_OK_IMMEDIATE = 0x40,
    };

    typedef bool (*Predicate)();
    typedef void (ButtonActionControl::*Func)();

    struct Description
    {
        ssys::ma::lyt::ResAccInterface* resAcc;  // 0x00
        ssys::ma::lyt::Layout* layout;           // 0x04
        nw::lyt::Bounding* bounding;             // 0x08
        const char* touch;                       // 0x0C the names of the animations
        const char* select;                      // 0x10
        const char* touchOk;                     // 0x14
        const char* selectOk;                    // 0x18
        s32 cursorTimer;                         // 0x1C
        Predicate checkInput;                    // 0x20
    };
    ASSERT_SIZE(Description, 0x24);

    ButtonActionControl(); // 0x002F7FD0 | libgarden [tier A]
    virtual ~ButtonActionControl(); // 0x002F8188 slot 0x00 | libgarden
    // 0x002F80A4 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void Update(); // 0x002F7BA8 slot 0x08 | libgarden
    virtual void Reset(); // 0x002F779C slot 0x0C | libgarden
    // the touch input
    virtual void Process(); // 0x002F6188 slot 0x10 (name after libgarden)
    // the pad input (buttons, hand cursor and cursor keys)
    virtual void ProcessOk(); // 0x002F7BCC slot 0x14 (name after libgarden)
    // the pad bits of the buttons
    virtual bool CheckInputForNode(); // 0x002F6DDC slot 0x18 (name after libgarden)
    // the cursor keys
    virtual void CheckInputForNodeOk(); // 0x002F5EFC slot 0x1C (name after libgarden)
    // the hand cursor
    virtual void CheckInputForNodeTouch(); // 0x002F655C slot 0x20 (name after libgarden)
    virtual void CheckRelease(); // 0x002F6498 slot 0x24 (name after libgarden)
    // a button that requested to be chosen (NODE_OK_IMMEDIATE)
    virtual bool UpdateCurrentTouched(); // 0x002F74C4 slot 0x28 (name after libgarden)
    // chooses the current button: its ok animations play in State_Selecting
    virtual void SelectCurrent(); // 0x002F6EE8 slot 0x2C (name after libgarden)
    virtual void FUN_002f6ee4(); // 0x002F6EE4 slot 0x30 (name after libgarden)
    virtual void OnBegin(); // 0x002F6DD8 slot 0x34 (name after libgarden)
    // a state for the derived controls (m_pFunc; empty here)
    virtual void FUN_002f7300(); // 0x002F7300 slot 0x38 (name after libgarden)

    void Initialize(ButtonActionControl::Description const&, char const*); // 0x002F7810 | libgarden [tier A]
    void AddNode(ButtonActionNode*, bool); // 0x002F7F30 | libgarden [tier A]
    s32 GetPressedIndex() const; // 0x007241B0 | libgarden [tier A]
    s32 GetSelectedIndex() const; // 0x00724214 | libgarden [tier A]
    bool IsSelectOkDone() const; // 0x00724498 | libgarden [tier A]
    void UnselectCurrent(); // 0x002F6DA4 | libgarden [tier A]
    void UnselectActive(); // 0x002F7394 | libgarden [tier A]
    // (libgarden) the cursor keys
    void SetCursor(bool enable); // 0x002F7154

    // (names are ours)
    // the cursor keys with the default bits of the cursor input
    void InitializeCursor(bool enable); // 0x002F743C
    // the hand cursor came to this control: the button of the remembered index
    void BeginCursor(); // 0x002F6B78
    // calls ResetSelect of the buttons with the index (unused)
    void ResetSelectByIndex(s32 index); // 0x002F6D30
    // index -1: all buttons
    void SetNodeActive(s32 index, bool active); // 0x002F6F90
    void SelectNode(s32 index, bool select); // 0x002F70EC
    void SelectByIndex(s32 index); // 0x002F7654
    void RequestOk(s32 index, u8 request); // 0x002F7700
    void ResetSelectAll(); // 0x002F732C
    DECOMP_EMIT_COPY bool IsSelectingOk() const { return (m_Flags & IS_SELECTING_OK) != 0; } // 0x00724488
    DECOMP_EMIT_COPY bool IsCurrentPressed() const // 0x00724478
    {
        if (m_pCurrent == NULL) {
            return false;
        }
        return m_pCurrent->m_IsPressed;
    }
    // the next / previous active button after node (from the start / the end for NULL)
    DECOMP_EMIT_COPY ButtonActionNode* GetNextActive(ButtonActionNode* node) const // 0x007D2828
    {
        ButtonActionNode* next = static_cast<ButtonActionNode*>(node != NULL ? node->m_pNext : m_pFirst);
        if (next != NULL && !next->IsActivated()) {
            next = GetNextActive(next);
        }
        return next;
    }
    DECOMP_EMIT_COPY ButtonActionNode* GetPrevActive(ButtonActionNode* node) const // 0x007D2874
    {
        ButtonActionNode* prev = static_cast<ButtonActionNode*>(node != NULL ? node->m_pPrev : m_pLast);
        if (prev != NULL && !prev->IsActivated()) {
            prev = GetPrevActive(prev);
        }
        return prev;
    }
    // the cursor keys towards the next / previous button (two pairs of pad bits, each enabled by m_CursorInput)
    DECOMP_EMIT_COPY bool IsNextPressed() const // 0x007242D0
    {
        if ((m_CursorInput & 0x42000) != 0) {
            if (BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x2000) || BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x40000)) {
                return true;
            }
        }
        if ((m_CursorInput & 0x10800) != 0) {
            if (BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x800) || BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x10000)) {
                return true;
            }
        }
        return false;
    }
    DECOMP_EMIT_COPY bool IsPrevPressed() const // 0x007243A4
    {
        if ((m_CursorInput & 0x21000) != 0) {
            if (BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x1000) || BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x20000)) {
                return true;
            }
        }
        if ((m_CursorInput & 0x4400) != 0) {
            if (BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x400) || BsMenuMgr::s_pInstance->IsTrigOrRepeat(0x4000)) {
                return true;
            }
        }
        return false;
    }

    // (libgarden)
    void SetFlags(u32 flags, bool set = true)
    {
        if (set) {
            m_Flags |= flags;
        } else {
            m_Flags &= ~flags;
        }
    }
    void SetFlags2(u16 flags, bool set = true)
    {
        if (set) {
            m_Flags2 |= flags;
        } else {
            m_Flags2 &= ~flags;
        }
    }
    ButtonActionNode* GetCurrent() { return m_pCurrent; }
    bool HasCurrent() const { return m_pCurrent != NULL; }
    bool IsSelectOkOngoing() const { return m_pFunc == &ButtonActionControl::State_Selecting; }

    void State_Selecting(); // 0x002F6928 | libgarden [tier A]
    // the input of the buttons (name is ours)
    void State_Normal(); // 0x002F6AA4

    ssys::ma::lyt::Layout* m_pLayout;         // 0x0C
    ssys::ma::lyt::Animation m_Touch;         // 0x10 (given to the buttons)
    ssys::ma::lyt::Animation m_Select;        // 0x38
    ssys::ma::lyt::Animation m_TouchOk;       // 0x60
    ssys::ma::lyt::Animation m_SelectOk;      // 0x88
    nw::lyt::Bounding* m_pBounding;           // 0xB0 touches outside of it are ignored
    ButtonActionNode* m_pCurrent;             // 0xB4
    Func m_pFunc;                             // 0xB8 the state
    Predicate m_pCheckInput;                  // 0xC0 called when no key of the cursor was pressed
    u32 m_CursorInput;                        // 0xC4 the pad bits that bring the hand cursor (and the cursor keys)
    u32 m_ReleaseInput;                       // 0xC8 the pad bits that cancel
    // the cursor id of the control (see BsMenuMgr::m_HandGrabTimer)
    s32 m_CursorTimer;                        // 0xCC
    u16 m_Flags2;                             // 0xD0 Flag2
    u8 m_CurrentIndex;                        // 0xD2 the index of the current button among the active ones
    // the hand cursor comes when a touch is released outside of the buttons (name is ours)
    bool m_EnableCursorOnRelease;             // 0xD3
    u32 m_Flags;                              // 0xD4 Flag
    s32 m_Counter;                            // 0xD8 the frames a repeating button is held
    s32 m_MaxCounter;                         // 0xDC 11 before the first repeat, 3 after it
    u8 m_IsFirstRepeat;                       // 0xE0 (name is ours, libgarden: unk)
    // never accessed, but the members of the derived classes start at 0xE4 (ARMCC does not use the tail padding)
    u8 _paddingE1[3];                         // 0xE1
};
ASSERT_OFFSET(ButtonActionControl, m_pBounding, 0xB0);
ASSERT_OFFSET(ButtonActionControl, m_pFunc, 0xB8);
ASSERT_OFFSET(ButtonActionControl, m_Flags2, 0xD0);
ASSERT_SIZE(ButtonActionControl, 0xE4);
