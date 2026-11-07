#pragma once

#include "decomp.h"
#include "Button/dButtonActionControl.h"
#include "Button/dButtonActionNodeEx.h"

// A control of ButtonActionNodeEx: four more animations for the "back" animations of its buttons. Class and member
// names after libgarden.
// RTTI 21ButtonActionControlEx @ 0x008CCCCC
// vtable 0x008F612C (vptr 0x008F6134), offset_to_top 0, 15 entries
class ButtonActionControlEx : public ::ButtonActionControl
{
public:
    struct Description
    {
        ssys::ma::lyt::ResAccInterface* resAcc;  // 0x00
        ssys::ma::lyt::Layout* layout;           // 0x04
        nw::lyt::Bounding* bounding;             // 0x08
        const char* touch;                       // 0x0C the names of the animations
        const char* select;                      // 0x10
        const char* touchOk;                     // 0x14
        const char* selectOk;                    // 0x18
        const char* touchBack;                   // 0x1C
        const char* selectBack;                  // 0x20
        const char* touchOkBack;                 // 0x24
        const char* selectOkBack;                // 0x28
        s32 cursorTimer;                         // 0x2C
        Predicate checkInput;                    // 0x30
    };
    ASSERT_SIZE(Description, 0x34);

    ButtonActionControlEx(); // 0x00328338 (name after libgarden)
    virtual ~ButtonActionControlEx(); // 0x003283AC slot 0x00 (name after libgarden)
    // 0x00328370 slot 0x04 | slot vf_0x04 of ssys::st::List (deleting dtor)
    // (libgarden declares a bool result; the code returns nothing)
    void Initialize(ButtonActionControlEx::Description const&); // 0x003281D8 | libgarden [tier A]
    void AddNode(ButtonActionNodeEx*, bool); // 0x003282D4 | libgarden [tier A]

    ssys::ma::lyt::Animation m_TouchBack;     // 0x0E4
    ssys::ma::lyt::Animation m_SelectBack;    // 0x10C
    ssys::ma::lyt::Animation m_TouchOkBack;   // 0x134
    ssys::ma::lyt::Animation m_SelectOkBack;  // 0x15C
};
ASSERT_SIZE(ButtonActionControlEx, 0x184);
