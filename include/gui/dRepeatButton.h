#pragma once

#include "decomp.h"
#include "gui/dButtonBase.h"

namespace gui {
// A button that repeats: held long enough (touch or its keys) it decides again every few frames; a short press
// decides when it is released. It can also be pressed with keys of BsMenuMgr (Description::keys). Only the class name
// is from the binary (RTTI); member and function names are ours.
// RTTI N3gui12RepeatButtonE @ 0x008D0E24
// vtable 0x00903E3C (vptr 0x00903E44), offset_to_top 0, 15 entries
class RepeatButton : public ::gui::ButtonBase
{
public:
    struct Description : ButtonBase::Description
    {
        Description(); // 0x004FA624

        lyt::AnmObject* repeatAnim;  // 0x38 looped while held
        s32 repeatDelay;             // 0x3C the frames before the first repeat
        s32 repeatInterval;          // 0x40 the frames between the repeats
        bool isPlayRepeatAnim;       // 0x44
        s32 repeatSound;             // 0x48 the system sound of each repeat (<= 0: none)
        s32 decideSound;             // 0x4C the system sound of a decision (with SOUND_DECIDE; <= 0: none)
        bool isCenterSound;          // 0x50 SOUND_DECIDE is played without pan
        s16 soundValue;              // 0x52 the value of the system sounds
    };

    explicit RepeatButton(const Description& desc); // 0x004FA784
    ~RepeatButton() override; // 0x004FA864 slot 0x00
    // 0x004FA834 slot 0x04 (deleting dtor)
    void Update() override; // 0x004FA69C slot 0x08
    u32 GetKind() const override { return KIND_REPEAT_BUTTON; } // 0x00747578 slot 0x0C
    void UpdateIdle() override; // 0x004F9F50 slot 0x28
    void UpdatePressed() override; // 0x004FA194 slot 0x2C
    void UpdateSlidOut() override; // 0x004FA59C slot 0x34
    void UpdateDisabled() override; // 0x004FA0EC slot 0x38

    // the repeat animation is played once
    void PlayRepeatAnimation(); // 0x004F9F14

    // the default sounds (the static initializer 0x00785A28 sets them to -1)
    static s32 s_DefaultSounds[SOUND_NUM];

private:
    // (inline) the event of a decision with its sounds
    void Decide();
    // (inline) one more frame held: the repeats
    void Repeat();
    // (inline) the touch or the key left: STATE_SLID_OUT
    void SlideOut();

public:
    Description m_Desc;    // 0x30
    s32 m_HoldFrames;      // 0x84
    bool m_IsKeyPress;     // 0x88 pressed with a key (released: no decision)
};
ASSERT_SIZE(RepeatButton::Description, 0x54);
ASSERT_SIZE(RepeatButton, 0x8C);
} // namespace gui
