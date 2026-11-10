#pragma once

#include "decomp.h"
#include "nw/ut/ut_LinkList.h"

namespace gui {
// An input element of the software keyboard (the buttons of gui): all widgets are in a global list and get their
// Update each frame from gui::Update. Only the class name is from the binary (RTTI); member and function names are
// ours.
// RTTI N3gui6WidgetE @ 0x008D0EC4
// vtable 0x009040FC (vptr 0x00904104), offset_to_top 0, 4 entries
class Widget
{
public:
    typedef nw::ut::LinkList<Widget, 0x4> List;

    // the four-character codes of GetKind (multi-character literals as in the binary)
    enum Kind : u32
    {
        KIND_BUTTON_EX = 'BTEX',
        KIND_BUTTON_EX2 = 'BTE2',
        KIND_TOGGLE_BUTTON = 'TGLB',
        KIND_RADIO_BUTTON = 'RDBT',
        KIND_RADIO_BUTTON_EX = 'RDEX',
        KIND_REPEAT_BUTTON = 'RPBT',
        KIND_REPEAT_BUTTON_EX = 'RPEX',
        KIND_REPEAT_BUTTON_EX2 = 'RPE2',
        KIND_REPEAT_INPUT_BUTTON = 'RIPB',
        KIND_TOUCH_ON_BUTTON = 'TONB',
        KIND_INPUT_BUTTON = 'IPBT',
    };

    // (inline) the widget goes to the end of the list
    Widget() : m_IsHolding(false) { s_List.PushBack(this); }
    virtual ~Widget(); // 0x004FED24 slot 0x00
    // 0x004FECF4 slot 0x04 (deleting dtor)
    // the input of the frame
    virtual void Update() = 0; // slot 0x08
    virtual u32 GetKind() const = 0; // slot 0x0C

    static List s_List;

    nw::ut::LinkListNode m_Link;  // 0x04 in s_List
    bool m_IsHolding;             // 0x0C the widget has the touch (the others ignore the input meanwhile)
};
ASSERT_SIZE(Widget, 0x10);

// the events of the callback (the value: see each)
enum Event
{
    EVENT_DECIDE = 1,     // a button was decided (value 0)
    EVENT_KEY_PRESS = 4,  // the buttons of the menus were pressed (value 1)
    EVENT_KEY_HOLD = 5,   // ... are held (value 1)
    EVENT_KEY_REPEAT = 6, // ... are held long enough to repeat (value: the repeated keys)
};
// the callback of the widgets (sender NULL: the event of the menu buttons)
typedef void (*Callback)(Widget* sender, s32 event, s32 value, void* user);

void SetCallback(Callback callback, void* user); // 0x004FED50
// all widgets are deleted, the callback is cleared
void DestroyAll(); // 0x004FED64
// the widgets get their input (nothing while one of the bits 0x107 of flags is set or the text area holds the
// input); the buttons of the menus send their events while no widget has the touch; flags are returned
u32 Update(u32 flags); // 0x004FEDD8
// a direction (pad or circle pad) of BsMenuMgr is held / pressed (the buttons test the result as the key bit 1)
bool IsDirectionHold(); // 0x004FEAC8
bool IsDirectionPressed(); // 0x004FEC40

extern bool g_IsWidgetHolding;
extern Callback g_Callback;
extern void* g_pCallbackUser;

// (inline) the callback gets the event
inline void SendEvent(Widget* sender, s32 event, s32 value)
{
    if (g_Callback != NULL) {
        g_Callback(sender, event, value, g_pCallbackUser);
    }
}
} // namespace gui
