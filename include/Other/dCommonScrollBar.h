#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionNode.h"
#include "Other/dLayout.h"
#include "Other/dRepeatCounter.h"
#include "nn/math/math_ARMv6.h"
#include "nn/math/math_Vector2.h"
#include "nn/math/math_Vector3.h"
#include "nw/lyt/lyt_Pane.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/st/dList.h"

namespace nw {
namespace lyt {
class Group;
class Pane;
} // namespace lyt
} // namespace nw
class ContentNode;

// the sizes of the scroll bar (the frame of scrl_bar_kind.bclan; libgarden)
enum class ScrollBarSize : u8
{
    LARGE,
    MEDIUM,
    SMALL,
};

// what CommonScrollBar::Initialize takes (layout and member names after libgarden except the callbacks')
struct ScrollBarDescription
{
    typedef void (*Callback)(void* container);

    ScrollBarSize size;      // 0x00
    f32 scrollSpace;         // 0x04 the distance of one scroll step
    s32 scrollSteps1;        // 0x08 the visible items
    s32 scrollSteps2;        // 0x0C the items
    s32 unk0;                // 0x10
    Callback onScroll;       // 0x14 (libgarden: callback1) called with the container when the scroll changes
    Callback onSelect;       // 0x18 (libgarden: callback2) called when the selected item changes by the pad
};
ASSERT_SIZE(ScrollBarDescription, 0x1C);

// The scroll bar of the lists of the menus (Layout/common/scrl_bar.arc): a thumb that can be dragged by touch or with
// the hand cursor, two arrow buttons, the cursor keys with a key repeat, and the panes of the list (ContentNode) that
// move with the scroll. Class, Initialize/Finalize/SetParent/Process and m_Scroll after libgarden; the other names
// are ours.
// RTTI 15CommonScrollBar @ 0x008CC018
// vtable 0x008F18B0 (vptr 0x008F18B8), offset_to_top 0, 7 entries
class CommonScrollBar
{
public:
    // the values of m_Mode: what moves the scroll bar
    enum Mode : u8
    {
        MODE_NONE,
        MODE_PAD,          // the cursor keys
        MODE_TOUCH,        // the touch screen
        MODE_HAND_CURSOR,  // the hand cursor of the menus
    };

    CommonScrollBar(void* container); // 0x0029A0EC | libgarden [tier A]
    virtual ~CommonScrollBar(); // 0x0029A2C4 slot 0x00 | libgarden
    // 0x0029A254 slot 0x04 (deleting dtor)
    // a touch on the bar (not the thumb): the thumb jumps there and is dragged
    virtual void OnTouchTrack(const nn::math::VEC2& pos); // 0x002991CC slot 0x08
    // the touch on the thumb, the arrows and the bar
    virtual void UpdateTouch(); // 0x00298340 slot 0x0C
    // the dragged thumb follows the position
    virtual void OnDragMove(const nn::math::VEC2& pos); // 0x0029A080 slot 0x10
    // the thumb moves by dy this frame (falls into ClampThumbMove)
    virtual void MoveThumb(f32 dy); // 0x00298E08 slot 0x14
    // the panes of the list follow the scroll
    virtual void UpdateContents(); // 0x00298198 slot 0x18

    void Initialize(ScrollBarDescription&& desc); // 0x00299A14 | libgarden [tier A]
    void Finalize(); // 0x00133ACC | libgarden [tier A]
    void SetParent(nw::lyt::Pane* parent); // 0x0029811C | libgarden [tier A]
    void Process(); // 0x00299D38 | libgarden [tier A]
    s32 GetScroll() const { return m_Scroll; }

