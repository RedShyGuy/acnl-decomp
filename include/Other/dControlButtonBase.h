#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Other/dCommonButtonSingle.h"
#include "state/dMode.h"
#include "Sound/dSoundLabel.h"

namespace nw {
namespace font {
class Font;
} // namespace font
} // namespace nw

// A control over an array of CommonButtonSingle with a state machine of its own (member and helper names are ours).
// RTTI 17ControlButtonBase @ 0x008CC528
// vtable 0x008F37D8 (vptr 0x008F37E0), offset_to_top 0, 15 entries
// vtable 0x008F381C (vptr 0x008F3824), offset_to_top -228, 3 entries
class ControlButtonBase : public ::ButtonActionControl, public ::state::Mode<ControlButtonBase>
{
public:
    ControlButtonBase(CommonButtonSingle* buttons, s32 buttonNum); // 0x002CDA10 (name after the RTTI)
    virtual ~ControlButtonBase(); // 0x002F8184 slot 0x00 | slot vf_0x00 of ssys::st::List
    // 0x002CDA78 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    // the visible buttons, then the state
    virtual void Update(); // 0x002CD748 slot 0x08 | slot vf_0x08 of ButtonActionControl

    // the buttons with the two fonts of the texts (once); the machine starts idle
    bool Initialize(const char* fontName, const nw::font::Font* font, const char* fontName2,
                    const nw::font::Font* font2); // 0x002CD428
    // the same with the message font and the ruby font
    bool Initialize(); // 0x002CD5A0
    // the buttons are in at once / out at once
    void SetIn(); // 0x002CD630
    void SetOut(); // 0x002CD6EC
    // the buttons leave their panes
    void Finalize(); // 0x002CD6AC
    // the buttons come in / go out (animations)
    bool StartIn(); // 0x002CD88C
    bool StartOut(); // 0x002CD8D8
    void AnimateButtons(); // 0x002CD810
    void SetButtonOkSound(SeID se, s32 index); // 0x002CD8AC
    void SetDisabled(bool disabled); // 0x002CD944
    // only the last num buttons are visible and active
    void SetVisibleNum(s32 num); // 0x002CD99C
    // the button index is inactive (or active again)
    void SetButtonInactive(s32 index, bool inactive); // 0x002CD1B4
    void SetButtonsVisible(bool visible); // 0x002CD1D0
    // the disabled look of the button index (it is inactive then)
    void SetButtonDisabled(s32 index, bool disabled); // 0x002CD23C
    // (copy for the game) the buttons come in or go out
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsBusy() const // 0x0071FB44
    {
        return GetState() == &ControlButtonBase::State_Out || GetState() == &ControlButtonBase::State_In;
    }

    // the states (the names in the comments are the debug names of the binary)
    void State_Idle(); // 0x002CD198 (waiting)
    void Enter_Idle(); // 0x002CD19C
    void State_In(); // 0x002CD270 Button IN
    void Enter_In(); // 0x002CD2E0
    void State_Out(); // 0x002CD338 button OUT
    void Enter_Out(); // 0x002CD39C
    // (a nop entry that falls into ButtonActionControl::Update)
    void State_Select(); // 0x002F7BA4 select
    void Enter_Select(); // 0x002CD224

    typedef ::state::Mode<ControlButtonBase>::Info StateInfo;
    static const StateInfo s_StateInfos[4];

    CommonButtonSingle* m_pButtons;  // 0x108
    s32 m_ButtonNum;                 // 0x10C
    s32 m_FirstVisible;              // 0x110
    bool m_IsInitialized;            // 0x114
};
ASSERT_OFFSET(ControlButtonBase, m_pButtons, 0x108);
ASSERT_SIZE(ControlButtonBase, 0x118);
