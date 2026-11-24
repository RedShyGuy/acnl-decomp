#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Other/dLayout.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "state/dMode.h"

namespace nw {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw

// The moving background of the menus (class name from the RTTI, the state names from the table of the binary, ChangeBg
// after libgarden, all other names are ours): Layout/Menu/mvmnt_undscrn.arc, built twice - for the lower screen and as a
// copy for the upper screen. The frame of the "kind" animation selects the pattern of the background: one for the
// current stage (UpdateStageFrame) or one for the menu (ChangeBg); the upper screen has its own pattern (StartChange).
// RTTI 8BsMenuBg @ 0x008CD484
// vtable 0x008F99AC (vptr 0x008F99B4), offset_to_top 0, 16 entries
// vtable 0x008F99F4 (vptr 0x008F99FC), offset_to_top -20, 3 entries
class BsMenuBg : public ::Base, public ::state::Mode<BsMenuBg>
{
public:
    BsMenuBg(); // 0x00695EC8
    // 0x00695F88 (deleting dtor)
    ~BsMenuBg() override; // 0x00695FF4 slot 0x00
    oml::framework::Result Initialize() override; // 0x006959E4 slot 0x0C
    oml::framework::Result Finalize() override; // 0x00695CB0 slot 0x18
    oml::framework::Result Calc() override; // 0x00695BF4 slot 0x24
    oml::framework::Result Draw() override; // 0x00695988 slot 0x30

    // the background of the lower screen changes to the pattern of a menu (-1: the pattern of the stage)
    void ChangeBg(long kind); // 0x006953F0 | libgarden [tier A]
    // the background of both screens goes back to the pattern of the stage, the upper screen hides
    void Reset(); // 0x0069562C
    // the upper screen changes to kind (0: it goes out); frames: the frame of the kind animation (6: the one of the menu)
    void StartChange(s8 kind, u8 frames); // 0x00695E90
    // (copy for the game) the change of the upper screen has ended
    DECOMP_EMIT_COPY DECOMP_NOINLINE bool IsChangeDone() const // 0x00766F70
    {
        return ::state::Mode<BsMenuBg>::GetState() != &BsMenuBg::State_ChangeTop;
    }
    // (inline) the background does not change
    bool IsWaiting() const { return ::state::Mode<BsMenuBg>::GetState() == &BsMenuBg::State_Wait; }
    DECOMP_EMIT_COPY DECOMP_NOINLINE static BsMenuBg* GetInstance() { return s_pInstance; } // 0x006952A8

    // the panes, animations and the 3D effect of the layouts
    void Setup(); // 0x00695CF4
    // the frame of the pattern of the current stage; a change of it changes the background
    void UpdateStageFrame(); // 0x006952C0

    // the states (the names in the comments are the debug names of the binary)
    void State_Wait();       // 0x006952B8 待機 (wait)
    void Enter_Wait();       // 0x006952BC
    void State_ChangeBg();   // 0x006956B4 背景替え (changing the background)
    void Enter_ChangeBg();   // 0x006957C8
    void State_ChangeTop();  // 0x00695468 上画面着替え (changing the upper screen)
    void Enter_ChangeTop();  // 0x006954B8

    typedef ::state::Mode<BsMenuBg>::Info StateInfo;
    static const StateInfo s_StateInfos[3];
    static BsMenuBg* s_pInstance;

    ssys::ma::lyt::ArcResAccReader m_Reader;   // 0x038 Layout/Menu/mvmnt_undscrn.arc
    ::Layout m_Layout;                          // 0x280 mvmnt_undscrn.bclyt (lower screen)
    ::Layout m_TopLayout;                       // 0x3CC mvmnt_undscrn.bclyt (upper screen)
    ssys::ma::lyt::Animation m_LoopAnim;        // 0x518 mvmnt_undscrn_loop (lower screen)
    ssys::ma::lyt::Animation m_KindAnim;        // 0x540 mvmnt_undscrn_kind (lower screen)
    ssys::ma::lyt::Animation m_ChangeAnim;      // 0x568 mvmnt_undscrn_change (lower screen)
    ssys::ma::lyt::Animation m_TopLoopAnim;     // 0x590 mvmnt_undscrn_loop (upper screen)
    ssys::ma::lyt::Animation m_TopKindAnim;     // 0x5B8 mvmnt_undscrn_kind (upper screen)
    ssys::ma::lyt::Animation m_TopChangeAnim;   // 0x5E0 mvmnt_undscrn_change (upper screen)
    nw::lyt::Pane* m_pBase0;                    // 0x608 P_base_00 (lower screen)
    nw::lyt::Pane* m_pBase1;                    // 0x60C P_base_01 (lower screen; the new pattern while changing)
    bool m_IsShown;                             // 0x610 drawn and calculated while set (set by BsMenuMgr every frame)
    s8 m_TopKind;                               // 0x611 StartChange
    bool m_IsTopShown;                          // 0x612 the upper screen is drawn
    s32 m_MenuKind;                             // 0x614 ChangeBg (-1: the stage)
    u8 m_TopFrames;                             // 0x618 StartChange
    f32 m_KindFrame;                            // 0x61C the frame of the kind animation being changed to
    f32 m_StageFrame;                           // 0x620 the frame of the kind animation of the current stage
};
ASSERT_OFFSET(BsMenuBg, m_Reader, 0x38);
ASSERT_OFFSET(BsMenuBg, m_Layout, 0x280);
ASSERT_OFFSET(BsMenuBg, m_LoopAnim, 0x518);
ASSERT_OFFSET(BsMenuBg, m_pBase0, 0x608);
ASSERT_SIZE(BsMenuBg, 0x624);
