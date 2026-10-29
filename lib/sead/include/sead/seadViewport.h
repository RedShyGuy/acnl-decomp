#pragma once

#include "decomp.h"
#include "sead/seadBoundBox2.h"
#include "sead/seadGraphics.h"

namespace sead {
class LogicalFrameBuffer;

// RTTI N4sead8ViewportE @ 0x008D2258
// vtable 0x00906EF8 (vptr 0x00906F00), offset_to_top 0, 2 entries
//
// The area of a frame buffer that is drawn to, with the device posture (names are ours).
class Viewport : public ::sead::BoundBox2<float>
{
public:
    // the whole bound frame buffer (32 x 32 without one)
    Viewport(); // 0x00561E4C
    // the whole frame buffer
    explicit Viewport(const LogicalFrameBuffer& frameBuffer); // 0x00561DF8
    virtual ~Viewport(); // 0x00561F6C slot 0x00
    // 0x00561F68 slot 0x04 (deleting dtor)

    // the whole frame buffer (its size swapped for a turned posture)
    void setByFrameBuffer(const LogicalFrameBuffer& frameBuffer); // 0x00561D4C

    Graphics::DevicePosture mDevicePosture; // 0x14
};
ASSERT_SIZE(Viewport, 0x18);
} // namespace sead
