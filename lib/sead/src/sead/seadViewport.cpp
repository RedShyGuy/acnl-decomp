#include "sead/seadViewport.h"
#include "sead/seadFrameBuffer.h"

namespace sead {
// 0x00561D4C (name is ours)
void sead::Viewport::setByFrameBuffer(const LogicalFrameBuffer& frameBuffer)
{
    switch (mDevicePosture) {
    case Graphics::DEVICE_POSTURE_SAME:
    case Graphics::DEVICE_POSTURE_ROTATE_180:
    case Graphics::DEVICE_POSTURE_FLIP_X:
    case Graphics::DEVICE_POSTURE_FLIP_Y:
        set(0.0f, 0.0f, frameBuffer.mVirtualSize.x, frameBuffer.mVirtualSize.y);
        break;
    case Graphics::DEVICE_POSTURE_ROTATE_90:
    case Graphics::DEVICE_POSTURE_ROTATE_270:
        set(0.0f, 0.0f, frameBuffer.mVirtualSize.y, frameBuffer.mVirtualSize.x);
        break;
    default:
        break;
    }
}

// 0x00561DF8 (name is ours)
sead::Viewport::Viewport(const LogicalFrameBuffer& frameBuffer)
    : BoundBox2<f32>(), mDevicePosture(Graphics::s_DefaultDevicePosture)
{
    setByFrameBuffer(frameBuffer);
}

// 0x00561E4C (name is ours)
sead::Viewport::Viewport() : BoundBox2<f32>(), mDevicePosture(Graphics::s_DefaultDevicePosture)
{
    const LogicalFrameBuffer* frameBuffer = FrameBuffer::s_pBoundFrameBuffer;
    if (frameBuffer != nullptr) {
        setByFrameBuffer(*frameBuffer);
    } else {
        set(0.0f, 0.0f, 32.0f, 32.0f);
    }
}

// 0x00561F6C slot 0x00
// 0x00561F68 (deleting dtor)
sead::Viewport::~Viewport()
{
    // empty (in the original too)
}
} // namespace sead
