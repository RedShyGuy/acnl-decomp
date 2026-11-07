#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"
#include "applet/dErrEulaMgr.h"
#include "sead/seadControllerWrapper.h"

// RTTI 9BsMenuMgr @ 0x008CD708
// vtable 0x008FA3D0 (vptr 0x008FA3D8), offset_to_top 0, 16 entries
// vtable 0x008FA418 (vptr 0x008FA420), offset_to_top -20, 3 entries
class BsMenuMgr : public ::Base, public ::state::Mode<BsMenuMgr>
{
public:
    struct Menu { u32 _unknown; }; // TODO: real type unknown (placeholder)
    BsMenuMgr(); // ctor candidate(s) 0x006D52B4 (unverified)
    virtual ~BsMenuMgr(); // 0x006D53A4 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x006D5378 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x006D4978 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual oml::framework::Result Finalize(); // 0x006D5158 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual oml::framework::Result Calc(); // 0x006D4CE0 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x006D4970 slot 0x30 | slot vf_0x30 of oml::framework::Process
    void CanOpenMenuThink(); // 0x005C4B04 | libgarden [tier A]
    void CreateMenuProcess(BsMenuMgr::Menu, unsigned char); // 0x006D2B4C | libgarden [tier A]
    bool IsPressed(unsigned long input); // 0x006D4C24 | libgarden [tier A]

    // (libgarden; the original calls the copy instead of inlining it)
    DECOMP_EMIT_COPY DECOMP_NOINLINE DECOMP_NOIPA static BsMenuMgr* Get() { return s_pInstance; } // 0x006D29B4

    // the inline input tests of the menus (names are ours; no input while the error applet blocks it)
    bool IsTrigOrRepeat(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && ((m_Controller.mPadTrig | m_Controller.mPadRepeat) & input) != 0;
    }
    bool IsHold(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && !m_IsCancel && (m_Controller.mPadHold & input) != 0;
    }
    // (m_IsCancel counts as released)
    bool IsRelease(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && (m_IsCancel || (m_Controller.mPadRelease & input) != 0);
    }
    // the circle pad (zero while the input is blocked)
    const sead::Vector2<f32>& GetLeftStick() const
    {
        if (applet::ErrEulaMgr::IsInputBlocked()) {
            // 0x00983614 (guard 0x00983610)
            static const sead::Vector2<f32> s_ZeroStick(0.0f, 0.0f);
            return s_ZeroStick;
        }
        return m_Controller.mLeftStick;
    }

    // (member names after libgarden)
    u8 m_Unk[8];                        // 0x38
    u8 m_HandMode;                      // 0x40 1: the hand cursor is used
    u8 m_Unknown0x41;                   // 0x41
    u8 m_Unknown0x42;                   // 0x42
    sead::ControllerWrapper m_Controller; // 0x44
    s32 m_Unk1;                         // 0x1D4
    void* m_pBsMenuMain;                // 0x1D8
    void* m_pBsMenuBg;                  // 0x1DC
    void* m_pBsMenuTab;                 // 0x1E0
    void* m_pProcess;                   // 0x1E4
    void* m_pRoomMgr;                   // 0x1E8
    s32 m_Unk2[2];                      // 0x1EC
    BsHandCursor* m_pBsHandCursor;      // 0x1F4
    void* m_pBsMenuRandomHeadsUp;       // 0x1F8
    // the cursor id of the ButtonActionControl that owns the cursor (14 at the start; 12/13 = cancelled)
    u8 m_HandGrabTimer;                 // 0x1FC
    u8 m_CurrentActionNodeIndex;        // 0x1FD the node index of that control
    u8 m_Unknown0x1FE;                  // 0x1FE
    s32 m_Unk3[2];                      // 0x200
    bool m_IsCancel;                    // 0x208

    static BsMenuMgr* s_pInstance;
    // flags of the menu system (name is ours)
    static u32 s_Flags;
};
ASSERT_OFFSET(BsMenuMgr, m_HandMode, 0x40);
ASSERT_OFFSET(BsMenuMgr, m_Controller, 0x44);
ASSERT_OFFSET(BsMenuMgr, m_pBsHandCursor, 0x1F4);
ASSERT_SIZE(BsMenuMgr, 0x20C);
