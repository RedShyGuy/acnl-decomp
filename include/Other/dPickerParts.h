#pragma once

#include "decomp.h"
#include "Other/dPickerBase.h"

// A picker of 3x3 colors of one hue (class name from the RTTI; the other names are ours): the colors i + index * 16 of
// the color table at the cell (index % 4, index / 4) of 3x3 pixels of the texture.
// RTTI 11PickerParts @ 0x008CB310
// vtable 0x008ECC7C (vptr 0x008ECC84), offset_to_top 0, 3 entries
class PickerParts : public ::PickerBase
{
public:
    PickerParts(); // 0x001CE950

    void Setup(const SetupArg& arg) override; // 0x001CE6BC slot 0x00
    u8 Pick() override; // 0x001CE56C slot 0x04
    void SetCursor(u32 index) override; // 0x001CE8BC slot 0x08

    s32 m_CellX;     // 0x20 the first pixel in the texture
    s32 m_CellY;     // 0x24
    u8 m_Colors[9];  // 0x28 row by row
};
ASSERT_SIZE(PickerParts, 0x34);
