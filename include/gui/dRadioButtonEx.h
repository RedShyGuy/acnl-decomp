#pragma once

#include "decomp.h"
#include "gui/dButtonEx.h"
#include "gui/dRadioButton.h"

namespace gui {
// A radio button with the animations of ButtonEx (e.g. the key sets of the keyboards: deciding one selects it and
// puts the selected one of the group back). Only the class name is from the binary (RTTI); member and function names
// are ours.
// RTTI N3gui13RadioButtonExE @ 0x008D0E3C
// vtable 0x00903EA4 (vptr 0x00903EAC), offset_to_top 0, 15 entries
class RadioButtonEx : public ::gui::ButtonEx
{
public:
    struct Description : ButtonEx::Description
    {
        // (inline)
        Description() : group(0) {}

        s32 group;  // 0xE8 the buttons of a group select each other off (the keyboards use four-character codes)
    };

    // (inline) the description; its sounds left at SOUND_DEFAULT get those of RadioButton
    explicit RadioButtonEx(const Description& desc) : ButtonEx(desc), m_Desc2(desc), m_IsSelectedByCode(false)
    {
        static_cast<ButtonEx::Description&>(m_Desc2) = m_Desc;
        for (s32 i = 0; i < SOUND_NUM; i++) {
            if (desc.sounds[i] == SOUND_DEFAULT) {
                m_Desc2.sounds[i] = RadioButton::s_DefaultSounds[i];
            }
        }
    }
    ~RadioButtonEx() override; // 0x004FAFD4 slot 0x00
    // 0x004FAFA4 slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_RADIO_BUTTON_EX; } // 0x00747584 slot 0x0C
    void UpdatePressed() override; // 0x004FAE84 slot 0x2C
    void UpdateDecided() override; // 0x004FAE54 slot 0x30

    // the button is selected, the selected one of the group goes back to STATE_IDLE (see RadioButton::Select)
    void Select(bool playAnimation); // 0x004FAD4C

    Description m_Desc2;        // 0x11C
    bool m_IsSelectedByCode;    // 0x208
};
ASSERT_SIZE(RadioButtonEx::Description, 0xEC);
ASSERT_SIZE(RadioButtonEx, 0x20C);
} // namespace gui
