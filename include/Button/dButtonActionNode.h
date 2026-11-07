#pragma once

#include "decomp.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "ssys/st/dListNode.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dLayout.h"
#include "Sound/dSoundLabel.h"

namespace nw {
namespace lyt {
class Group;
class Pane;
} // namespace lyt
} // namespace nw

// A button of a menu layout: a pane with four animations (touched, selected, and their "ok" animations played when it
// is chosen) and two sounds. The ButtonActionControl that holds it in its list drives it. Class, slot and member names
// after libgarden except where noted.
// RTTI 16ButtonActionNode @ 0x008CC2C0
// vtable 0x008F28E4 (vptr 0x008F28EC), offset_to_top 0, 16 entries
class ButtonActionNode : public ::ssys::st::ListNode
{
public:
    // called with the position of the hand cursor on the button instead of moving the cursor (name is ours)
    typedef void (*CursorCallback)(const nn::math::VEC3* pos, s32 index);

    struct Info
    {
        nw::lyt::Pane* pane;            // 0x00
        nw::lyt::Group* group;          // 0x04 the animations are bound to the group (else to the whole layout)
        nn::math::VEC2 pos;             // 0x08 the offset of the hand cursor from the pane
        u32 input;                      // 0x10 the pad bits that choose the button directly
        s32 index;                      // 0x14
        CursorCallback cursorCallback;  // 0x18 (libgarden: unk)
    };
    ASSERT_SIZE(Info, 0x1C);

    ButtonActionNode(); // 0x002B73AC | libgarden [tier A]
    virtual ~ButtonActionNode(); // 0x002B74C4 slot 0x00 | libgarden
    // 0x002B7454 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual bool IsActivated() const; // 0x0071DB7C slot 0x08 (name after libgarden)
    virtual void ResetSelect(); // 0x002B72E4 slot 0x0C | libgarden
    // called once per frame for the active buttons (empty here)
    virtual void FUN_002b72e0(); // 0x002B72E0 slot 0x10 (name after libgarden)
    virtual bool IsTouch() const; // 0x0071DAE0 slot 0x14 (name after libgarden)
    virtual bool IsCursorHover() const; // 0x0071DB38 slot 0x18 (name after libgarden)
    virtual void TouchSelect(bool touch, bool sound); // 0x002B7350 slot 0x1C (name after libgarden)
    virtual void Select(bool select, bool sound); // 0x002B7234 slot 0x20 (name after libgarden)
    virtual void BeginSelectOk(); // 0x002B7040 slot 0x24 (name after libgarden)
    virtual void EndSelectOk(); // 0x002B70F8 slot 0x28 (name after libgarden)
    virtual bool IsOkOngoing() const; // 0x002B7070 slot 0x2C (name after libgarden)
    virtual void DoOk(); // 0x002B7174 slot 0x30 (name after libgarden)
    virtual void Animate(ssys::ma::lyt::Animation& anim, bool set); // 0x002B71AC slot 0x34 (name after libgarden)
    virtual void PlayOkSound(); // 0x0071DB84 slot 0x38 (name after libgarden)
    virtual void PlaySound(); // 0x0071D9F8 slot 0x3C (name after libgarden)

    void Initialize(ButtonActionNode::Info const&); // 0x002B6FCC | libgarden [tier A]
    // (libgarden)
    DECOMP_NOINLINE void Select(bool set); // 0x002B72EC
    // (names are ours) bind/unbind an animation to the group or the layout; the frame of a bound one starts at 0
    void BindAnimation(ssys::ma::lyt::Animation& anim); // 0x002B7278
    void SetAnimationFrame(ssys::ma::lyt::Animation& anim, f32 frame); // 0x002B70C4
    // moves the hand cursor onto the button (or calls the cursor callback)
    void MoveHandCursor(); // 0x0071D944

    // (inline; the copy is called by the derived classes of the game)
    DECOMP_EMIT_COPY void UnbindAnimation(ssys::ma::lyt::Animation& anim) // 0x002B7008
    {
        if (anim.GetAnimTransform() != NULL && m_pLayout != NULL) {
            if (m_pGroup != NULL) {
                m_pLayout->UnbindAnimation(&anim, m_pGroup, false);
            } else {
                m_pLayout->UnbindAnimation(&anim);
            }
        }
    }
    // (inline) the animations of the control (name is ours)
    void SetAnimTransforms(nw::lyt::AnimTransform* touch, nw::lyt::AnimTransform* select,
                           nw::lyt::AnimTransform* touchOk, nw::lyt::AnimTransform* selectOk,
                           ssys::ma::lyt::Layout* layout)
    {
        m_pLayout = layout;
        m_Touch.SetAnimTransform(touch);
        m_Select.SetAnimTransform(select);
        m_TouchOk.SetAnimTransform(touchOk);
        m_SelectOk.SetAnimTransform(selectOk);
        m_Touch.SetFrame(0.0f);
        m_Select.SetFrame(0.0f);
    }

    // (libgarden)
    void SetSoundEnabled(bool set) { m_SoundEnabled = set; }
    void SetOkSoundEnabled(bool set) { m_OkSoundEnabled = set; }
    void SetSe(SeID id) { m_Se = id; }
    void SetSeOk(SeID id) { m_SeOk = id; }
    void Deactivate() { m_IsActive = false; }
    void Activate() { m_IsActive = true; }
    void Unselect() { Select(false); }
    s32 GetIndex() const { return m_Index; }

    nw::lyt::Pane* m_pPane;                   // 0x0C
    nw::lyt::Group* m_pGroup;                 // 0x10
    nn::math::VEC2 m_Position;                // 0x14 the offset of the hand cursor
    ssys::ma::lyt::Layout* m_pLayout;         // 0x1C
    ssys::ma::lyt::Animation m_Touch;         // 0x20
    ssys::ma::lyt::Animation m_Select;        // 0x48
    ssys::ma::lyt::Animation m_TouchOk;       // 0x70
    ssys::ma::lyt::Animation m_SelectOk;      // 0x98
    CursorCallback m_pCursorCallback;         // 0xC0 (name is ours; libgarden: unk)
    u32 m_Input;                              // 0xC4
    s32 m_Index;                              // 0xC8
    SeID m_SeOk;                              // 0xCC
    SeID m_Se;                                // 0xD0
    // passed to SoundMgr::SetNextSoundValue before the ok sound (-1: none; name is ours, libgarden: unk2)
    s16 m_SoundValue;                         // 0xD4
    // the control chooses the button in its next frame (name is ours, libgarden: unk3)
    u8 m_IsOkRequested;                       // 0xD6
    bool m_IsPressed;                         // 0xD7 touched or selected
    u8 m_Unknown0xD8;                         // 0xD8 (libgarden: unk4)
    bool m_IsActive;                          // 0xD9
    bool m_OkSoundEnabled;                    // 0xDA
    bool m_SoundEnabled;                      // 0xDB
    bool m_SoundMode;                         // 0xDC true: SoundMgr::SPlaySystemSound
    // chosen again and again while it is held (name is ours, libgarden: m_IsAvailable)
    bool m_IsRepeat;                          // 0xDD
    // never accessed, but the members of the derived classes start at 0xE0 (ARMCC does not use the tail padding)
    u8 _paddingDE[2];                         // 0xDE
};
ASSERT_OFFSET(ButtonActionNode, m_Touch, 0x20);
ASSERT_OFFSET(ButtonActionNode, m_pCursorCallback, 0xC0);
ASSERT_OFFSET(ButtonActionNode, m_SoundValue, 0xD4);
ASSERT_SIZE(ButtonActionNode, 0xE0);
