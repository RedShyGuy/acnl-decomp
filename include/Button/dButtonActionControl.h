#pragma once

#include "decomp.h"
#include "ssys/st/dList.h"

// RTTI 19ButtonActionControl @ 0x008CC9C4
// vtable 0x008F4F58 (vptr 0x008F4F60), offset_to_top 0, 15 entries
class ButtonActionControl : public ::ssys::st::List
{
public:
    struct Description { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~ButtonActionControl(); // 0x002F8188 slot 0x00 | libgarden
    // 0x002F80A4 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    virtual void Update(); // 0x002F7BA8 slot 0x08 | libgarden
    virtual void Reset(); // 0x002F779C slot 0x0C | libgarden
    virtual void vf_0x10(); // 0x002F6188 slot 0x10 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x14(); // 0x002F7BCC slot 0x14 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x18(); // 0x002F6DDC slot 0x18 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x1C(); // 0x002F5EFC slot 0x1C | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x20(); // 0x002F655C slot 0x20 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x24(); // 0x002F6498 slot 0x24 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x28(); // 0x002F74C4 slot 0x28 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x2C(); // 0x002F6EE8 slot 0x2C | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x30(); // 0x002F6EE4 slot 0x30 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x34(); // 0x002F6DD8 slot 0x34 | virtual slot, introduced by ButtonActionControl
    virtual void vf_0x38(); // 0x002F7300 slot 0x38 | virtual slot, introduced by ButtonActionControl
    void State_Selecting(); // 0x002F6928 | libgarden [tier A]
    void UnselectCurrent(); // 0x002F6DA4 | libgarden [tier A]
    void UnselectActive(); // 0x002F7394 | libgarden [tier A]
    void Initialize(ButtonActionControl::Description const&, char const*); // 0x002F7810 | libgarden [tier A]
    void AddNode(ButtonActionNode*, bool); // 0x002F7F30 | libgarden [tier A]
    ButtonActionControl(); // 0x002F7FD0 | libgarden [tier A]
    void GetPressedIndex() const; // 0x007241B0 | libgarden [tier A]
    void GetSelectedIndex() const; // 0x00724214 | libgarden [tier A]
    void IsSelectOkDone() const; // 0x00724498 | libgarden [tier A]
};