    // the scroll position (clamped); the thumb, the contents and the arrows follow
    DECOMP_NOINLINE void Scroll(s32 pos); // 0x002988AC
    // the number of items (isKeepScroll: the scroll stays where it can)
    void SetItemNum(u32 num, bool isKeepScroll); // 0x002984C0
    void SetUnknown0x754(u32 value); // 0x002994E0
    // a pane that moves with the scroll
    void AddContent(ContentNode* node); // 0x00299F5C
    void SetSize(ScrollBarSize size); // 0x00299F68
    // no input is processed (and the cursor of the buttons is off)
    DECOMP_NOINLINE void SetFrozen(bool isFrozen); // 0x00299F40

protected:
    // the global position of a pane (inline; the copy is called from CommonScrollBarWidth)
    DECOMP_EMIT_COPY nn::math::VEC3 GetGlobalPosition(const nw::lyt::Pane* pane) const // 0x0071B95C
    {
        nn::math::VEC3 pos(0.0f, 0.0f, 0.0f);
        nn::math::ARMv6::VEC3TransformAsm(&pos, &pane->m_GlbMtx, &pos);
        return pos;
    }

    // the cursor keys move the selection (or the scroll)
    DECOMP_NOINLINE void UpdatePad(); // 0x00297E3C
    // the arrow button shows that it cannot scroll further (and is inactive)
    DECOMP_NOINLINE void SetArrowNg(bool isNg, s32 index); // 0x002981FC
    // N_All is shown when the items do not fit
    void UpdateAllVisible(); // 0x00298310
    DECOMP_NOINLINE void UpdateHandCursor(); // 0x0029857C
    DECOMP_NOINLINE void UpdateDrag(); // 0x00298768
    DECOMP_NOINLINE void UpdateRepeat(); // 0x00298AF0
    // the size of the thumb for the number of items
    DECOMP_NOINLINE void UpdateThumbSize(); // 0x00298CAC
    bool IsOnBar(const nn::math::VEC2& pos); // 0x00298DF0
    // the thumb stays on the bar (the arrows become inactive at the ends)
    void ClampThumbMove(); // 0x00298E14
    // the arrow buttons scroll by one
    DECOMP_NOINLINE void UpdateArrowButtons(); // 0x00298F0C
    DECOMP_NOINLINE void InitializeButtons(); // 0x00299028
    DECOMP_NOINLINE void MoveHandCursorToThumb(); // 0x0029916C
    DECOMP_NOINLINE void UpdateMode(); // 0x00299260
    void PlayUpSound(bool isRepeat); // 0x00299450
    bool IsOnThumb(const nn::math::VEC2& pos); // 0x00299494
    // the scroll from the position of the dragged thumb
    DECOMP_NOINLINE void UpdateScrollFromThumb(); // 0x002994E8
    void UpdateMaxScroll(); // 0x0029962C
    void PlayDownSound(bool isRepeat); // 0x00299650
    DECOMP_NOINLINE void FindPanes(); // 0x00299694
    DECOMP_NOINLINE void LeaveMode(); // 0x002997B0
    // the thumb is let go: the scroll snaps to the nearest step
    DECOMP_NOINLINE void EndDrag(); // 0x002998B4
    void UpdateThumbBase(); // 0x00299938
    // (falls into SetThumbSelected(true))
    void StartDrag(); // 0x0029995C
    DECOMP_NOINLINE void SetThumbSelected(bool isSelected); // 0x00299968

public:
    ssys::ma::lyt::ArcResAccReader m_ArcReader;   // 0x004
    Layout m_Layout;                              // 0x24C scrl_bar.bclyt
    ssys::ma::lyt::Animation m_SelectAnim;        // 0x398 scrl_bar_select.bclan (the thumb is selected)
    ssys::ma::lyt::Animation m_TouchAnim;         // 0x3C0 scrl_bar_touch.bclan (an arrow is pressed)
    ssys::ma::lyt::Animation m_KindAnim;          // 0x3E8 scrl_bar_kind.bclan (the size)
    ssys::ma::lyt::Animation m_NgAnim;            // 0x410 scrl_bar_ng.bclan (an arrow cannot scroll)
    ssys::st::List m_Contents;                    // 0x438 ContentNodes
    ScrollBarSize m_Size;                         // 0x444
    ButtonActionNode m_Arrows[2];                 // 0x448 up, down
    ButtonActionControl m_Control;                // 0x608
    RepeatCounter m_Repeat;                       // 0x6EC
    bool m_IsFrozen;                              // 0x6F8
    bool m_IsThumbSelected;                       // 0x6F9
    bool m_IsDragging;                            // 0x6FA
    bool m_IsTrackTouched;                        // 0x6FB (this frame)
    bool m_IsPadEnabled;                          // 0x6FC
    bool m_IsScrollByPad;                         // 0x6FD the cursor keys scroll instead of moving the selection
    bool m_IsBoard;                               // 0x6FE board_scrl_bar layout (CommonScrollBarWidth); the arrow buttons do nothing
    bool m_IsSelectSoundPlayed;                   // 0x6FF
    bool m_IsPadModeEntered;                      // 0x700 (this frame)
    bool m_Unknown0x701;                          // 0x701 (Finalize disables the hand cursor)
    bool m_IsHorizontal;                         // 0x702 the arrows play the left/right sounds
    bool m_IsAudioSelectSound;                   // 0x703 a new selection plays SE_SYS_AUDIO_TITLE_SELECTED
    f32 m_BarTop;                                 // 0x704 the y of N_barT_pos_00
    f32 m_BarBottom;                              // 0x708 the y of N_barB_pos_00
    f32 m_ScrollPerUnit;                          // 0x70C scroll space per unit of the bar
    f32 m_ThumbMove;                              // 0x710 the move of the thumb in this frame
    f32 m_ThumbTop;                               // 0x714 the y of the thumb at scroll 0
    f32 m_GrabOffset;                             // 0x718 from the thumb to the touch
    s32 m_Scroll;                                 // 0x71C (libgarden)
    s32 m_MaxScroll;                              // 0x720
    f32 m_ScrollRemainder;                        // 0x724 the part of a step the dragged thumb is beyond the scroll
    s32 m_Selected;                               // 0x728 (-1: none)
    f32 m_HalfSpace;                              // 0x72C
    f32 m_ThumbPan;                               // 0x730 the sound pans
    f32 m_UpArrowPan;                             // 0x734
    f32 m_DownArrowPan;                           // 0x738
    f32 m_SoundTop;                               // 0x73C the drag sound plays when the thumb passes these
    f32 m_SoundBottom;                            // 0x740
    Mode m_Mode;                                  // 0x744
    f32 m_ScrollSpace;                            // 0x748
    s32 m_VisibleNum;                             // 0x74C
    s32 m_ItemNum;                                // 0x750
    u32 m_Unknown0x754;                           // 0x754 (at least the item number)
    ScrollBarDescription::Callback m_OnScroll;    // 0x758
    void (*m_OnSelect)(void* container, s32 selected, s32 prev); // 0x75C
    void* m_pContainer;                           // 0x760
    nw::lyt::Pane* m_pThumb;                      // 0x764 N_btn_00
    nw::lyt::Pane* m_pThumbCenter;                // 0x768 P_btnC_00
    nw::lyt::Pane* m_pThumbTopShadow;             // 0x76C P_btnT_sh_00
    nw::lyt::Pane* m_pThumbCenterShadow;          // 0x770 P_btnC_sh_00
    nw::lyt::Pane* m_pThumbBottomShadow;          // 0x774 P_btnB_sh_00
    nw::lyt::Pane* m_pThumbTopBounding;           // 0x778 B_btnT_00
    nw::lyt::Pane* m_pThumbCenterBounding;        // 0x77C B_btnC_00
    nw::lyt::Pane* m_pThumbBottomBounding;        // 0x780 B_btnB_00
    nw::lyt::Pane* m_pBar;                        // 0x784 B_bar_00
    nw::lyt::Pane* m_pAll;                        // 0x788 N_All
    nw::lyt::Group* m_pThumbGroup;                // 0x78C G_btn_00
    nw::lyt::Group* m_pUpArrowGroup;              // 0x790 G_triT_00
    nw::lyt::Group* m_pDownArrowGroup;            // 0x794 G_triB_00
};
ASSERT_OFFSET(CommonScrollBar, m_Layout, 0x24C);
ASSERT_OFFSET(CommonScrollBar, m_Arrows, 0x448);
ASSERT_OFFSET(CommonScrollBar, m_Repeat, 0x6EC);
ASSERT_OFFSET(CommonScrollBar, m_Scroll, 0x71C);
ASSERT_OFFSET(CommonScrollBar, m_pThumb, 0x764);
ASSERT_SIZE(CommonScrollBar, 0x798);
