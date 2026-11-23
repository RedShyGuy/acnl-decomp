#pragma once

#include "decomp.h"
#include "Other/dPickerBase.h"

// The picker of the 15 shades without hue (class name from the RTTI; the other names are ours): the colors 15 + i * 16 of
// the color table in a row (row 15 of the texture).
// RTTI 10PickerMono @ 0x008CB11C
// vtable 0x008EC2B0 (vptr 0x008EC2B8), offset_to_top 0, 3 entries
class PickerMono : public ::PickerBase
{
public:
    // (inline in the constructor of Palette)
    PickerMono() {}

    void Setup(const SetupArg& arg) override; // 0x001AF560 slot 0x00
    u8 Pick() override; // 0x001AF450 slot 0x04
    void SetCursor(u32 index) override; // 0x001AF724 slot 0x08

    u8 m_Colors[15]; // 0x20
};
ASSERT_SIZE(PickerMono, 0x30);
