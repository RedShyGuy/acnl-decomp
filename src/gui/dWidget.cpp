#include "gui/dWidget.h"

#include "Bs/dBsMenuMgr.h"
#include "Other/dController.h"
#include "applet/dErrEulaMgr.h"
#include "lyt/dTextArea.h"

namespace {
// the buttons of the menus whose events the widgets send: A, B, X, Y, Start and the directions
const u32 MENU_KEYS = BsMenuMgr::INPUT_A | BsMenuMgr::INPUT_B | BsMenuMgr::INPUT_X | BsMenuMgr::INPUT_Y | BsMenuMgr::INPUT_START |
                      BsMenuMgr::INPUT_DPAD_UP | BsMenuMgr::INPUT_DPAD_DOWN | BsMenuMgr::INPUT_DPAD_LEFT | BsMenuMgr::INPUT_DPAD_RIGHT |
                      BsMenuMgr::INPUT_CPAD_UP | BsMenuMgr::INPUT_CPAD_DOWN | BsMenuMgr::INPUT_CPAD_LEFT | BsMenuMgr::INPUT_CPAD_RIGHT;
static_assert(MENU_KEYS == 0x77C7C, "MENU_KEYS");
// the flags of Update that stop the input
const u32 UPDATE_STOP_FLAGS = 0x107;
// the frames before a held key repeats, then every REPEAT_INTERVAL frames
const s32 REPEAT_DELAY = 20;
const s32 REPEAT_INTERVAL = 5;
} // namespace

namespace gui {
// 0x0095F0EE
bool g_IsWidgetHolding = false;
// the keys whose holding repeats (1: those of BsMenuMgr::IsPressed)
// 0x0095F0F0
u16 g_RepeatKeyMask = 1;
// 0x0095F0F2
u16 g_HeldKeys = 0;
// 0x0095F12C
Callback g_Callback = NULL;
// 0x0095F130
s32 g_RepeatCounter = 0;
// 0x0095F1A0
void* g_pCallbackUser = NULL;
// the static initializer 0x00785A28 (shared with other files of the game) builds the list
// 0x00AD762C
Widget::List Widget::s_List;

// 0x004FEAC8
bool IsDirectionHold()
{
    return BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_DPAD_UP) || BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_UP) || BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_DPAD_DOWN) ||
           BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_DOWN) || BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_DPAD_LEFT) || BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_LEFT) ||
           BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_DPAD_RIGHT) || BsMenuMgr::s_pInstance->IsHold(BsMenuMgr::INPUT_CPAD_RIGHT);
}

// 0x004FEC40
bool IsDirectionPressed()
{
    return BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_DPAD_UP) || BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_CPAD_UP) || BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_DPAD_DOWN) ||
           BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_CPAD_DOWN) || BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_DPAD_LEFT) ||
           BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_CPAD_LEFT) || BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_DPAD_RIGHT) || BsMenuMgr::s_pInstance->IsPressed(BsMenuMgr::INPUT_CPAD_RIGHT);
}

// 0x004FED24 slot 0x00
// 0x004FECF4 (deleting dtor)
Widget::~Widget()
{
    s_List.Erase(this);
}

// 0x004FED50
void SetCallback(Callback callback, void* user)
{
    g_Callback = callback;
    g_pCallbackUser = user;
}

// 0x004FED64
void DestroyAll()
{
    for (Widget::List::iterator it = Widget::s_List.begin(); it != Widget::s_List.end();) {
        Widget* widget = Widget::List::GetPointerFromNode(it.GetNode());
        it = Widget::s_List.erase(it);
        delete widget;
    }
    g_Callback = NULL;
    g_pCallbackUser = NULL;
    lyt::g_Unknown95F1A4 = 0;
    g_IsWidgetHolding = false;
}

// 0x004FEDD8
u32 Update(u32 flags)
{
    g_IsWidgetHolding = false;
    for (Widget::List::iterator it = Widget::s_List.begin(); it != Widget::s_List.end(); ++it) {
        if (Widget::List::GetPointerFromNode(it.GetNode())->m_IsHolding) {
            g_IsWidgetHolding = true;
            break;
        }
    }
    if ((flags & UPDATE_STOP_FLAGS) != 0) {
        return flags;
    }
    if (lyt::g_Unknown95F1A4 != 0) {
        g_HeldKeys = 0;
        return flags;
    }
    for (Widget::List::iterator it = Widget::s_List.begin(); it != Widget::s_List.end(); ++it) {
        Widget::List::GetPointerFromNode(it.GetNode())->Update();
    }
    if (g_IsWidgetHolding || Controller::IsHold(Controller::TOUCH)) {
        return flags;
    }
    BsMenuMgr* mgr = BsMenuMgr::s_pInstance;
    const bool isPressed = mgr->IsPressed(MENU_KEYS);
    if (isPressed) {
        SendEvent(NULL, EVENT_KEY_PRESS, isPressed);
        g_HeldKeys = isPressed;
        g_RepeatCounter = 0;
    } else {
        const u16 keys = mgr->IsHold(MENU_KEYS) & g_RepeatKeyMask;
        if (keys != 0 && g_HeldKeys == keys) {
            g_RepeatCounter++;
            if (g_RepeatCounter >= REPEAT_DELAY && g_RepeatCounter % REPEAT_INTERVAL == 0) {
                SendEvent(NULL, EVENT_KEY_REPEAT, keys);
            }
        }
        g_HeldKeys = keys;
    }
    if (BsMenuMgr::s_pInstance->IsHold(MENU_KEYS)) {
        SendEvent(NULL, EVENT_KEY_HOLD, 1);
    }
    return flags;
}
} // namespace gui
