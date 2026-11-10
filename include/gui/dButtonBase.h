#pragma once

#include <cstring>

#include "decomp.h"
#include "gui/dWidget.h"
#include "lyt/dAnmObject.h"

namespace lyt {
class Object;
} // namespace lyt

namespace gui {
// The base of the buttons of the software keyboard: a group of bounding panes of a layout (lyt::Object) that is
// touched, with an animation and a sound per state. Only the class name is from the binary (RTTI); member and function
// names are ours.
// RTTI N3gui10ButtonBaseE @ 0x008D0DB8
// vtable 0x00903C70 (vptr 0x00903C78), offset_to_top 0, 15 entries
class ButtonBase : public ::gui::Widget
{
public:
    // the values of m_State (Update calls the slot of the state)
    enum State
    {
        STATE_IDLE,      // UpdateIdle: waits for a touch
        STATE_PRESSED,   // UpdatePressed: touched, the touch is on the button
        STATE_DECIDED,   // UpdateDecided: released on the button
        STATE_SLID_OUT,  // UpdateSlidOut: the touch left the button
        STATE_LOCKED,    // no input, SetEnabled keeps it
        STATE_DISABLED,  // UpdateDisabled
        STATE_NUM
    };
    // the animations of a description (an animation that a state has not falls back to ANIM_PRESS played backwards)
    enum Anim
    {
        ANIM_PRESS,      // touched (its start frame is the idle look)
        ANIM_SLIDE_OUT,  // the touch left the button
        ANIM_DECIDE,     // released on the button
        ANIM_RELEASE,    // Reset(true)
        ANIM_DISABLE,    // SetEnabled(false, true)
        ANIM_NUM
    };
    // the sounds of a description
    enum Sound
    {
        SOUND_PRESS,
        SOUND_PRESS_AGAIN,  // the touch came back onto the button
        SOUND_SLIDE_OUT,
        SOUND_DECIDE,
        SOUND_DISABLED,     // touched while disabled
        SOUND_NUM
    };
    // the values that the sounds of a description start with: the default sound of the class
    static const s32 SOUND_DEFAULT = -100;

    // what a button is made of (the derived classes extend it)
    struct Description
    {
        lyt::Object* pLayout;         // 0x00
        const char* groupName;        // 0x04 the group of the bounding panes that are touched
        lyt::AnmObject* anims[ANIM_NUM]; // 0x08 Anim
        s32 sounds[SOUND_NUM];        // 0x1C Sound (SOUND_DEFAULT: the default sound of the class; <= 0: none)
        s32 m_Unknown0x30;            // 0x30 (-1 in all descriptions)
        u32 keys;                     // 0x34 the keys of BsMenuMgr that press the button (only RepeatButton)
    };

    ButtonBase(); // 0x004F3A64
    ~ButtonBase() override; // 0x004F3AF8 slot 0x00
    // 0x004F3AC8 slot 0x04 (deleting dtor)
    // enabled (STATE_IDLE) or disabled (STATE_DISABLED); with playAnimation the disabling animation is played,
    // otherwise the animation of STATE_IDLE is set; keepAnimation: the animations are not touched; STATE_LOCKED stays
    virtual void SetEnabled(bool enabled, bool playAnimation, bool keepAnimation); // 0x004F394C slot 0x10
    // (only while the layout is attached to a parent) the layout is shown or hidden, the button enabled or disabled
    virtual void SetVisible(bool visible, bool playAnimation); // 0x004F372C slot 0x14
    virtual void SetActive(bool active); // 0x004F393C slot 0x18
    // back to STATE_IDLE (not while disabled); isRelease: the animation of releasing is played
    virtual void Reset(bool isRelease); // 0x004F389C slot 0x1C
    virtual bool IsEnabled() const { return m_State != STATE_DISABLED; } // 0x0074752C slot 0x20
    virtual s32 GetState() const { return m_State; } // 0x00747540 slot 0x24
    virtual void UpdateIdle(); // 0x004F3888 slot 0x28
    virtual void UpdatePressed(); // 0x004F3894 slot 0x2C
    virtual void UpdateDecided(); // 0x004F388C slot 0x30
    virtual void UpdateSlidOut(); // 0x004F3898 slot 0x34
    virtual void UpdateDisabled(); // 0x004F3890 slot 0x38

protected:
    // the animations of the description that play are stopped
    DECOMP_NOINLINE void StopAnimations(); // 0x004F379C
    // (inline) the sounds left at SOUND_DEFAULT get the defaults, the description and the group name are taken
    void SetDescription(Description* desc, const s32* defaultSounds)
    {
        for (s32 i = 0; i < SOUND_NUM; i++) {
            if (desc->sounds[i] == SOUND_DEFAULT) {
                desc->sounds[i] = defaultSounds[i];
            }
        }
        m_pDesc = desc;
        std::strncpy(m_GroupName, desc->groupName, 16);
    }

public:
    s32 m_State;            // 0x10 State
    bool m_IsActive;        // 0x14 the button gets its input
    Description* m_pDesc;   // 0x18 (the copy in the derived class)
    char m_GroupName[17];   // 0x1C
};
ASSERT_OFFSET(ButtonBase, m_pDesc, 0x18);
ASSERT_SIZE(ButtonBase, 0x30);

namespace detail {
// (inline) the animation is stopped if it plays
inline void StopIfPlaying(lyt::AnmObject* anim)
{
    if (anim->m_State == lyt::AnmObject::STATE_PLAYING || anim->m_State == lyt::AnmObject::STATE_STOPPING) {
        anim->Stop();
    }
}
// (inline) the animation is played once from the start
inline void PlayOnce(lyt::AnmObject* anim, s32 playMode)
{
    anim->SetPlayMode(playMode);
    anim->Restart();
}
// (inline) the start (isStart) or the end frame of the animation is applied
inline void ApplyLimit(lyt::AnmObject* anim, bool isStart)
{
    anim->SetPlayMode(lyt::AnmObject::PLAY_MODE_ONCE);
    anim->ApplyLimitFrame(isStart);
}
} // namespace detail
} // namespace gui
