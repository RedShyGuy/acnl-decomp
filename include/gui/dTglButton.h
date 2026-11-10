#pragma once

#include "decomp.h"
#include "gui/dButtonEx.h"

namespace gui {
// A toggle button: each decision switches it on or off; the description of the button (m_Desc) is replaced by the
// one of the new state (the "on" one has its own sub animations of the first three animations and plays the sound of
// switching off, the "off" one that of switching on). Only the class name is from the binary (RTTI); member and
// function names are ours.
// RTTI N3gui9TglButtonE @ 0x008D0EF8
// vtable 0x00904204 (vptr 0x0090420C), offset_to_top 0, 15 entries
class TglButton : public ::gui::ButtonEx
{
public:
    struct Description : ButtonEx::Description
    {
        // (inline)
        Description()
        {
            for (s32 i = 0; i < 3; i++) {
                for (s32 j = 0; j < SUB_ANIM_NUM; j++) {
                    onSubAnims[i][j] = NULL;
                }
            }
        }

        // the sub animations of ANIM_PRESS, ANIM_SLIDE_OUT and ANIM_DECIDE while the button is on
        lyt::AnmObject* onSubAnims[3][SUB_ANIM_NUM];  // 0xE8
    };

    explicit TglButton(const Description& desc); // 0x00501154
    ~TglButton() override; // 0x005012EC slot 0x00
    // 0x005012BC slot 0x04 (deleting dtor)
    u32 GetKind() const override { return KIND_TOGGLE_BUTTON; } // 0x00747878 slot 0x0C
    // (switched off)
    void SetEnabled(bool enabled, bool playAnimation, bool keepAnimation) override; // 0x00500FD0 slot 0x10
    void Reset(bool isRelease) override; // 0x00500EB4 slot 0x1C
    void UpdatePressed() override; // 0x00500D58 slot 0x2C
    void UpdateDecided() override; // 0x00500D38 slot 0x30

    // switched on or off, back to STATE_IDLE
    void SetOn(bool isOn); // 0x00500F3C

    ButtonEx::Description m_OnDesc;   // 0x11C
    ButtonEx::Description m_OffDesc;  // 0x204
    bool m_IsOn;                      // 0x2EC
};
ASSERT_SIZE(TglButton::Description, 0x124);
ASSERT_OFFSET(TglButton, m_IsOn, 0x2EC);
ASSERT_SIZE(TglButton, 0x2F0);
} // namespace gui
