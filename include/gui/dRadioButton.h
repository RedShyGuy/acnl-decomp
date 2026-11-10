#pragma once

#include "decomp.h"
#include "gui/dButtonBase.h"

namespace gui {
// A radio button: deciding it selects it (STATE_LOCKED, unless it is repeatable) and puts the selected button of
// the same group back to STATE_IDLE. Only the class name is from the binary (RTTI); member and function names are
// ours.
// RTTI N3gui11RadioButtonE @ 0x008D0E00
// vtable 0x00903DC8 (vptr 0x00903DD0), offset_to_top 0, 15 entries
class RadioButton : public ::gui::ButtonBase
{
public:
    struct Description : ButtonBase::Description
    {
        Description(); // 0x004F7D30

        s32 group;            // 0x38 the buttons of a group select each other off
        u32 isRepeatable;     // 0x3C (a word) deciding leaves the button in STATE_IDLE
        bool isCenterSound;   // 0x40 the decision sound is played without pan
    };

    explicit RadioButton(const Description& desc); // 0x004F7E14
    ~RadioButton() override; // 0x004F7EF0 slot 0x00
    // 0x004F7EC0 slot 0x04 (deleting dtor)
    void Update() override; // 0x004F7D80 slot 0x08
    u32 GetKind() const override { return KIND_RADIO_BUTTON; } // 0x00747554 slot 0x0C
    void UpdateIdle() override; // 0x004F7A1C slot 0x28
    void UpdatePressed() override; // 0x004F7B90 slot 0x2C
    void UpdateDecided() override; // 0x004F7AC4 slot 0x30
    void UpdateSlidOut() override; // 0x004F7CB4 slot 0x34
    void UpdateDisabled() override; // 0x004F7B38 slot 0x38

    // the button is selected, the selected one of the group goes back to STATE_IDLE (playAnimation: their
    // animations are played, the button decides with STATE_DECIDED but without the event; otherwise both get the end
    // look at once)
    void Select(bool playAnimation); // 0x004F7880

    // the default sounds (also those of TglButton; the static initializer 0x00785A28 sets them to -1)
    static s32 s_DefaultSounds[SOUND_NUM];

    Description m_Desc;        // 0x30
    bool m_IsSelectedByCode;   // 0x74 Select was called: no decision event
};
ASSERT_SIZE(RadioButton::Description, 0x44);
ASSERT_SIZE(RadioButton, 0x78);
} // namespace gui
