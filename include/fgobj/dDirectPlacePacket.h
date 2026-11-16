#pragma once

#include "decomp.h"
#include "fgobj/fgobj_Api.h"
#include "Sv/dSvFgName.h"

namespace fgobj {
// places an item directly at a unit, also on the other consoles (packet type 0x83; layout after libgarden)
struct DirectPlacePacket {
    DirectPlacePacket(); // 0x00594E40
    ~DirectPlacePacket(); // 0x00594E60
    void Set(const SvFgName& fg, u8 x, u8 y, stage::Name stage); // 0x00594E18 (name after libgarden)
    void Place() const; // 0x00594D88 | libgarden [tier A]
    void Send() const; // 0x00594E04 | libgarden [tier A]

    /* 0x0 */ SvFgName fg;
    /* 0x4 */ stage::Name stage;
    /* 0x5 */ Position8 pos;
    /* 0x7 */ u8 pad;
};
ASSERT_SIZE(DirectPlacePacket, 0x8);
} // namespace fgobj
