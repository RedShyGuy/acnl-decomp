#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace font {
// the process of the font manager: Calc runs the loading of the font that is loaded on request
// RTTI N4font4BaseE @ 0x008D10F0
// vtable 0x00904874 (vptr 0x0090487C), offset_to_top 0, 16 entries
class Base : public ::Base
{
public:
    Base(); // 0x0052F120 (name after the RTTI)
    virtual ~Base(); // 0x0052F148 slot 0x00 (name after the RTTI)
    // 0x0052F138 slot 0x04 (deleting dtor)
    virtual oml::framework::Result Initialize(); // 0x0052F0C4 slot 0x0C
    virtual oml::framework::Result Finalize(); // 0x0052F104 slot 0x18
    virtual oml::framework::Result Calc(); // 0x0052F0E8 slot 0x24
    virtual oml::framework::Result Draw(); // 0x0052F0A8 slot 0x30
};
} // namespace font
