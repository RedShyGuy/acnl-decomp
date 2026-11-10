#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "state/dMode.h"
#include "applet/dErrEulaMgr.h"
#include "sead/seadControllerWrapper.h"

class SvFgName;

// RTTI 9BsMenuMgr @ 0x008CD708
// vtable 0x008FA3D0 (vptr 0x008FA3D8), offset_to_top 0, 16 entries
// vtable 0x008FA418 (vptr 0x008FA420), offset_to_top -20, 3 entries
class BsMenuMgr : public ::Base, public ::state::Mode<BsMenuMgr>
{
public:
    struct Menu { u32 _unknown; }; // TODO: real type unknown (placeholder)
    // the bits of the controller wrapper of the menus (m_Controller): the constructor maps them with the pad config
    // 0x0084C678 to the bits of the CTR controller (Controller::Pad); names are ours after that mapping
    enum Input : u32
    {
        INPUT_A_0 = 1 << 0,        // (A as well)
        INPUT_B_1 = 1 << 1,        // (B as well)
        INPUT_A = 1 << 2,
        INPUT_B = 1 << 3,
        INPUT_X = 1 << 4,
        INPUT_Y = 1 << 5,
        INPUT_START = 1 << 6,
        INPUT_SELECT = 1 << 7,
        INPUT_L = 1 << 8,
        INPUT_R = 1 << 9,
        INPUT_DPAD_UP = 1 << 10,
        INPUT_DPAD_DOWN = 1 << 11,
        INPUT_DPAD_LEFT = 1 << 12,
        INPUT_DPAD_RIGHT = 1 << 13,
        INPUT_CPAD_UP = 1 << 14,
        INPUT_CPAD_DOWN_15 = 1 << 15,  // (circle pad down as well)
        INPUT_CPAD_DOWN = 1 << 16,
        INPUT_CPAD_LEFT = 1 << 17,
        INPUT_CPAD_RIGHT = 1 << 18,
    };
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
    // (m_IsCancel counts as a release of INPUT_A; gui::RepeatButton tests the bit with other inputs)
    bool IsRelease(u32 input) const
    {
        return !applet::ErrEulaMgr::IsInputBlocked() && (((input & INPUT_A) != 0 && m_IsCancel) || (m_Controller.mPadRelease & input) != 0);
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

    // (game code that is not decompiled yet; names are ours)
    // whether no menu is open any more
    static bool IsIdle(); // 0x006D51AC
    // the menu process with its argument
    static void OpenMenu(u32 menu, u32 arg); // 0x006D2810
    static bool Func6D2718(); // 0x006D2718
    static void Func6D2744(); // 0x006D2744
    static void Func6D29C4(u32 a, u32 b); // 0x006D29C4
    static void Func6D2A08(u32 menu); // 0x006D2A08
    // a filter of an inventory slot (the item and a value of the slot); true: the slot is meant
    typedef bool (*InventoryFilter)(const SvFgName* item, u32 value);
    // the mask of the 16 inventory slots of the current player that the filter accepts
    static u32 Func6D2AC8(InventoryFilter filter); // 0x006D2AC8
    static void Func6D322C(u32 arg, u32 a, u32 menu); // 0x006D322C
    static void Func6D3274(); // 0x006D3274
    static void Func6D33D8(u32 value); // 0x006D33D8
    static void Func6D3A74(u8 a, u32 b); // 0x006D3A74
    static void Func6D3B20(u8 menu, u32 arg); // 0x006D3B20
    static bool Func6D3C34(); // 0x006D3C34
    static bool Func6D3C68(); // 0x006D3C68
    static void Func6D3D90(u32 arg, u8 menu, u32 param); // 0x006D3D90
    static void Func6D3DF8(); // 0x006D3DF8
    static void Func6D3F60(); // 0x006D3F60
    static void Func6D3F8C(u32 menu, u32 arg, u32 a); // 0x006D3F8C
    static void Func6D40D4(); // 0x006D40D4
    static void Func6D4308(); // 0x006D4308
    static void Func6D448C(u32 menu, u32 arg); // 0x006D448C
    static void Func6D44B0(); // 0x006D44B0
    static void Func6D4724(u32 arg, u8 menu, u32 param); // 0x006D4724

    static u8 s_Unknown950D25;
    static u8 s_SelectedSlot;       // the pocket slot of the item menu
    static u8 s_Result;             // the result of the last menu
    static s16 s_Unknown950D42;
    static u16 s_PocketItemMask;    // the pocket slots marked in the item menu
    static u32 s_Unknown950D70;
    static u32 s_MenuParam;
    static u32 s_Unknown950D94;
    static u32 s_Unknown950DB0;

    static BsMenuMgr* s_pInstance;
    // flags of the menu system (name is ours)
    static u32 s_Flags;
};
ASSERT_OFFSET(BsMenuMgr, m_HandMode, 0x40);
ASSERT_OFFSET(BsMenuMgr, m_Controller, 0x44);
ASSERT_OFFSET(BsMenuMgr, m_pBsHandCursor, 0x1F4);
ASSERT_SIZE(BsMenuMgr, 0x20C);
