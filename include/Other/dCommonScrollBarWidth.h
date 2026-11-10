#pragma once

#include "decomp.h"
#include "Other/dCommonScrollBar.h"

// The horizontal scroll bar (scrl_bar_W.bclyt, or the board_scrl_bar.bclyt of the bulletin board without working
// arrows): the thumb moves along x, a move to the right scrolls back (only the class name is from the RTTI).
// RTTI 20CommonScrollBarWidth @ 0x008CCB70
// vtable 0x008F5814 (vptr 0x008F581C), offset_to_top 0, 7 entries
class CommonScrollBarWidth : public ::CommonScrollBar
{
public:
    CommonScrollBarWidth(void* container); // 0x0031DBC0
    virtual ~CommonScrollBarWidth(); // 0x0031DC48 slot 0x00
    // 0x0031DBD8 slot 0x04 (deleting dtor)
    void OnTouchTrack(const nn::math::VEC2& pos) override; // 0x0031D7D0 slot 0x08
    void UpdateTouch() override; // 0x0031D4DC slot 0x0C
    void OnDragMove(const nn::math::VEC2& pos) override; // 0x0031DB6C slot 0x10
    void MoveThumb(f32 dx) override; // 0x0031D5E8 slot 0x14
    void UpdateContents() override; // 0x0031D470 slot 0x18

    // like CommonScrollBar::Initialize with the horizontal layout (isBoard: the one of the bulletin board); the
    // font GARDEN_MSG_16 is registered for the texts of the layout
    void Initialize(ScrollBarDescription&& desc, bool isBoard); // 0x0031D844
    // the arrow buttons; a chosen one is reset at once
    void UpdateButtons(); // 0x0031D5F8

private:
    DECOMP_NOINLINE void InitializeButtons(); // 0x0031D634
};
